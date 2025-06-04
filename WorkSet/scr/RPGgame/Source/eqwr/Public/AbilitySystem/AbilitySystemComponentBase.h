// why

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemComponentBase.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags,const FGameplayTagContainer& );
DECLARE_MULTICAST_DELEGATE_OneParam(FACSForGA,const UAbilitySystemComponent* );

/**
 * 
 */
UCLASS()
class EQWR_API UAbilitySystemComponentBase : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;

	FACSForGA ACSForGA;

	void AddGameplayAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities);


	UFUNCTION(Client,reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
		const FGameplayEffectSpec& EffectSpec,FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const;


	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void AbilityInputTagHold(const FGameplayTag& InputTag);

	virtual void OnRep_ActivateAbilities() override;

};

inline void UAbilitySystemComponentBase::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	ACSForGA.Broadcast(this);
}
