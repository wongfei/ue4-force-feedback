#pragma once

#include "WheeledVehicle.h"
#include "HapticVehicle.generated.h"

UCLASS(BlueprintType)
class HAPTICVEHICLE_API AHapticVehicle : public AWheeledVehicle
{
	GENERATED_UCLASS_BODY()

public:

	UFUNCTION(Category="Haptic", BlueprintCallable)
	class UHapticVehicleMovementComponent* GetHapticVehicleMovementComponent() const;
};
