// why


#include "AbilitySystem/AbilitySystemComponentBase.h"
#include "AbilitySystem/MyAttributeSetBase.h"
#include "UI/WidgetController/OverlayWidgetController.h"

//下面的监听委托已经该值了，这个委托仅仅传值用于蓝图内ui回调函数逻辑.
//health&mana show
void UOverlayWidgetController::BroadcastInitialValues()
{
	const UMyAttributeSetBase* AttributeSetBase = CastChecked<UMyAttributeSetBase>(AttributeSet);

	OnHealthChanged.Broadcast(AttributeSetBase->GetHealth());
	OnMaxHealthChanged.Broadcast(AttributeSetBase->GetMaxHealth());
	OnManaChanged.Broadcast(AttributeSetBase->GetMana());
	OnMaxManaChanged.Broadcast(AttributeSetBase->GetMaxMana());
	
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UMyAttributeSetBase* AttributeSetBase = CastChecked<UMyAttributeSetBase>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSetBase->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSetBase->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
			);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSetBase->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
			);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSetBase->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);

	//get massage show
	//tag ui message 从asc绑定委托
	Cast<UAbilitySystemComponentBase>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
	{
		for (const auto& Tag : AssetTags)
		{
			
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Message"))))
			{
				//蓝图没有使用的tag，在这里发挥作用，传row.连接effect和table.
				FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
				if (Row)
				{
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Row is nullptr for Tag: %s"), *Tag.ToString());
				}
			}
		}
	}
	);

	Cast<UAbilitySystemComponentBase>(AbilitySystemComponent)->ACSForGA.AddLambda(
		[this](const UAbilitySystemComponent* ASC)
		{
			for (const FGameplayAbilitySpec& AbilitySpec : ASC->GetActivatableAbilities())
			{
				for(FGameplayTag GA_Tag : AbilitySpec.Ability->AbilityTags)
				{
					if (GA_Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("GA.Hero"))))
					{
						FStrAbilityInfo AbilityInfo = Abilityinfos->GetAbilityInfoByTag(GA_Tag);
						
						GAUIDelegate.Broadcast(AbilityInfo);
					}
				}
			}
		}
		);
}

