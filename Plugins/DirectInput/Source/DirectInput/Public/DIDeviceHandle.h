#pragma once

#include "UObject/Object.h"
#include "DIDeviceInfo.h"
#include "DIDeviceHandle.generated.h"

UCLASS(BlueprintType)
class DIRECTINPUT_API UDIDeviceHandle : public UObject
{
	GENERATED_UCLASS_BODY()
	
public:

	// begin UObject
	void BeginDestroy() override;
	// end UObject

	UFUNCTION(BlueprintCallable)
	static TArray<FDIDeviceInfo> EnumerateDIDevices();

	UFUNCTION(BlueprintCallable)
	static UDIDeviceHandle* CreateDIDevice();

	UFUNCTION(BlueprintCallable)
	virtual bool Open(const FString& Guid);
	
	UFUNCTION(BlueprintCallable)
	virtual void Close();

	UFUNCTION(BlueprintCallable)
	virtual void Tick();

	UFUNCTION(BlueprintCallable)
	virtual float GetAxis(int Id);

	UFUNCTION(BlueprintCallable)
	virtual float GetPov(int Id);

	UFUNCTION(BlueprintCallable)
	virtual bool GetButton(int Id);

	UFUNCTION(BlueprintCallable)
	virtual void EnableFeedback(bool Enabled);

	UFUNCTION(BlueprintCallable)
	virtual void SetFeedback(float Constant, float Damper);

private:

	struct DIDeviceImpl* Impl = nullptr;
};
