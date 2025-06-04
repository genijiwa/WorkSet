// why

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/MyWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UDataAttributeInfo;
struct FAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature,const FAttributeInfo& ,Info);
/**
 * 
 */



UCLASS(BlueprintType, Blueprintable)
class EQWR_API UAttributeMenuWidgetController : public UMyWidgetController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() override;
	
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;
	
	
protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataAttributeInfo> AttributeInfo;

	
};
