// why

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Maxhealth.generated.h"

/**
 * 
 */
UCLASS()
class EQWR_API UMMC_Maxhealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_Maxhealth();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition VigorDef;
};
