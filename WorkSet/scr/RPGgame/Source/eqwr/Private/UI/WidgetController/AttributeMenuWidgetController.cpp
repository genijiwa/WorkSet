// why


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/MyAttributeSetBase.h"
#include "AssetManager/AttributeInfo.h"
#include "Misc/LowLevelTestAdapter.h"


void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	check(AttributeInfo);
	
	const UMyAttributeSetBase* AS = Cast<UMyAttributeSetBase>(AttributeSet);
	
	for (auto Pair:AS->TagtoAttribute)
	{
		FAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(Pair.Key);
		Info.AttributeValue = Pair.Value().GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	check(AttributeInfo);

	
	const UMyAttributeSetBase* AS = Cast<UMyAttributeSetBase>(AttributeSet);

	for (auto& Pair:AS->TagtoAttribute)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this,Pair,AS](const FOnAttributeChangeData& Data)
			{
				FAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(Pair.Key);
				Info.AttributeValue = Pair.Value().GetNumericValue(AS);
				AttributeInfoDelegate.Broadcast(Info);
			}
			);
	}
}
