// why


#include "ExecuCalc/ExecuCalc_Damage.h"

#include "AbilitySystem/MyAbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/MyAttributeSetBase.h"
#include "AssetManager/MyGameplayTags.h"


struct SDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);

	SDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSetBase,CriticalHitChance,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSetBase,BlockChance,Target,false);
	}
};

static const SDamageStatics& DamageStatics()
{
	static SDamageStatics DamageStatics;
	return DamageStatics;
} 

UExecuCalc_Damage::UExecuCalc_Damage()
{	
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
}


void UExecuCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetEffectContext();
	
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Damage = Spec.GetSetByCallerMagnitude(FMyGameplayTags::Get().Damage);
	//BlockChance
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef,EvaluationParameters,TargetBlockChance);//get debuff
	TargetBlockChance = FMath::Clamp(0.f,TargetBlockChance,0.f);
	//set
	const bool bIsBlockChance = FMath::RandRange(1,100) < TargetBlockChance;
	if (bIsBlockChance){Damage = 0;}

	//CritialHitChance
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef,EvaluationParameters,SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Clamp(0.f,SourceCriticalHitChance,0.f);

	const bool bIsCriticalHit = FMath::RandRange(1,100) < SourceCriticalHitChance;
	if (bIsCriticalHit){Damage = Damage * 1.5;}

	const FGameplayModifierEvaluatedData EvaluatedData(UMyAttributeSetBase::GetInComingDamageAttribute(),EGameplayModOp::Additive,Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);

	//for ui
	UMyAbilitySystemBlueprintLibrary::SetIsBlockedHit(EffectContextHandle,bIsBlockChance);
	UMyAbilitySystemBlueprintLibrary::SetIsCriticalHit(EffectContextHandle,bIsCriticalHit);
}
