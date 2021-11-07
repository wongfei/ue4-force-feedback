#include "DIEngineDevice.h"
#include "DIDeviceImpl.h"
#include "DIKeys.h"
#include "DIWheelKeys.h"

#include "Framework/Application/SlateApplication.h"
#include "Windows/WindowsApplication.h"
#include "GameFramework/InputSettings.h"
#include "UEInputUtils.h"

FDIEngineDevice::FDIEngineDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
:
	MessageHandler(InMessageHandler)
{
	UE_LOG(LogDirectInput, Display, TEXT("+FDIEngineDevice"));

	const TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().GetActiveTopLevelWindow();
	UE_LOG(LogDirectInput, Display, TEXT("Window -> \"%s\""), *(ParentWindow->GetTitle().ToString()));

	if (ParentWindow.IsValid() && (ParentWindow->GetNativeWindow().IsValid()))
	{
		EngineWindow = (HWND)ParentWindow->GetNativeWindow()->GetOSWindowHandle();
	}

	InitConfiguration();
}

FDIEngineDevice::~FDIEngineDevice()
{
	for (auto* Dev : ActiveDevices)
		delete Dev;
}

void FDIEngineDevice::InitConfiguration()
{
	UE_LOG(LogDirectInput, Display, TEXT("InitConfiguration"));

	DefaultDriving.Uid = TEXT("DefaultDriving");
	DefaultDriving.AxisProperties.Add(FDIDeviceAxisProperties(0, FDIWheelKeys::DIWheel_Axis_Steer, false));
	DefaultDriving.AxisProperties.Add(FDIDeviceAxisProperties(1, FDIWheelKeys::DIWheel_Axis_Gas, true));
	DefaultDriving.AxisProperties.Add(FDIDeviceAxisProperties(5, FDIWheelKeys::DIWheel_Axis_Brake, true));
	DefaultDriving.AxisProperties.Add(FDIDeviceAxisProperties(7, FDIWheelKeys::DIWheel_Axis_Clutch, true));
	DefaultDriving.ButtonProperties.Add(FDIDeviceButtonProperties(4, FDIWheelKeys::DIWheel_Button_GearUp));
	DefaultDriving.ButtonProperties.Add(FDIDeviceButtonProperties(5, FDIWheelKeys::DIWheel_Button_GearDown));
	DefaultDriving.FeedbackProperties.Tag = TEXT("Steer");
	DefaultDriving.FeedbackProperties.Enabled = true;

	auto* Settings = GetMutableDefault<UDISettings>();
	FDIDeviceConfiguration Existing;

	EnumeratedDevices = DIDeviceImpl::Enumerate();
	for (const auto& Dev : EnumeratedDevices)
	{
		UE_LOG(LogDirectInput, Display, TEXT("DEVICE"));
		UE_LOG(LogDirectInput, Display, TEXT("\tInstanceGuid: %s"), *Dev.InstanceGuid);
		UE_LOG(LogDirectInput, Display, TEXT("\tProductGuid:  %s"), *Dev.ProductGuid);
		UE_LOG(LogDirectInput, Display, TEXT("\tInstanceName: %s"), *Dev.InstanceName);
		UE_LOG(LogDirectInput, Display, TEXT("\tProductName:  %s"), *Dev.ProductName);

		if (!Settings->FindByUid(Dev.Uid, Existing))
		{
			FDIDeviceConfiguration DevConfig;

			if (Dev.Type == DI8DEVTYPE_DRIVING)
				DevConfig = DefaultDriving;

			DevConfig.Uid = Dev.Uid;

			Settings->DeviceConfigurations.Emplace(MoveTemp(DevConfig));
		}
	}

	UpdateConfigurationFlag = true;
}

