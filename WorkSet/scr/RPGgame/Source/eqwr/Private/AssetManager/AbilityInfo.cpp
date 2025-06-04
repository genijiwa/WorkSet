// why


#include "AssetManager/AbilityInfo.h"

FStrAbilityInfo UAbilityInfo::GetAbilityInfoByTag(const FGameplayTag AbilityTag)
{
	for (const FStrAbilityInfo AbilityInfo : AbilityInfos)
	{
		if (AbilityInfo.AbilityTag.MatchesTagExact(AbilityTag)) return AbilityInfo;
	}
	return FStrAbilityInfo();
}
