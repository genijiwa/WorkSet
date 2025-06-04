// why

#pragma once

#include "CoreMinimal.h"
#include "GameplayDamageAbility.h"
#include "Actor/Projectile.h"
#include "ProjectileSpell.generated.h"


/**
 * 
 */
UCLASS()
class EQWR_API UProjectileSpell : public UGameplayDamageAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable,Category="Projectile")
	void SpawnProjectile(const FVector& TargetLocation);

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AProjectile> ProjectileClass;


private:
	
};
