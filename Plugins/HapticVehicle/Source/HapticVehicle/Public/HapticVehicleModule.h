#pragma once

#include "Modules/ModuleManager.h"

class HAPTICVEHICLE_API IHapticVehicleModule : public IModuleInterface
{
public:

	static inline IHapticVehicleModule& Get()
	{
		return FModuleManager::LoadModuleChecked<IHapticVehicleModule>("HapticVehicle");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("HapticVehicle");
	}
};
