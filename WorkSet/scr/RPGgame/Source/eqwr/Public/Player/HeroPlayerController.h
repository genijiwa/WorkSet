// why

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AbilitySystemComponentBase.h"
#include "GameFramework/PlayerController.h"
#include "Input/InputConfig.h"
#include "Interaction/EnemyInterface.h"
#include "UI/Widget/DamageWidgetComponent.h"
#include "HeroPlayerController.generated.h"

class USplineComponent;
/**
 * 
 */
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
UCLASS()
class EQWR_API AHeroPlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext> CharacterContext; 
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;//���������ʲ������ô���Ĭ�ϣ�����ֱ����ʾ��editor��ʱ����ѡ��

	void Move(const FInputActionValue& InputAction);

	void CursorTrace();
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;

	UPROPERTY()
	UAbilitySystemComponentBase* AbilitySystemComponent;
	
	//auto Run
	FVector CachedDirection = FVector::ZeroVector;
	float FollowTime = 0.f;
	bool bAutoRun = false;
	bool bTarget = false;
	
	UPROPERTY(EditDefaultsOnly)
	float ShortPresstime;
	UPROPERTY(EditDefaultsOnly)
	float AutoRunRadius;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();

	FHitResult CursorHit;
	
public:
	AHeroPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	UAbilitySystemComponentBase* GetASC();

	void OnPresse(FGameplayTag InputTag);
	void OnRelease(FGameplayTag InputTag);
	void OnHold(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputConfig> InputConfig;

	//Damage Text , for player so playercontroller
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageWidgetComponent> DamageWidgetComponentClass;

	UFUNCTION(Client,Reliable)
	void ShowDamageText(float DamageNum , ACharacter* TargetCharacter,bool bIsBlockedHit , bool bIsCriticalHit);
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	
};

