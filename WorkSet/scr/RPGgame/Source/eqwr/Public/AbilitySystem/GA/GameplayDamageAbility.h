// why

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GA/GameplayAbilityBase.h"
#include "Interaction/CombatInterface.h"
#include "GameplayDamageAbility.generated.h"

/**
 * 
 */
UCLASS()
class EQWR_API UGameplayDamageAbility : public UGameplayAbilityBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,category="Damage")
	FScalableFloat DamageScalable;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageClass;

	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
	static FTaggedMontage GetRamdonTaggedMontageFromArray(const TArray<FTaggedMontage>& TagMontageArray);
};
