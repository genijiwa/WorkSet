// why


#include "AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	return NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);//atach to abiility get info. abilitycompomen = owingability.
}

void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocallyController = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyController)
	{
		SendMouseCursorData();
		//UE_LOG(LogTemp, Log, TEXT("Client ability: %p"),AbilitySystemComponent.Get());
	}
	//client server different logic
	else
	{
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(),GetActivationPredictionKey()).
		AddUObject(this,&UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet
		(GetAbilitySpecHandle(),GetActivationPredictionKey());

		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get(),true);//predict

	APlayerController* PS = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult HitResult;
	PS->GetHitResultUnderCursor(ECC_Visibility,false,HitResult);

	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();//one of datarule to the dele pams:handle
	Data->HitResult = HitResult;

	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(Data);

	//send data(mouse position)
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),GetActivationPredictionKey()
		,DataHandle,FGameplayTag(),AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
		//UE_LOG(LogTemp, Log, TEXT("client - SpecHandle: %s"), *GetAbilitySpecHandle().ToString());

	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(),GetActivationPredictionKey());
	
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
		//UE_LOG(LogTemp, Log, TEXT("Server - SpecHandle: %s"), *GetAbilitySpecHandle().ToString());

	}
}
