// why

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "RPGAbilityType.h"
#include "AssetManager/CharacterClassInfo.h"

#include "MyAbilitySystemBlueprintLibrary.generated.h"

class UAttributeMenuWidgetController;
class UOverlayWidgetController;


/**
 * 
 */

UCLASS()
class EQWR_API UMyAbilitySystemBlueprintLibrary : public UAbilitySystemBlueprintLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure,Category="MyAbilitySystemBlueprintLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure,Category="MyAbilitySystemBlueprintLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="MyAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC );

	UFUNCTION(BlueprintCallable, Category="MyAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject,UAbilitySystemComponent* ASC,ECharacterClass CharacterClass);

	UFUNCTION(BlueprintPure, Category="MyAbilitySystemLibrary|GameplayEffect")
	static bool IsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category="MyAbilitySystemLibrary|GameplayEffect")
	static bool IsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category="MyAbilitySystemLibrary|GameplayEffect")
	static  void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool BinIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category="MyAbilitySystemLibrary|GameplayEffect")
	static  void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool BinIsCriticaHit);

	UFUNCTION(BlueprintCallable, Category="MyAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore,
		float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, Category="MyAbilitySystemLibrary|GameplayMechanics")
	static float GetScalableFloatValue(const FScalableFloat ScalableFloat,const int32 Level);

};


