#include "HapticVehicleMovementComponent.h"

UHapticVehicleMovementComponent::UHapticVehicleMovementComponent(const FObjectInitializer& ObjectInitializer)
:
	Super(ObjectInitializer)
{
	HapticTires.SetNum(4);
}

void UHapticVehicleMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float LatForceN = (HapticTires[0].LatForce + HapticTires[1].LatForce) * 0.01f;
	FFConstant = LatForceN / FMath::Max(1.0f, FFLateralForceMax);

	float SpeedKmh = GetForwardSpeed() * 0.036f;
	FFDamper = 1.0f - FMath::Clamp(SpeedKmh, 0.0f, 10.0f) * 0.1f; // damper enabled when speed>0 and speed<10 km/h
}

void UHapticVehicleMovementComponent::GenerateTireForces(class UVehicleWheel* Wheel, const FTireShaderInput& Input, FTireShaderOutput& Output)
{
	Super::GenerateTireForces(Wheel, Input, Output);

	auto& State = HapticTires[Wheel->WheelIndex];

	State.TireFriction = Input.TireFriction;
	State.LongSlip = Input.LongSlip;
	State.LatSlip = Input.LatSlip;
	State.WheelOmega = Input.WheelOmega;
	State.WheelRadius = Input.WheelRadius;
	State.RestTireLoad = Input.RestTireLoad;
	State.TireLoad = Input.TireLoad;
	State.NormalizedTireLoad = Input.NormalizedTireLoad;
	State.Gravity = Input.Gravity;

	State.WheelTorque = Output.WheelTorque;
	State.LongForce = Output.LongForce;
	State.LatForce = Output.LatForce;
}
