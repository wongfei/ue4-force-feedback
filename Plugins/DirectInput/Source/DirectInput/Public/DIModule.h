#pragma once

#include "Modules/ModuleManager.h"
#include "IInputDeviceModule.h"
#include "IInputDevice.h"

class DIRECTINPUT_API IDirectInputModule : public IInputDeviceModule
{
public:

	static inline IDirectInputModule& Get()
	{
		return FModuleManager::LoadModuleChecked<IDirectInputModule>("DirectInput");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("DirectInput");
	}

	virtual IInputDevice* GetDevicePtr() = 0;
};
