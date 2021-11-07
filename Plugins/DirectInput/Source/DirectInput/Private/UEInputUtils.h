#pragma once

struct FMatchMappingByKey
{
	FMatchMappingByKey(const FKey InKey) : Key(InKey) {}
	bool operator() (const FInputActionKeyMapping& ActionMapping) { return ActionMapping.Key == Key; }
	bool operator() (const FInputAxisKeyMapping& AxisMapping) { return AxisMapping.Key == Key; }
	FKey Key;
};

inline FKey AddUniqKey(const FName& Name)
{
	FKey Key(Name);
	if (!Key.IsValid())
	{
		UE_LOG(LogDirectInput, Display, TEXT("AddKey: %s"), *Key.ToString());
		EKeys::AddKey(FKeyDetails(Key, FText::FromName(Key.GetFName()), FKeyDetails::GamepadKey));
	}
	return MoveTemp(Key);
}

inline bool AddUniqAxisMapping(UInputSettings* Settings, const FKey& Key, const FName& Name, float Scale = 1.0f)
{
	if (Settings->DoesAxisExist(Name) && !Settings->GetAxisMappings().FindByPredicate(FMatchMappingByKey(Key)))
	{
		UE_LOG(LogDirectInput, Display, TEXT("AddAxisMapping: %s -> %s"), *Key.ToString(), *Name.ToString());
		FInputAxisKeyMapping Mapping;
		Mapping.Key = Key;
		Mapping.AxisName = Name;
		Mapping.Scale = Scale;
		Settings->AddAxisMapping(Mapping);
		return true;
	}
	return false;
}

inline void RemoveAxisMapping(UInputSettings* Settings, const FKey& Key, const FName& Name)
{
	if (Settings->DoesAxisExist(Name) && Settings->GetAxisMappings().FindByPredicate(FMatchMappingByKey(Key)))
	{
		UE_LOG(LogDirectInput, Display, TEXT("RemoveAxisMapping: %s -> %s"), *Key.ToString(), *Name.ToString());
		FInputAxisKeyMapping Mapping;
		Mapping.Key = Key;
		Mapping.AxisName = Name;
		Settings->RemoveAxisMapping(Mapping);
	}
}

inline bool AddUniqActionMapping(UInputSettings* Settings, const FKey& Key, const FName& Name)
{
	if (Settings->DoesActionExist(Name) && !Settings->GetActionMappings().FindByPredicate(FMatchMappingByKey(Key)))
	{
		UE_LOG(LogDirectInput, Display, TEXT("AddActionMapping: %s -> %s"), *Key.ToString(), *Name.ToString());
		FInputActionKeyMapping Mapping;
		Mapping.Key = Key;
		Mapping.ActionName = Name;
		Settings->AddActionMapping(Mapping);
		return true;
	}
	return false;
}

inline void RemoveActionMapping(UInputSettings* Settings, const FKey& Key, const FName& Name)
{
	if (Settings->DoesActionExist(Name) && Settings->GetActionMappings().FindByPredicate(FMatchMappingByKey(Key)))
	{
		UE_LOG(LogDirectInput, Display, TEXT("RemoveActionMapping: %s -> %s"), *Key.ToString(), *Name.ToString());
		FInputActionKeyMapping Mapping;
		Mapping.Key = Key;
		Mapping.ActionName = Name;
		Settings->RemoveActionMapping(Mapping);
	}
}
