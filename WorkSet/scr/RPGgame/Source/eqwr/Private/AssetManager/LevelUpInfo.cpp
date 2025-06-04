// why


#include "AssetManager/LevelUpInfo.h"

int32 ULevelUpInfo::GetLevelByXP(int32 XP)
{
	int32 CurLevel = 1;
	for (FStrLevelUpInfo Info : LevelUpInfo)
	{
		if (XP > Info.NeedXP) CurLevel++;
	}
	return CurLevel;
}
