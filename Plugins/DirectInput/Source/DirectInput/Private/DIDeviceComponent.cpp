#include "DIDeviceComponent.h"
#include "DIDeviceHandle.h"

UDIDeviceComponent::UDIDeviceComponent(const FObjectInitializer& ObjectInitializer)
:
	Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDIDeviceComponent::InitializeComponent()
{
	Super::InitializeComponent();
	Device = NewObject<UDIDeviceHandle>();
}

void UDIDeviceComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDIDeviceComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Device->Close();
	Super::EndPlay(EndPlayReason);
}

void UDIDeviceComponent::UninitializeComponent()
{
	Device->Close();
	Super::UninitializeComponent();
}

void UDIDeviceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Device->Tick();
}

float UDIDeviceComponent::GetAxis(int Id)
{
	return Device->GetAxis(Id);
}

float UDIDeviceComponent::GetPov(int Id)
{
	return Device->GetPov(Id);
}

bool UDIDeviceComponent::GetButton(int Id)
{
	return Device->GetButton(Id);
}

void UDIDeviceComponent::EnableFeedback(bool Enabled)
{
	Device->EnableFeedback(Enabled);
}

void UDIDeviceComponent::SetFeedback(float Constant, float Damper)
{
	Device->SetFeedback(Constant, Damper);
}
