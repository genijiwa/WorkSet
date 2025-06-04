// why


#include "AssetManager/MyAssetManager.h"

#include "AssetManager/MyGameplayTags.h"

UMyAssetManager& UMyAssetManager::Get()
{
	check(GEngine);

	UMyAssetManager* MyAssetManager = Cast<UMyAssetManager>(GEngine->AssetManager);
	return *MyAssetManager;
}

void UMyAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FMyGameplayTags::InitializeNativeGameplayTags();
	FMyGameplayTags::InitializeInputGameplayTags();
}
