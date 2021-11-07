#include "DISettings.h"
#include "DIFunctionLibrary.h"

FName UDISettings::GetCategoryName() const
{
	return TEXT("Plugins");
}

#if WITH_EDITOR
FText UDISettings::GetSectionText() const
{
	return FText::FromString(TEXT("Direct Input"));
}

void UDISettings::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	UDIFunctionLibrary::RequestUpdateConfiguration();
}
#endif

bool UDISettings::FindByUid(const FString& Uid, FDIDeviceConfiguration& Config) const
{
	for (const auto& Iter : DeviceConfigurations)
	{
		if (Iter.Uid == Uid)
		{ 
			Config = Iter; 
			return true;
		}
	}
	return false;
}
