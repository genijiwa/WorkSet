// why

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/MyAttributeSetBase.h"
#include "PlayerStateBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged,int32);

UCLASS()
class EQWR_API APlayerStateBase : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	APlayerStateBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	virtual  void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	FORCEINLINE int32 GetPlayerLevel() const {return Level;};
	FORCEINLINE int32 GetPlayerXp() const {return XP;};

	FOnPlayerStateChanged OnPlayerLevelChanged;
	FOnPlayerStateChanged OnPlayerXPChanged;

	void AddPlayerLevel(int32 InLevel){Level += InLevel; OnPlayerLevelChanged.Broadcast(Level);};
	void AddPlayerXP(int32 InXP){XP += InXP; OnPlayerXPChanged.Broadcast(InXP);UE_LOG(LogTemp, Warning, TEXT("%d 的InXP发生了修改，当前InXp：%d"), XP, GetPlayerXp());;};

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
private:
	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_Level);
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_XP);
	int32 XP = 0 ;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel)const;

	UFUNCTION()
	void OnRep_XP(int32 OldXP)const	;
	
};
