// why

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InputConfig.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FInputActionStruct
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};
UCLASS()
class EQWR_API UInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FInputActionStruct> AbilityInputActions;

	const UInputAction* FindInputActionByTag(const FGameplayTag& InputTag,bool bLogNotFound = false) const;
	
};
