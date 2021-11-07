#pragma once

#include "DICore.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDirectInput, Log, All);

struct DIDeviceState
{
	enum { AxisCount = 8, PovCount = 4, ButtonCount = 32 };
	float Axes[AxisCount] = {};
	float Povs[PovCount] = {};
	uint8_t Buttons[ButtonCount] = {};
};

struct DIDeviceImpl
{
	DIDeviceImpl();
	~DIDeviceImpl();

	static TArray<FDIDeviceInfo> Enumerate();
	bool Open(const FString& InInstanceGuid, HWND Window = nullptr);
	void Close();

	void Tick();
	float GetAxis(int Id) const;
	float GetPov(int Id) const;
	bool GetButton(int Id) const;

	void EnableFeedback(bool Enabled);
	void SetFeedback(float Constant, float Damper);

	void _InitFeedback();
	void _ShutFeedback();
	void _SubmitFeedback(bool EnableAutostart);

	std::unique_ptr<IDirectInput8, ComDtor> DInput;
	std::unique_ptr<IDirectInputDevice8, ComDtor> Device;
	std::unique_ptr<IDirectInputEffect, ComDtor> ConstantFx;
	std::unique_ptr<IDirectInputEffect, ComDtor> DamperFx;

	FDIDeviceInfo DeviceInfo;
	FDIDeviceConfiguration Configuration;
	DIDeviceState State;
	DIDeviceState OldState;

	bool FeedbackEnabled = false;
	bool ConstantFxStarted = false;
	bool DamperFxStarted = false;

	float ConstantValue = 0;
	float DamperValue = 0;
};