void FDIEngineDevice::UpdateConfiguration()
{
	//UE_LOG(LogDirectInput, Display, TEXT("UpdateConfiguration"));

	const auto* Settings = GetDefault<UDISettings>();

	for (const auto& DevConfig : Settings->DeviceConfigurations)
	{
		auto* Dev = GetDevice(DevConfig.Uid);

		if (DevConfig.Enabled && !Dev) // open
		{
			auto* DevInfo = FindByUid(EnumeratedDevices, DevConfig.Uid);
			if (DevInfo)
			{
				Dev = new DIDeviceImpl();
				Dev->Open(DevInfo->InstanceGuid, (HWND)EngineWindow);
				ActiveDevices.Add(Dev);
			}
		}
		else if (!DevConfig.Enabled && Dev) // close
		{
			RemoveBindings(Dev);
			ActiveDevices.Remove(Dev);
			delete Dev;
			Dev = nullptr;
		}

		if (DevConfig.Enabled && Dev) // update
		{
			Dev->Configuration = DevConfig;
			Dev->EnableFeedback(DevConfig.FeedbackProperties.Enabled);
			SetupBindings(Dev);
		}
	}

	UInputSettings::GetInputSettings()->ForceRebuildKeymaps();
	UpdateConfigurationFlag = false;
}

void FDIEngineDevice::SetupBindings(DIDeviceImpl* Dev)
{
	auto* Settings = UInputSettings::GetInputSettings();

	for (auto& Axis : Dev->Configuration.AxisProperties)
	{
		Axis.Uid = *FString::Printf(TEXT("%s Axis %d"), *Dev->DeviceInfo.Uid, Axis.AxisId);
		auto Key = AddUniqKey(Axis.Uid);
		Rebind(Settings, Key, Axis.RemapAxis, true);
	}

	for (auto& Button : Dev->Configuration.ButtonProperties)
	{
		Button.Uid = *FString::Printf(TEXT("%s Button %d"), *Dev->DeviceInfo.Uid, Button.ButtonId);
		auto Key = AddUniqKey(Button.Uid);
		Rebind(Settings, Key, Button.RemapAction, false);
	}
}

void FDIEngineDevice::RemoveBindings(DIDeviceImpl* Dev)
{
	auto* Settings = UInputSettings::GetInputSettings();

	for (auto& Axis : Dev->Configuration.AxisProperties)
	{
		Rebind(Settings, FKey(Axis.Uid), NAME_None, true);
	}

	for (auto& Button : Dev->Configuration.ButtonProperties)
	{
		Rebind(Settings, FKey(Button.Uid), NAME_None, false);
	}
}

void FDIEngineDevice::Rebind(UInputSettings* Settings, const FKey& Key, const FName& NewTarget, bool IsAxis)
{
	const auto KeyName = Key.GetFName();
	const auto* Old = InputMapper.Find(KeyName);

	if (Old)
	{
		if (*Old == NewTarget)
		{
			// nothing changed
			return;
		}

		if (IsAxis)
			RemoveAxisMapping(Settings, Key, *Old);
		else
			RemoveActionMapping(Settings, Key, *Old);

		InputMapper.Remove(KeyName);
	}

	if (NewTarget != NAME_None)
	{
		bool Added;
		if (IsAxis)
			Added = AddUniqAxisMapping(Settings, Key, NewTarget);
		else
			Added = AddUniqActionMapping(Settings, Key, NewTarget);

		if (Added)
			InputMapper.Add(KeyName, NewTarget);
	}
}

void FDIEngineDevice::RequestUpdateConfiguration()
{
	UpdateConfigurationFlag = true;
}

