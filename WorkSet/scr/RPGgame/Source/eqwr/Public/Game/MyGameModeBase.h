// why

#pragma once

#include "CoreMinimal.h"
#include "AssetManager/CharacterClassInfo.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class EQWR_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,Category="Character Class Default")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
	
};
