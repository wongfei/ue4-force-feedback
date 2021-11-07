#pragma once

#include "Engine/Engine.h"
#include "DIDeviceInfo.h"
#include "DISettings.h"
#include <memory>

//#pragma pack (push,8)
#include "Windows/AllowWindowsPlatformTypes.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "Windows/HideWindowsPlatformTypes.h"
//#pragma pack (pop)

#define DI_NOP()

#define DI_GUARD_HR(hr, msg, action)\
	if (FAILED((hr))) {\
		UE_LOG(LogDirectInput, Error, TEXT("%s: hr=0x%X"), (msg), (hr));\
		action;\
	}

#define DI_MAKE_PROP(type, name)\
	type name;\
	FMemory::Memset(&name, 0, sizeof(name));\
	name.diph.dwSize = sizeof(type);\
	name.diph.dwHeaderSize = sizeof(DIPROPHEADER)

struct ComDtor
{
	void operator()(IUnknown* obj) const { obj->Release(); }
};

inline FString GuidToString(const GUID& guid)
{
	const int len = 256;
	TCHAR str[len]; str[0] = 0;
	StringFromGUID2(guid, str, len);
	return FString(str);
}

inline FString GenerateDeviceUid(const FDIDeviceInfo& Info)
{
	return Info.InstanceName;
	//return Info.InstanceName + TEXT(" ") + Info.ProductGuid;
}

inline FDIDeviceInfo GetDeviceInfo(const DIDEVICEINSTANCE& In)
{
	FDIDeviceInfo Info;
	Info.InstanceGuid = GuidToString(In.guidInstance);
	Info.InstanceName = In.tszInstanceName;
	Info.ProductGuid = GuidToString(In.guidProduct);
	Info.ProductName = In.tszProductName;
	Info.Uid = GenerateDeviceUid(Info);
	Info.Type = (int)GET_DIDEVICE_TYPE(In.dwDevType);
	Info.SubType = (int)GET_DIDEVICE_SUBTYPE(In.dwDevType);
	return MoveTemp(Info);
}

inline FDIDeviceInfo* FindByUid(TArray<FDIDeviceInfo>& Devices, const FString& Uid)
{
	for (auto& Dev : Devices)
	{
		if (Dev.Uid == Uid)
			return &Dev;
	}
	return nullptr;
}
