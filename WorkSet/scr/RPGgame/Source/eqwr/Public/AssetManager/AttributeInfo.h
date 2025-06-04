// why

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();
	

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText AttributeName =FText() ;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText AttributeDescription =FText() ;

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
};
UCLASS()
class EQWR_API UDataAttributeInfo : public UDataAsset
{
	
	GENERATED_BODY()

public:
	
	FAttributeInfo FindAttributeInfoByTag(const FGameplayTag& AtrributeTag,bool bLogNoFound = false) const;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FAttributeInfo> AttributeInformation;
};
