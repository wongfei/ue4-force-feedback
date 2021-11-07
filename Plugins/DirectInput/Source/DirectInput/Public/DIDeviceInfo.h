#pragma once

#include "CoreMinimal.h"
#include "DIDeviceInfo.generated.h"

USTRUCT(BlueprintType)
struct DIRECTINPUT_API FDIDeviceInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString Uid;

	UPROPERTY(BlueprintReadOnly)
	FString InstanceGuid;

	UPROPERTY(BlueprintReadOnly)
	FString InstanceName;

	UPROPERTY(BlueprintReadOnly)
	FString ProductGuid;

	UPROPERTY(BlueprintReadOnly)
	FString ProductName;

	UPROPERTY(BlueprintReadOnly)
	int Type;

	UPROPERTY(BlueprintReadOnly)
	int SubType;
};
