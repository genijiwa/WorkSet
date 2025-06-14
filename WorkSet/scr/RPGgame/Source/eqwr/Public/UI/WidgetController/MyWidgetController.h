// why

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MyWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;


USTRUCT(BlueprintType)
struct FWidgetControllerParams {

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
	:PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC),AttributeSet(AS)
	{}
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr <APlayerController> PlayerController = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

/**
 * 
 */
UCLASS()
class EQWR_API UMyWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	
	
	UFUNCTION(BlueprintCallable)
	void SetWdigetControllerParams(const FWidgetControllerParams& WCParams);

	//ok你好》
	virtual void  BroadcastInitialValues();
	virtual void  BindCallbacksToDependencies();

	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController");
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController");
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController"); 
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController");
	TObjectPtr<UAttributeSet> AttributeSet;	
	
};


