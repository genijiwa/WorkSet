// why

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GA/GameplayAbilityBase.h"
#include "SummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class EQWR_API USummonAbility : public UGameplayAbilityBase
{
	GENERATED_BODY()


public:
	
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	int32 Num = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MaxDistance = 250.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MinDistance = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float SpawnSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	TArray<TSubclassOf<APawn>> SpawnClass;

	UFUNCTION(blueprintCallable)
	TSubclassOf<APawn> RandomGetClass();

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocation() ;
};
