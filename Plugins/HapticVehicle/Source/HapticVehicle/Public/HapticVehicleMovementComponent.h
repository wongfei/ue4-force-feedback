#pragma once

#include "WheeledVehicleMovementComponent4W.h"
#include "HapticVehicleMovementComponent.generated.h"

USTRUCT(BlueprintType)
struct HAPTICVEHICLE_API FHapticTire
{
	GENERATED_BODY()

	// INPUTS

	// Friction value of the tire contact.
	UPROPERTY(BlueprintReadOnly)
	float TireFriction;

	// Longitudinal slip of the tire
	UPROPERTY(BlueprintReadOnly)
	float LongSlip;

	// Lateral slip of the tire.
	UPROPERTY(BlueprintReadOnly)
	float LatSlip;

	// Rotational speed of the wheel, in radians.
	UPROPERTY(BlueprintReadOnly)
	float WheelOmega;

	// The distance from the tire surface to the center of the wheel.
	UPROPERTY(BlueprintReadOnly)
	float WheelRadius;

	// How much force (weight) is pushing on the tire when the vehicle is at rest.
	UPROPERTY(BlueprintReadOnly)
	float RestTireLoad;

	// How much force (weight) is pushing on the tire right now.
	UPROPERTY(BlueprintReadOnly)
	float TireLoad;

	// RestTireLoad / TireLoad
	UPROPERTY(BlueprintReadOnly)
	float NormalizedTireLoad;

	// Acceleration due to gravity
	UPROPERTY(BlueprintReadOnly)
	float Gravity;

	// OUTPUTS

	// The torque to be applied to the wheel around the wheel axle. Opposes the engine torque on the wheel
	UPROPERTY(BlueprintReadOnly)
	float WheelTorque;

	// The magnitude of the longitudinal tire force to be applied to the vehicle's rigid body.
	UPROPERTY(BlueprintReadOnly)
	float LongForce;

	// The magnitude of the lateral tire force to be applied to the vehicle's rigid body.
	UPROPERTY(BlueprintReadOnly)
	float LatForce;
};

UCLASS(BlueprintType)
class HAPTICVEHICLE_API UHapticVehicleMovementComponent : public UWheeledVehicleMovementComponent4W
{
	GENERATED_UCLASS_BODY()

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GenerateTireForces(class UVehicleWheel* Wheel, const FTireShaderInput& Input, FTireShaderOutput& Output) override;

	// Runtime

	UPROPERTY(Category="Haptic", BlueprintReadOnly)
	TArray<FHapticTire> HapticTires;

	// FF

	UPROPERTY(Category="Haptic", BlueprintReadWrite, EditAnywhere, meta=(ClampMin="1.0", ClampMax="50000.0", UIMin="1.0", UIMax="50000.0"), Config)
	float FFLateralForceMax = 15000.0f;

	UPROPERTY(Category="Haptic", BlueprintReadOnly)
	float FFConstant = 0;

	UPROPERTY(Category="Haptic", BlueprintReadOnly)
	float FFDamper = 0;
};
