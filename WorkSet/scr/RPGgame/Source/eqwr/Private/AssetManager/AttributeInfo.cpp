// why


#include "AssetManager/AttributeInfo.h"

#include "Misc/LowLevelTestAdapter.h"

FAttributeInfo UDataAttributeInfo::FindAttributeInfoByTag(const FGameplayTag& AtrributeTag, bool bLogNoFound) const
{
	for (auto Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AtrributeTag))
		{
			return Info;
		}
		
	}
	
	if (bLogNoFound)
	{
		UE_LOG(LogTemp,Error,TEXT("in [%s],no [%s]"),*GetNameSafe(this),*AtrributeTag.ToString())
	}
	
	return FAttributeInfo();
}
