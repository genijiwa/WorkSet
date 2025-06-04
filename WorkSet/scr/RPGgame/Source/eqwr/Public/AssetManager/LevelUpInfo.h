// why

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FStrLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 NeedXP = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointReward = 1;

	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointReward = 1;
};
UCLASS()
class EQWR_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FStrLevelUpInfo> LevelUpInfo;

	UFUNCTION()
	int32 GetLevelByXP(int32 XP);
	
};
