#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "GenericPlatform/GenericApplicationMessageHandler.h"

struct DIRECTINPUT_API FDIWheelKeyNames
{
	static const FGamepadKeyNames::Type DIWheel_Axis_Steer;
	static const FGamepadKeyNames::Type DIWheel_Axis_Clutch;
	static const FGamepadKeyNames::Type DIWheel_Axis_Brake;
	static const FGamepadKeyNames::Type DIWheel_Axis_Gas;
	static const FGamepadKeyNames::Type DIWheel_Axis_HandBrake;
	
	static const FGamepadKeyNames::Type DIWheel_Button_GearUp;
	static const FGamepadKeyNames::Type DIWheel_Button_GearDown;
	static const FGamepadKeyNames::Type DIWheel_Button_GearN;
	static const FGamepadKeyNames::Type DIWheel_Button_GearR;
	static const FGamepadKeyNames::Type DIWheel_Button_Gear1;
	static const FGamepadKeyNames::Type DIWheel_Button_Gear2;
	static const FGamepadKeyNames::Type DIWheel_Button_Gear3;
	static const FGamepadKeyNames::Type DIWheel_Button_Gear4;
	static const FGamepadKeyNames::Type DIWheel_Button_Gear5;
	static const FGamepadKeyNames::Type DIWheel_Button_Gear6;
	static const FGamepadKeyNames::Type DIWheel_Button_Gear7;
};

struct DIRECTINPUT_API FDIWheelKeys
{
	static const FKey DIWheel_Axis_Steer;
	static const FKey DIWheel_Axis_Clutch;
	static const FKey DIWheel_Axis_Brake;
	static const FKey DIWheel_Axis_Gas;
	static const FKey DIWheel_Axis_HandBrake;
	
	static const FKey DIWheel_Button_GearUp;
	static const FKey DIWheel_Button_GearDown;
	static const FKey DIWheel_Button_GearN;
	static const FKey DIWheel_Button_GearR;
	static const FKey DIWheel_Button_Gear1;
	static const FKey DIWheel_Button_Gear2;
	static const FKey DIWheel_Button_Gear3;
	static const FKey DIWheel_Button_Gear4;
	static const FKey DIWheel_Button_Gear5;
	static const FKey DIWheel_Button_Gear6;
	static const FKey DIWheel_Button_Gear7;

	static void RegisterKeys();
};
