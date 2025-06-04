//why

#pragma once

#include "CoreMinimal.h"
#include "MyWidgetController.h"
#include "GameplayEffectTypes.h"
#include "AssetManager/AbilityInfo.h"
#include "OverlayWidgetController.generated.h"

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UMyUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewAttribute);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowDelegate,FUIWidgetRow,Row);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGAUIDelegate,const FStrAbilityInfo,Info);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class EQWR_API UOverlayWidgetController : public UMyWidgetController
{
	GENERATED_BODY()
	
public:

	virtual void  BroadcastInitialValues() override;
	virtual void  BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes");
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes");
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes");
	FOnAttributeChangedSignature OnManaChanged;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes");
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Message");
	FMessageWidgetRowDelegate MessageWidgetRowDelegate;

	UPROPERTY(BlueprintAssignable)
	FGAUIDelegate GAUIDelegate;

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable,const FGameplayTag& Tag);
	
protected:
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Widget Data");
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly);
	TObjectPtr<UAbilityInfo> Abilityinfos;

};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	//findrow，行名，所以tag。name和行名一摸一样.
	T* Row = DataTable->FindRow<T>(Tag.GetTagName(),TEXT(""));
	return Row;
}


