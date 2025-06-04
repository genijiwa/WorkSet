// why

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class EQWR_API UDamageWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float DamageNum,bool bIsBlockedHit , bool bIsCriticalHit);
};
