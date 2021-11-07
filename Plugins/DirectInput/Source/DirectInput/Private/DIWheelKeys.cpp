#include "DIWheelKeys.h"

#define LOCTEXT_NAMESPACE "DirectInputPlugin"

// FDIWheelKeyNames

const FGamepadKeyNames::Type FDIWheelKeyNames::DIWheel_Axis_Steer("DIWheel_Axis_Steer");
const FGamepadKeyNames::Type FDIWheelKeyNames::DIWheel_Axis_Clutch("DIWheel_Axis_Clutch");
const FGamepadKeyNames::Type FDIWheelKeyNames::DIWheel_Axis_Brake("DIWheel_Axis_Brake");
const FGamepadKeyNames::Type FDIWheelKeyNames::DIWheel_Axis_Gas("DIWheel_Axis_Gas");
const FGamepadKeyNames::Type FDIWheelKeyNames::DIWheel_Axis_HandBrake("DIWheel_Axis_HandBrake");

const FGamepadKeyNames::Type FDIWheelKeyNames::DIWheel_Button_GearUp("DIWheel_Button_GearUp");
const FGamepadKeyNames::Type FDIWheelKeyNames::DIWheel_Button_GearDown("DIWheel_Button_GearDown");
const FGamepadKeyNames::Type FDIWheelKeyNames::DIWheel_Button_GearN("DIWheel_Button_GearN");
const FGamepadKeyNames::Type FDIWheelKeyNames::DIWheel_Button_GearR("DIWheel_Button_GearR");
const FGamepadKeyNames::Type FDIWheelKeyNames::DIWheel_Button_Gear1("DIWheel_Button_Gear1");
const FGamepadKeyNames::Type FDIWheelKeyNames::DIWheel_Button_Gear2("DIWheel_Button_Gear2");
const FGamepadKeyNames::Type FDIWheelKeyNames::DIWheel_Button_Gear3("DIWheel_Button_Gear3");
const FGamepadKeyNames::Type FDIWheelKeyNames::DIWheel_Button_Gear4("DIWheel_Button_Gear4");
const FGamepadKeyNames::Type FDIWheelKeyNames::DIWheel_Button_Gear5("DIWheel_Button_Gear5");
const FGamepadKeyNames::Type FDIWheelKeyNames::DIWheel_Button_Gear6("DIWheel_Button_Gear6");
const FGamepadKeyNames::Type FDIWheelKeyNames::DIWheel_Button_Gear7("DIWheel_Button_Gear7");

// FDIWheelKeys

const FKey FDIWheelKeys::DIWheel_Axis_Steer(FDIWheelKeyNames::DIWheel_Axis_Steer);
const FKey FDIWheelKeys::DIWheel_Axis_Clutch(FDIWheelKeyNames::DIWheel_Axis_Clutch);
const FKey FDIWheelKeys::DIWheel_Axis_Brake(FDIWheelKeyNames::DIWheel_Axis_Brake);
const FKey FDIWheelKeys::DIWheel_Axis_Gas(FDIWheelKeyNames::DIWheel_Axis_Gas);
const FKey FDIWheelKeys::DIWheel_Axis_HandBrake(FDIWheelKeyNames::DIWheel_Axis_HandBrake);
	
const FKey FDIWheelKeys::DIWheel_Button_GearUp(FDIWheelKeyNames::DIWheel_Button_GearUp);
const FKey FDIWheelKeys::DIWheel_Button_GearDown(FDIWheelKeyNames::DIWheel_Button_GearDown);
const FKey FDIWheelKeys::DIWheel_Button_GearN(FDIWheelKeyNames::DIWheel_Button_GearN);
const FKey FDIWheelKeys::DIWheel_Button_GearR(FDIWheelKeyNames::DIWheel_Button_GearR);
const FKey FDIWheelKeys::DIWheel_Button_Gear1(FDIWheelKeyNames::DIWheel_Button_Gear1);
const FKey FDIWheelKeys::DIWheel_Button_Gear2(FDIWheelKeyNames::DIWheel_Button_Gear2);
const FKey FDIWheelKeys::DIWheel_Button_Gear3(FDIWheelKeyNames::DIWheel_Button_Gear3);
const FKey FDIWheelKeys::DIWheel_Button_Gear4(FDIWheelKeyNames::DIWheel_Button_Gear4);
const FKey FDIWheelKeys::DIWheel_Button_Gear5(FDIWheelKeyNames::DIWheel_Button_Gear5);
const FKey FDIWheelKeys::DIWheel_Button_Gear6(FDIWheelKeyNames::DIWheel_Button_Gear6);
const FKey FDIWheelKeys::DIWheel_Button_Gear7(FDIWheelKeyNames::DIWheel_Button_Gear7);

