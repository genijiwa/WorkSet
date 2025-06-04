// why


#include "AbilitySystem/GA/GameplayDamageAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/MyAbilitySystemBlueprintLibrary.h"
#include "AssetManager/MyGameplayTags.h"

void UGameplayDamageAbility::CauseDamage(AActor* TargetActor)
{

	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageClass);

	const float ScaleDamage = DamageScalable.GetValueAtLevel(GetAbilityLevel());/*ICombatInterface:GetplayerLeve is Character Level*/
	UMyAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle,FMyGameplayTags::Get().GA_Attack,ScaleDamage);
	
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(),
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FTaggedMontage UGameplayDamageAbility::GetRamdonTaggedMontageFromArray(const TArray<FTaggedMontage>& TagMontageArray)
{
	if (TagMontageArray.Num() > 0)
	{	
		FTaggedMontage TagMontage = TagMontageArray[FMath::RandRange(0,TagMontageArray.Num() - 1)];
		return TagMontage;
	}
	return FTaggedMontage();
}
