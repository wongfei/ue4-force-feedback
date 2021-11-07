#pragma once

#include "Components/SceneComponent.h"
#include "DIDeviceComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIRECTINPUT_API UDIDeviceComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()
	
public:

	// begin UActorComponent
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void UninitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// end UActorComponent
	
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

	UPROPERTY(BlueprintReadOnly)
	class UDIDeviceHandle* Device = nullptr;
};
