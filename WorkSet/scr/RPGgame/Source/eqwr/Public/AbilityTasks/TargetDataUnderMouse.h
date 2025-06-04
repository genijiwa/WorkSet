// why

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetDataUnderMouseDelegate, const FGameplayAbilityTargetDataHandle& , DataHandle);

/**
 * 
 */
UCLASS()
class EQWR_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,category="Ability|Tasks",meta=(Displayname="TargetDataUnderMouse",
		HidePin="OwningAbility",DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);// task attach to ability .

	UPROPERTY(BlueprintAssignable)
	FTargetDataUnderMouseDelegate ValidData;

private:
	virtual void Activate() override;

	void SendMouseCursorData();

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,FGameplayTag ActivationTag);
	
};
//yi bu class:task .