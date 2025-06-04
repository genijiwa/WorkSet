// why


#include "AssetManager/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetEffectByClass(ECharacterClass CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}

int32 UCharacterClassInfo::GetRewardXPByClass(ECharacterClass CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass).RewardXP;
}
