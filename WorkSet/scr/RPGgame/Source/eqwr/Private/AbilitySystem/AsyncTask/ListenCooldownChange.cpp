// why


#include "AbilitySystem/AsyncTask/ListenCooldownChange.h"

UListenCooldownChange* UListenCooldownChange::ListenCooldownChange(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayTag& InCooldownTag)
{
	UListenCooldownChange* ListenCooldownChange = NewObject<UListenCooldownChange>();
	
	ListenCooldownChange->CooldownTag = InCooldownTag;
	ListenCooldownChange->ASC = AbilitySystemComponent;

	AbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag,EGameplayTagEventType::NewOrRemoved).AddLambda(
		[ListenCooldownChange](const FGameplayTag& InTag,int32 NewCount)
		{
			if (NewCount == 0)
			{
				ListenCooldownChange->CooldownEnd.Broadcast(0.f);
			}
		});

	//ListenCooldownChange is shili not This....
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(ListenCooldownChange,&UListenCooldownChange::OnActiveEffectAdded);
	
	return ListenCooldownChange;
}

void UListenCooldownChange::EndTask()
{
	if (!IsValid(ASC)) return;

	ASC->RegisterGameplayTagEvent(CooldownTag,EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UListenCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC,
                                                const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)const
{

	FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(CooldownTag.GetSingleTagContainer());

	TArray<float> TimeRemaining = ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);

	if (TimeRemaining.Num() > 0)
	{
		float CurrentTime = TimeRemaining[0];
		for (int32 i = 0; i < TimeRemaining.Num(); i++)
		{
			if (TimeRemaining[i] > CurrentTime) CurrentTime = TimeRemaining[i];
		}
		CooldownStart.Broadcast(CurrentTime);
	}
}


