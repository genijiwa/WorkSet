// why

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "MyAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class EQWR_API UMyAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UMyAssetManager& Get();

protected:
	// same time get my ban ben
	virtual void StartInitialLoading() override;
};
