#include "DIDeviceHandle.h"
#include "DIDeviceImpl.h"

// static
TArray<FDIDeviceInfo> UDIDeviceHandle::EnumerateDIDevices()
{
	return DIDeviceImpl::Enumerate();
}

// static
UDIDeviceHandle* UDIDeviceHandle::CreateDIDevice()
{
	return NewObject<UDIDeviceHandle>();
}

UDIDeviceHandle::UDIDeviceHandle(const FObjectInitializer& ObjectInitializer)
:
	Super(ObjectInitializer)
{
	Impl = new DIDeviceImpl();
}

void UDIDeviceHandle::BeginDestroy()
{
	delete Impl;
	Super::BeginDestroy();
}

bool UDIDeviceHandle::Open(const FString& Guid)
{
	return Impl->Open(Guid);
}

void UDIDeviceHandle::Close()
{
	Impl->Close();
}

void UDIDeviceHandle::Tick()
{
	Impl->Tick();
}

float UDIDeviceHandle::GetAxis(int Id)
{
	return Impl->GetAxis(Id);
}

float UDIDeviceHandle::GetPov(int Id)
{
	return Impl->GetPov(Id);
}

bool UDIDeviceHandle::GetButton(int Id)
{
	return Impl->GetButton(Id);
}

void UDIDeviceHandle::EnableFeedback(bool Enabled)
{
	Impl->EnableFeedback(Enabled);
}

void UDIDeviceHandle::SetFeedback(float Constant, float Damper)
{
	Impl->SetFeedback(Constant, Damper);
}
