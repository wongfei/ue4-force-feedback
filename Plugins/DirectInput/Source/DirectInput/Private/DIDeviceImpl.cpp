#include "DIDeviceImpl.h"

#define DI_AXIS_MAX 10000

DEFINE_LOG_CATEGORY(LogDirectInput);

DIDeviceImpl::DIDeviceImpl()
{
}

DIDeviceImpl::~DIDeviceImpl()
{
	Close();
}

struct DIEnumState
{
	FString FilterGuid;
	TArray<DIDEVICEINSTANCE> Devices;
};

static BOOL DIEnumCallback(LPCDIDEVICEINSTANCE Device, LPVOID Param)
{
	auto* Context = (DIEnumState*)Param;
	const auto DevGuid = GuidToString(Device->guidInstance);

	if (Context->FilterGuid.IsEmpty() || Context->FilterGuid == DevGuid)
	{
		Context->Devices.Add(*Device);

		if (!Context->FilterGuid.IsEmpty())
			return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}

// static
TArray<FDIDeviceInfo> DIDeviceImpl::Enumerate()
{
	TArray<FDIDeviceInfo> Result;

	IDirectInput8* TmpDInput = nullptr;
	auto hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&TmpDInput, nullptr);
	if (hr == S_OK)
	{
		DIEnumState Enumerator;
		TmpDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, DIEnumCallback, &Enumerator, DIEDFL_ATTACHEDONLY);
		TmpDInput->Release();

		for (const auto& Iter : Enumerator.Devices)
		{
			Result.Emplace(GetDeviceInfo(Iter));
		}
	}

	return MoveTemp(Result);
}