/** Poll for controller state and send events if needed */
void FDIEngineDevice::SendControllerEvents()
{
	if (UpdateConfigurationFlag)
	{
		UpdateConfiguration();
	}

	int ControllerId = 0;
	for (auto* Dev : ActiveDevices)
	{
		Dev->Tick();

		for (const auto& Axis : Dev->Configuration.AxisProperties)
		{
			const int Id = Axis.AxisId;
			float AxisValue = Dev->State.Axes[Id];

			if (fabsf(AxisValue - Dev->OldState.Axes[Id]) > 0.0001f)
			{
				if (Axis.IsPedal)
				{
					float RangeMin = 1;
					float RangeMax = -1;
					AxisValue = (AxisValue - RangeMin) / (RangeMax - RangeMin);
				}

				if (Axis.Invert)
					AxisValue *= -1.0f;

				AxisValue *= Axis.Scale;

				MessageHandler->OnControllerAnalog(Axis.Uid, ControllerId, AxisValue);

				if (Axis.RemapKey.IsValid())
					MessageHandler->OnControllerAnalog(Axis.RemapKey.GetFName(), ControllerId, AxisValue);

				//UE_LOG(LogDirectInput, Display, TEXT("Axis%d -> %f"), Id, AxisValue);
			}
		}

		for (const auto& Button : Dev->Configuration.ButtonProperties)
		{
			const int Id = Button.ButtonId;
			const int ButtonValue = Dev->State.Buttons[Id];

			if (ButtonValue != Dev->OldState.Buttons[Id])
			{
				if (ButtonValue)
					MessageHandler->OnControllerButtonPressed(Button.Uid, ControllerId, false);
				else
					MessageHandler->OnControllerButtonReleased(Button.Uid, ControllerId, false);

				if (Button.RemapKey.IsValid())
				{
					if (ButtonValue)
						MessageHandler->OnControllerButtonPressed(Button.RemapKey.GetFName(), ControllerId, false);
					else
						MessageHandler->OnControllerButtonReleased(Button.RemapKey.GetFName(), ControllerId, false);
				}

				//UE_LOG(LogDirectInput, Display, TEXT("Button%d -> %d"), Id, ButtonValue);
			}
		}

		Dev->OldState = Dev->State;
		ControllerId++;
	}
}

void FDIEngineDevice::SetForceFeedback(const FString& DeviceTag, float Constant, float Damper)
{
	for (auto* Dev : ActiveDevices)
	{
		const auto& FeedbackProperties = Dev->Configuration.FeedbackProperties;

		if (FeedbackProperties.Enabled && FeedbackProperties.Tag == DeviceTag)
		{
			Dev->SetFeedback(Constant * FeedbackProperties.ConstantGain, Damper * FeedbackProperties.DamperGain);
			break;
		}
	}
}

DIDeviceImpl* FDIEngineDevice::GetDevice(const FString& Uid)
{
	for (auto* Dev : ActiveDevices)
	{
		if (Dev->DeviceInfo.Uid == Uid)
		{
			return Dev;
		}
	}
	return nullptr;
}

/** Tick the interface (e.g. check for new controllers) */
void FDIEngineDevice::Tick( float DeltaTime ) {}

/** Set which MessageHandler will get the events from SendControllerEvents. */
void FDIEngineDevice::SetMessageHandler( const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler ) { MessageHandler = InMessageHandler; }

/** Exec handler to allow console commands to be passed through for debugging */
bool FDIEngineDevice::Exec( UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar ) { return false; }

/**
 * IForceFeedbackSystem pass through functions
 */
void FDIEngineDevice::SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value) { UE_LOG(LogDirectInput, Display, TEXT("SetChannelValue %d %f"), ControllerId, Value); }
void FDIEngineDevice::SetChannelValues(int32 ControllerId, const FForceFeedbackValues &values) { UE_LOG(LogDirectInput, Display, TEXT("SetChannelValues %d"), ControllerId); }
bool FDIEngineDevice::SupportsForceFeedback(int32 ControllerId) { return true; }

/**
 * Pass though functions for light color
 */
void FDIEngineDevice::SetLightColor(int32 ControllerId, FColor Color) {}
void FDIEngineDevice::ResetLightColor(int32 ControllerId) {}

/**
* Sets a property for a given controller id.
* Will be ignored for devices which don't support the property.
*
* @param ControllerId the id of the controller whose property is to be applied
* @param Property Base class pointer to property that will be applied
*/
void FDIEngineDevice::SetDeviceProperty(int32 ControllerId, const FInputDeviceProperty* Property) {}

/** If this device supports a haptic interface, implement this, and inherit the IHapticDevice interface */
class IHapticDevice* FDIEngineDevice::GetHapticDevice() { return nullptr; }

bool FDIEngineDevice::IsGamepadAttached() const { return false; }
