// why


#include "AbilitySystem/GA/ProjectileSpell.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/MyAbilitySystemBlueprintLibrary.h"
#include "Actor/Projectile.h"
#include "AssetManager/MyGameplayTags.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LevelInstance/LevelInstanceTypes.h"

void UProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UProjectileSpell::SpawnProjectile(const FVector& TargetLocation)
{
	const bool bIsSever = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsSever){return;};
	
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		FTransform SpawnTransform;

		FRotator Rotation = (TargetLocation - ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo())).Rotation();
		//Rotation.Pitch = -10.f;

		FVector LocationS = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo());
		
		SpawnTransform.SetLocation(LocationS);
		SpawnTransform.SetRotation(Rotation.Quaternion());
		
		AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(ProjectileClass,SpawnTransform,GetOwningActorFromActorInfo(),
			Cast<APawn>(GetAvatarActorFromActorInfo()),ESpawnActorCollisionHandlingMethod::AlwaysSpawn);//deferred must finishspawning.

		const UAbilitySystemComponent* SourceACS =UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectContextHandle EffectContextHandle = SourceACS->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = SourceACS->MakeOutgoingSpec(DamageClass,GetAbilityLevel(),EffectContextHandle);

		//Set by caller Damage
		const float ScalarDamage = DamageScalable.GetValueAtLevel(GetAbilityLevel());
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("火球术伤害：%f"), ScalarDamage));
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,FMyGameplayTags::Get().Damage,ScalarDamage);
		
		Projectile->DamageEffectHandle = SpecHandle;

		Projectile->FinishSpawning(SpawnTransform);
		
		//UKismetSystemLibrary::PrintString(this, FString("在c++中打印数据"), true, true, FLinearColor::Blue, 3);
	}
}
