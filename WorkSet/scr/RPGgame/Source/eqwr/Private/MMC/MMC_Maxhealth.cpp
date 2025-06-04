// why


#include "MMC/MMC_Maxhealth.h"

#include "AbilitySystem/MyAttributeSetBase.h"
#include "Interaction/CombatInterface.h"

UMMC_Maxhealth::UMMC_Maxhealth()
{
	VigorDef.AttributeToCapture = UMyAttributeSetBase::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_Maxhealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluateParams;

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	EvaluateParams.SourceTags = SourceTags;;
	EvaluateParams.TargetTags = TargetTags;;
	
	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef,Spec,EvaluateParams,Vigor);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetEffectCauser());
	const int32 Level = CombatInterface->GetPlayerLevel();
	
	return Vigor * 2.5f + 80.f +Level * 10.f;
}
