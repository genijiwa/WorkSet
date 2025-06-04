// why

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecuCalc_Damage.generated.h"

/**
 * 
 */
UCLASS()
class EQWR_API UExecuCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()


public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	UExecuCalc_Damage();
};