void FDIWheelKeys::RegisterKeys()
{
	const FName NAME_DIWheel(TEXT("DIWheel"));

	EKeys::AddMenuCategoryDisplayInfo(NAME_DIWheel, LOCTEXT("DIWheelSubCateogry", "DIWheel"), TEXT("GraphEditor.KeyEvent_16x"));

	EKeys::AddKey(FKeyDetails(FDIWheelKeys::DIWheel_Axis_Steer, LOCTEXT("DIWheel_Axis_Steer", "DIWheel Axis Steer"), FKeyDetails::GamepadKey, NAME_DIWheel));
	EKeys::AddKey(FKeyDetails(FDIWheelKeys::DIWheel_Axis_Clutch, LOCTEXT("DIWheel_Axis_Clutch", "DIWheel Axis Clutch"), FKeyDetails::GamepadKey, NAME_DIWheel));
	EKeys::AddKey(FKeyDetails(FDIWheelKeys::DIWheel_Axis_Brake, LOCTEXT("DIWheel_Axis_Brake", "DIWheel Axis Brake"), FKeyDetails::GamepadKey, NAME_DIWheel));
	EKeys::AddKey(FKeyDetails(FDIWheelKeys::DIWheel_Axis_Gas, LOCTEXT("DIWheel_Axis_Gas", "DIWheel Axis Gas"), FKeyDetails::GamepadKey, NAME_DIWheel));
	EKeys::AddKey(FKeyDetails(FDIWheelKeys::DIWheel_Axis_HandBrake, LOCTEXT("DIWheel_Axis_HandBrake", "DIWheel Axis HandBrake"), FKeyDetails::GamepadKey, NAME_DIWheel));

	EKeys::AddKey(FKeyDetails(FDIWheelKeys::DIWheel_Button_GearUp, LOCTEXT("DIWheel_Button_GearUp", "DIWheel Button GearUp"), FKeyDetails::GamepadKey, NAME_DIWheel));
	EKeys::AddKey(FKeyDetails(FDIWheelKeys::DIWheel_Button_GearDown, LOCTEXT("DIWheel_Button_GearDown", "DIWheel Button GearDown"), FKeyDetails::GamepadKey, NAME_DIWheel));
	EKeys::AddKey(FKeyDetails(FDIWheelKeys::DIWheel_Button_GearN, LOCTEXT("DIWheel_Button_GearN", "DIWheel Button GearN"), FKeyDetails::GamepadKey, NAME_DIWheel));
	EKeys::AddKey(FKeyDetails(FDIWheelKeys::DIWheel_Button_GearR, LOCTEXT("DIWheel_Button_GearR", "DIWheel Button GearR"), FKeyDetails::GamepadKey, NAME_DIWheel));
	EKeys::AddKey(FKeyDetails(FDIWheelKeys::DIWheel_Button_Gear1, LOCTEXT("DIWheel_Button_Gear1", "DIWheel Button Gear1"), FKeyDetails::GamepadKey, NAME_DIWheel));
	EKeys::AddKey(FKeyDetails(FDIWheelKeys::DIWheel_Button_Gear2, LOCTEXT("DIWheel_Button_Gear2", "DIWheel Button Gear2"), FKeyDetails::GamepadKey, NAME_DIWheel));
	EKeys::AddKey(FKeyDetails(FDIWheelKeys::DIWheel_Button_Gear3, LOCTEXT("DIWheel_Button_Gear3", "DIWheel Button Gear3"), FKeyDetails::GamepadKey, NAME_DIWheel));
	EKeys::AddKey(FKeyDetails(FDIWheelKeys::DIWheel_Button_Gear4, LOCTEXT("DIWheel_Button_Gear4", "DIWheel Button Gear4"), FKeyDetails::GamepadKey, NAME_DIWheel));
	EKeys::AddKey(FKeyDetails(FDIWheelKeys::DIWheel_Button_Gear5, LOCTEXT("DIWheel_Button_Gear5", "DIWheel Button Gear5"), FKeyDetails::GamepadKey, NAME_DIWheel));
	EKeys::AddKey(FKeyDetails(FDIWheelKeys::DIWheel_Button_Gear6, LOCTEXT("DIWheel_Button_Gear6", "DIWheel Button Gear6"), FKeyDetails::GamepadKey, NAME_DIWheel));
	EKeys::AddKey(FKeyDetails(FDIWheelKeys::DIWheel_Button_Gear7, LOCTEXT("DIWheel_Button_Gear7", "DIWheel Button Gear7"), FKeyDetails::GamepadKey, NAME_DIWheel));
}
