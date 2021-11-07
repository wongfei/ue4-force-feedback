#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DIFunctionLibrary.generated.h"

UCLASS()
class DIRECTINPUT_API UDIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="DirectInput")
	static void RequestUpdateConfiguration();

	UFUNCTION(BlueprintCallable, Category="DirectInput")
	static void SetForceFeedback(const FString& DeviceTag, float Constant, float Damper);
};
