#include "DIFunctionLibrary.h"
#include "DIModule.h"
#include "DIEngineDevice.h"

inline FDIEngineDevice* GetInputDevice()
{
	return ((FDIEngineDevice*)IDirectInputModule::Get().GetDevicePtr());
}

void UDIFunctionLibrary::RequestUpdateConfiguration()
{
	GetInputDevice()->RequestUpdateConfiguration();
}

void UDIFunctionLibrary::SetForceFeedback(const FString& DeviceTag, float Constant, float Damper)
{
	GetInputDevice()->SetForceFeedback(DeviceTag, Constant, Damper);
}
