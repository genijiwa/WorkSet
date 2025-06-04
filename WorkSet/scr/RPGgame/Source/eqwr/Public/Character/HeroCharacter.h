// why

#pragma once

#include "CoreMinimal.h"
#include "myCharacterbase.h"
#include "HeroCharacter.generated.h"

/**
 * 
 */
UCLASS()
class EQWR_API AHeroCharacter : public AmyCharacterbase
{
	GENERATED_BODY()
private:
	void InitAbilityActorInfo();
public:
	AHeroCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual int32 GetPlayerLevel() override;
};
