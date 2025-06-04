// why


#include "Player/PlayerStateBase.h"
#include <AbilitySystem/AbilitySystemComponentBase.h>
#include "Net/UnrealNetwork.h"



APlayerStateBase::APlayerStateBase()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponentBase>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UMyAttributeSetBase>("AttributeSet");

}

UAbilitySystemComponent* APlayerStateBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APlayerStateBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerStateBase, Level);
	DOREPLIFETIME(APlayerStateBase, XP);

}

void APlayerStateBase::OnRep_Level(int32 OldLevel)const
{
	OnPlayerLevelChanged.Broadcast(Level);
}

void APlayerStateBase::OnRep_XP(int32 OldXP)const
{
	OnPlayerXPChanged.Broadcast(XP);
}
