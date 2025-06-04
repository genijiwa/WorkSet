// why

#pragma once

#include "CoreMinimal.h"
#include "myCharacterbase.h"
#include "AI/RPGAIController.h"
#include "AssetManager/CharacterClassInfo.h"
#include "Components/WidgetComponent.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Interaction/EnemyInterface.h"
#include "enemy.generated.h"


class UBehaviorTree;
enum class ECharacterClass : uint8;
/**
 * 
 */
UCLASS()
class EQWR_API Aenemy : public AmyCharacterbase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	//EnemyInterface
	virtual void Hightlight() override;
	virtual void UnHightlight() override;
	
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() override;
	UPROPERTY(BlueprintReadWrite,Category = "Combat")
	TObjectPtr<AActor> CombatTarget;
	//.
	
	Aenemy();

	virtual int32 GetPlayerLevel() override;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Character Class Defaults");
	int32 Level;

	//Health ui
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Widget");
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature  OnMaxHealthChanged;

	UPROPERTY(EditAnywhere,category="Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(BlueprintReadOnly,Category="Combat");
	bool bIsHiting = false;

	UPROPERTY(BlueprintReadOnly,Category="Combat")
	float BaseSpeed = 250;

	//die
	UPROPERTY(Editanywhere,BlueprintReadOnly,Category="Combat")
	float LifeSpan = 5.f;

	virtual void Die() override;

	//AI Controoler
	UPROPERTY(Editanywhere,Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ARPGAIController> AIController;

	virtual void PossessedBy(AController* NewController) override;

	
	
protected:
	virtual void BeginPlay() override;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