bool DIDeviceImpl::Open(const FString& InInstanceGuid, HWND Window)
{
	Close();

	UE_LOG(LogDirectInput, Display, TEXT("OpenDevice %s"), *InInstanceGuid);

	for (;;)
	{
		IDirectInput8* TmpDInput = nullptr;
		auto hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&TmpDInput, nullptr);
		DI_GUARD_HR(hr, TEXT("DirectInput8Create"), break);
		DInput.reset(TmpDInput);

		DIEnumState Enumerator;
		Enumerator.FilterGuid = *InInstanceGuid;
		hr = DInput->EnumDevices(DI8DEVCLASS_GAMECTRL, DIEnumCallback, &Enumerator, DIEDFL_ATTACHEDONLY);
		DI_GUARD_HR(hr, TEXT("EnumDevices"), break);

		if (Enumerator.Devices.Num() == 0)
		{
			UE_LOG(LogDirectInput, Error, TEXT("Device not found: %s"), *InInstanceGuid);
			break;
		}

		IDirectInputDevice8* TmpDevice = nullptr;
		hr = DInput->CreateDevice(Enumerator.Devices[0].guidInstance, &TmpDevice, nullptr);
		DI_GUARD_HR(hr, TEXT("CreateDevice"), break);
		Device.reset(TmpDevice);
		DeviceInfo = GetDeviceInfo(Enumerator.Devices[0]);

		hr = Device->SetDataFormat(&c_dfDIJoystick);
		DI_GUARD_HR(hr, TEXT("SetDataFormat"), break);

		//HWND Window = (HWND)GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
		if (!Window)
		{
			UE_LOG(LogDirectInput, Warning, TEXT("Invalid Window param"));
			Window = GetForegroundWindow();
		}

		hr = Device->SetCooperativeLevel(Window, DISCL_EXCLUSIVE | DISCL_BACKGROUND);
		if (FAILED(hr))
		{
			hr = Device->SetCooperativeLevel(Window, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
			if (FAILED(hr))
			{
				hr = Device->SetCooperativeLevel(Window, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
			}
		}
		DI_GUARD_HR(hr, TEXT("SetCooperativeLevel"), break);

		{DI_MAKE_PROP(DIPROPRANGE, p);
		p.lMin = -DI_AXIS_MAX;
		p.lMax = DI_AXIS_MAX;
		hr = Device->SetProperty(DIPROP_RANGE, &p.diph);
		DI_GUARD_HR(hr, TEXT("SetProperty DIPROP_RANGE"), break);}

		{DI_MAKE_PROP(DIPROPDWORD, p);
		p.diph.dwHow = DIPH_DEVICE;
		p.dwData = 0;
		hr = Device->SetProperty(DIPROP_AUTOCENTER, &p.diph);}

		// SUCCESS
		return true;
	}

	// ERROR
	Close();
	return false;
}

void DIDeviceImpl::Close()
{
	if (Device)
	{
		UE_LOG(LogDirectInput, Display, TEXT("CloseDevice %s"), *DeviceInfo.InstanceGuid);
	}

	_ShutFeedback();

	Device.reset();
	DInput.reset();
}

void DIDeviceImpl::Tick()
{
	if (!Device)
		return;

	auto hr = Device->Poll();
	if (FAILED(hr))
	{
		hr = Device->Acquire();
		if (FAILED(hr))
			return;

		UE_LOG(LogDirectInput, Display, TEXT("Device acquired"));
	}

	DIJOYSTATE JoyState;
	FMemory::Memset(&JoyState, 0, sizeof(JoyState));
	hr = Device->GetDeviceState(sizeof(JoyState), &JoyState);

	const float Scale = 1.0f / (float)DI_AXIS_MAX;

	State.Axes[0] = JoyState.lX * Scale;
	State.Axes[1] = JoyState.lY * Scale;
	State.Axes[2] = JoyState.lZ * Scale;
	State.Axes[3] = JoyState.lRx * Scale;
	State.Axes[4] = JoyState.lRy * Scale;
	State.Axes[5] = JoyState.lRz * Scale;
	State.Axes[6] = JoyState.rglSlider[0] * Scale;
	State.Axes[7] = JoyState.rglSlider[1] * Scale;

	/*
	The position is indicated in hundredths of a degree clockwise from north (away from the user). 
	The center position is normally reported as - 1;
	Some drivers report the centered position of the POV indicator as 65,535. 
	Determine whether the indicator is centered as follows: BOOL POVCentered = (LOWORD(dwPOV) == 0xFFFF);
	For indicators that have only five positions, the value for a controller is - 1, 0, 9,000, 18,000, or 27,000.
	*/
	for (int i = 0; i < DIDeviceState::PovCount; ++i)
	{
		DWORD dwPOV = JoyState.rgdwPOV[i];
		BOOL POVCentered = (LOWORD(dwPOV) == 0xFFFF);
		State.Povs[i] = POVCentered ? -1.0f : (float)dwPOV;
	}

	for (int i = 0; i < DIDeviceState::ButtonCount; ++i)
		State.Buttons[i] = JoyState.rgbButtons[i] ? 1 : 0;

	// Force Feedback

	const bool FeedbackInitialized = (ConstantFx || DamperFx);

	if (FeedbackEnabled && !FeedbackInitialized)
		_InitFeedback();
	else if (!FeedbackEnabled && FeedbackInitialized)
		_ShutFeedback();

	if (FeedbackEnabled)
		_SubmitFeedback(true);
}

float DIDeviceImpl::GetAxis(int Id) const
{
	if (Id >= 0 && Id < DIDeviceState::AxisCount)
		return State.Axes[Id];
	return 0;
}

float DIDeviceImpl::GetPov(int Id) const
{
	if (Id >= 0 && Id < DIDeviceState::PovCount)
		return State.Povs[Id];
	return 0;
}

bool DIDeviceImpl::GetButton(int Id) const
{
	if (Id >= 0 && Id < DIDeviceState::ButtonCount)
		return State.Buttons[Id] ? true : false;
	return false;
}

void DIDeviceImpl::EnableFeedback(bool Enabled)
{
	FeedbackEnabled = Enabled;
}

void DIDeviceImpl::SetFeedback(float Constant, float Damper)
{
	ConstantValue = Constant;
	DamperValue = Damper;
}

void DIDeviceImpl::_InitFeedback()
{
	DWORD DiAxes[] = {0};
	LONG DiDir[] = {0};

	if (!ConstantFx)
	{
		DICONSTANTFORCE DiForce = {0};

		DIEFFECT DiFX;
		memset(&DiFX, 0, sizeof(DiFX));
		DiFX.dwSize = sizeof(DiFX);
		DiFX.dwFlags = 18;
		DiFX.dwDuration = INFINITE;
		DiFX.dwSamplePeriod = 0;
		DiFX.dwGain = DI_AXIS_MAX;
		DiFX.dwTriggerButton = DIEB_NOTRIGGER;
		DiFX.dwTriggerRepeatInterval = 0;
		DiFX.cAxes = 1;
		DiFX.rgdwAxes = DiAxes;
		DiFX.rglDirection = DiDir;
		DiFX.lpEnvelope = 0;
		DiFX.cbTypeSpecificParams = sizeof(DiForce);
		DiFX.lpvTypeSpecificParams = &DiForce;
		DiFX.dwStartDelay = 0;

		IDirectInputEffect* TmpEffect = nullptr;
		auto hr = Device->CreateEffect(GUID_ConstantForce, &DiFX, &TmpEffect, nullptr);
		DI_GUARD_HR(hr, TEXT("CreateEffect GUID_ConstantForce"), DI_NOP());
		ConstantFx.reset(TmpEffect);
	}

	if (!DamperFx)
	{
		DICONDITION DiCond = {
			0,
			DI_AXIS_MAX,
			DI_AXIS_MAX,
			DI_AXIS_MAX,
			DI_AXIS_MAX,
			0
		};

		DIEFFECT DiFX;
		memset(&DiFX, 0, sizeof(DiFX));
		DiFX.dwSize = sizeof(DiFX);
		DiFX.dwFlags = 18;
		DiFX.dwDuration = INFINITE;
		DiFX.dwSamplePeriod = 0;
		DiFX.dwGain = DI_AXIS_MAX;
		DiFX.dwTriggerButton = DIEB_NOTRIGGER;
		DiFX.dwTriggerRepeatInterval = 0;
		DiFX.cAxes = 1;
		DiFX.rgdwAxes = DiAxes;
		DiFX.rglDirection = DiDir;
		DiFX.lpEnvelope = 0;
		DiFX.cbTypeSpecificParams = sizeof(DiCond);
		DiFX.lpvTypeSpecificParams = &DiCond;
		DiFX.dwStartDelay = 0;

		IDirectInputEffect* TmpEffect = nullptr;
		auto hr = Device->CreateEffect(GUID_Damper, &DiFX, &TmpEffect, nullptr);
		DI_GUARD_HR(hr, TEXT("CreateEffect GUID_Damper"), DI_NOP());
		DamperFx.reset(TmpEffect);
	}
}

void DIDeviceImpl::_ShutFeedback()
{
	ConstantValue = 0;
	DamperValue = 0;

	if (ConstantFxStarted || DamperFxStarted)
		_SubmitFeedback(false);

	ConstantFxStarted = false;
	DamperFxStarted = false;

	ConstantFx.reset();
	DamperFx.reset();
}

void DIDeviceImpl::_SubmitFeedback(bool EnableAutostart)
{
	ConstantValue = FMath::Clamp(ConstantValue, -1.0f, 1.0f);
	DamperValue = FMath::Clamp(DamperValue, 0.0f, 1.0f);

	DWORD DiAxes[] = {0};
	LONG DiDir[] = {1};
	HRESULT hr = S_OK;

	if (ConstantFx)
	{
		DICONSTANTFORCE DiForce = {
			(LONG)(ConstantValue * DI_AXIS_MAX)
		};

		DIEFFECT DiFX;
		memset(&DiFX, 0, sizeof(DiFX));
		DiFX.dwSize = sizeof(DiFX);
		DiFX.dwFlags = 18;
		DiFX.cAxes = 1;
		DiFX.rgdwAxes = DiAxes;
		DiFX.rglDirection = DiDir;
		DiFX.cbTypeSpecificParams = sizeof(DiForce);
		DiFX.lpvTypeSpecificParams = &DiForce;

		if (ConstantFxStarted)
		{
			hr = ConstantFx->SetParameters(&DiFX, DIEP_TYPESPECIFICPARAMS);
		}
		else if (EnableAutostart)
		{
			hr = ConstantFx->SetParameters(&DiFX, DIEP_START | DIEP_DIRECTION | DIEP_TYPESPECIFICPARAMS);
			ConstantFxStarted = true;
		}

		if (hr != S_OK)
		{
			UE_LOG(LogDirectInput, Error, TEXT("ConstantFx error: hr=0x%X"), hr);
		}
	}

	if (DamperFx)
	{
		DICONDITION DiCond = {
			0,
			(LONG)(DamperValue * DI_AXIS_MAX),
			(LONG)(DamperValue * DI_AXIS_MAX),
			DI_AXIS_MAX,
			DI_AXIS_MAX,
			0
		};

		DIEFFECT DiFX;
		memset(&DiFX, 0, sizeof(DiFX));
		DiFX.dwSize = sizeof(DiFX);
		DiFX.dwFlags = 18;
		DiFX.dwGain = DI_AXIS_MAX;
		DiFX.cAxes = 1;
		DiFX.rgdwAxes = DiAxes;
		DiFX.rglDirection = DiDir;
		DiFX.cbTypeSpecificParams = sizeof(DiCond);
		DiFX.lpvTypeSpecificParams = &DiCond;

		if (DamperFxStarted)
		{
			hr = DamperFx->SetParameters(&DiFX, DIEP_TYPESPECIFICPARAMS);
		}
		else if (EnableAutostart)
		{
			hr = DamperFx->SetParameters(&DiFX, DIEP_START | DIEP_DIRECTION | DIEP_TYPESPECIFICPARAMS);
			DamperFxStarted = true;
		}

		if (hr != S_OK)
		{
			UE_LOG(LogDirectInput, Error, TEXT("DamperFx error: hr=0x%X"), hr);
		}
	}
}
