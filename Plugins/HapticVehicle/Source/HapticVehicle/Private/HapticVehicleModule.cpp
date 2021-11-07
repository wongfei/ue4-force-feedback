#include "HapticVehicleModule.h"

class FHapticVehicleModule : public IHapticVehicleModule
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

void FHapticVehicleModule::StartupModule()
{
}

void FHapticVehicleModule::ShutdownModule()
{
}

IMPLEMENT_MODULE(FHapticVehicleModule, HapticVehicle)
