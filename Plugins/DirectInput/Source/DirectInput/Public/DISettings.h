#pragma once

#include "Engine/DeveloperSettings.h"
#include "InputCoreTypes.h"
#include "DISettings.generated.h"

USTRUCT()
struct DIRECTINPUT_API FDIDeviceAxisProperties
{
	GENERATED_BODY()

	FDIDeviceAxisProperties() {}
	FDIDeviceAxisProperties(int InAxisId, FKey InRemapKey, bool InPedal) : AxisId(InAxisId), RemapKey(InRemapKey), IsPedal(InPedal) {}

	/** Unique axis identifier: "DeviceName Axis #ID" */
	FName Uid;

	/** Hardware axis ID */
	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(ClampMin="0", ClampMax="7", UIMin="0", UIMax="7"))
	int AxisId = 0;
	
	/** Remap hardware events to specific input key */
	UPROPERTY(EditAnywhere, Category="Axis Properties")
	FKey RemapKey;

	/** Remap hardware events to specific input axis */
	UPROPERTY(EditAnywhere, Category="Axis Properties")
	FName RemapAxis;

	/** Scale axis */
	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(ClampMin="0.0", ClampMax="1.0", UIMin="0.0", UIMax="1.0"))
	float Scale = 1.0f;

	/** Invert axis */
	UPROPERTY(EditAnywhere, Category="Axis Properties")
	bool Invert = false;

	/** Normalize axis values to range [0..1] */
	UPROPERTY(EditAnywhere, Category="Axis Properties")
	bool IsPedal = false;
};

USTRUCT()
struct DIRECTINPUT_API FDIDeviceButtonProperties
{
	GENERATED_BODY()

	FDIDeviceButtonProperties() {}
	FDIDeviceButtonProperties(int InButtonId, FKey InRemapKey) : ButtonId(InButtonId), RemapKey(InRemapKey) {}

	/** Unique button identifier: "DeviceName Button #ID" */
	FName Uid;

	/** Hardware axis ID */
	UPROPERTY(EditAnywhere, Category="Button Properties", meta=(ClampMin="0", ClampMax="31", UIMin="0", UIMax="31"))
	int ButtonId = 0;
	
	/** Remap hardware events to specific input key */
	UPROPERTY(EditAnywhere, Category="Button Properties")
	FKey RemapKey;

	/** Remap hardware events to specific input axis */
	UPROPERTY(EditAnywhere, Category="Button Properties")
	FName RemapAction;
};

USTRUCT()
struct DIRECTINPUT_API FDIDeviceFeedbackProperties
{
	GENERATED_BODY()

	/** ForceFeedback device tag (used with SetForceFeedback) */
	UPROPERTY(EditAnywhere, Category="Feedback Properties")
	FString Tag;

	UPROPERTY(EditAnywhere, Category="Feedback Properties")
	bool Enabled = false;

	UPROPERTY(EditAnywhere, Category="Feedback Properties", meta=(ClampMin="0.0", ClampMax="2.0", UIMin="0.0", UIMax="2.0"))
	float ConstantGain = 1.0f;

	UPROPERTY(EditAnywhere, Category="Feedback Properties", meta=(ClampMin="0.0", ClampMax="2.0", UIMin="0.0", UIMax="2.0"))
	float DamperGain = 1.0f;
};

USTRUCT()
struct DIRECTINPUT_API FDIDeviceConfiguration
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category="Device Config")
	FString Uid;

	UPROPERTY(EditAnywhere, Category="Device Config")
	bool Enabled = true;

	UPROPERTY(EditAnywhere, Category="Device Config")
	TArray<FDIDeviceAxisProperties> AxisProperties;

	UPROPERTY(EditAnywhere, Category="Device Config")
	TArray<FDIDeviceButtonProperties> ButtonProperties;

	UPROPERTY(EditAnywhere, Category="Device Config")
	FDIDeviceFeedbackProperties FeedbackProperties;
};

UCLASS(config=Input, defaultconfig)
class DIRECTINPUT_API UDISettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

#if WITH_EDITOR
	// Begin UObject Interface
	virtual void PostEditChangeChainProperty( struct FPropertyChangedChainEvent& PropertyChangedEvent ) override;
	// End UObject Interface
#endif

	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override;
#if WITH_EDITOR
	virtual FText GetSectionText() const override;
#endif
	// End UDeveloperSettings Interface

	bool FindByUid(const FString& Uid, FDIDeviceConfiguration& Config) const;

	UPROPERTY(config, EditAnywhere, Category="Device Configurations")
	TArray<FDIDeviceConfiguration> DeviceConfigurations;
};
