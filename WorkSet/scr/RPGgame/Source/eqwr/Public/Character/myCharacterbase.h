// why

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "NiagaraSystem.h"
#include "Interaction/CombatInterface.h"
#include "myCharacterbase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
/**
 *
 */
UCLASS(Abstract)
class EQWR_API AmyCharacterbase : public ACharacter,public IAbilitySystemInterface,public ICombatInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere,Category="Attributes")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere,Category="Attributes")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

public:
	// Sets default values for this character's properties
	AmyCharacterbase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const { return AttributSet; }

	void AddCharacterAbilities();

	
	//spell location

	UPROPERTY(EditAnywhere,Category="Combat")
	FName WeaponTipSocketName;

	/*ICombatInterface start*/
	//Implementation is Cpp come true ,BlueprintNativeEven has tow:blueprint and cpp
	virtual FVector GetCombatSocketLocation_Implementation()override;

	UPROPERTY(EditAnywhere,Category="Combat")
	TObjectPtr<UAnimMontage> HitMontage;

	virtual UAnimMontage* GetHitMontage_Implementation() override;

	virtual bool IsDead_Implementation() const override;

	virtual AActor* GetAvatar_Implementation() override;

	virtual FVector GetCombatSocketLocationByStruct_Implementation(const FTaggedMontage TaggedMontage)const  override;

	virtual TArray<FTaggedMontage> GetAttackMontage_Implementation() override;

	UPROPERTY(EditAnywhere,Category="Combat")
	TArray<FTaggedMontage> AttackMontage;

	UPROPERTY(EditAnywhere,Category="Combat")
	TObjectPtr<UNiagaraSystem> BloodNS;
	
	virtual FTaggedMontage GetMontageStructByTag_Implementation(const FGameplayTag& Tag) override;
	virtual UNiagaraSystem* GetBloodNS_Implementation() override;
	/*ICombatInterface over */
	
	//die
	UPROPERTY(editanywhere,BlueprintReadOnly,Category="Combat")
	TObjectPtr<USoundBase> DeathSound;
	
	virtual void Die() override;
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	UPROPERTY(editanywhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> CharacterDissolveMaterial;

	UPROPERTY(editanywhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterial;

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(const TArray<UMaterialInstanceDynamic*>& DynamicMaterialInstance);

protected:
	bool bDead = false;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;//中转用，可以选择不同的ge，而不是硬编码

	UPROPERTY(BlueprintReadOnly, EditAnywhere,category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
	

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass,float Level) const;

	virtual void InitializeDefaultAttributes() const ;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Combat");
	TObjectPtr<USkeletalMeshComponent> Weapon;
	//���END.
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributSet;

};
