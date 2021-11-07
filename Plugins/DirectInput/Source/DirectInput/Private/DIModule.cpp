#include "DIModule.h"
#include "DIEngineDevice.h"
#include "DIKeys.h"
#include "DIWheelKeys.h"

class FDIModule : public IDirectInputModule
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual TSharedPtr<IInputDevice> CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) override;
	virtual IInputDevice* GetDevicePtr() override { return Device.Get(); }

protected:

	TSharedPtr<FDIEngineDevice> Device;
};

void FDIModule::StartupModule()
{
	FDIKeys::RegisterKeys();
	FDIWheelKeys::RegisterKeys();

	IInputDeviceModule::StartupModule();
}

void FDIModule::ShutdownModule()
{
	IInputDeviceModule::ShutdownModule();
}

TSharedPtr<class IInputDevice> FDIModule::CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
	Device = MakeShareable(new FDIEngineDevice(InMessageHandler));
	return Device;
}

IMPLEMENT_MODULE(FDIModule, DirectInput)
