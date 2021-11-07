#include "HapticVehicle.h"
#include "HapticVehicleMovementComponent.h"

AHapticVehicle::AHapticVehicle(const FObjectInitializer& ObjectInitializer)
:
	Super(ObjectInitializer.SetDefaultSubobjectClass<UHapticVehicleMovementComponent>(VehicleMovementComponentName))
{
}

UHapticVehicleMovementComponent* AHapticVehicle::GetHapticVehicleMovementComponent() const
{
	return Cast<UHapticVehicleMovementComponent>(GetVehicleMovementComponent());
}
