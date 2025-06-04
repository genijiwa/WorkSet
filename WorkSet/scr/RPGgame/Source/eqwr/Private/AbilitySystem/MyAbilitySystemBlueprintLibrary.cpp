// why


#include "AbilitySystem/MyAbilitySystemBlueprintLibrary.h"

#include "AssetTypeCategories.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/GA/GameplayAbilityBase.h"
#include "AssetManager/CharacterClassInfo.h"
#include "Components/SplineComponent.h"
#include "Engine/OverlapResult.h"
#include "Game/MyGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerStateBase.h"
#include "UI/HUD/MyHUD.h"
#include "UI/WidgetController/MyWidgetController.h"

class UGameplayAbilityBase;

UAttributeMenuWidgetController* UMyAbilitySystemBlueprintLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AMyHUD* HUD = Cast<AMyHUD>(PC->GetHUD()))
		{
			APlayerStateBase* PS = PC->GetPlayerState<APlayerStateBase>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC,PS, ASC, AS);
			return HUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UOverlayWidgetController* UMyAbilitySystemBlueprintLibrary::GetOverlayWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AMyHUD* HUD = Cast<AMyHUD>(PC->GetHUD()))
		{
			APlayerStateBase* PS = PC->GetPlayerState<APlayerStateBase>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC,PS, ASC, AS);
			return HUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UMyAbilitySystemBlueprintLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass,
												  float Level, UAbilitySystemComponent* ASC)
{
	const AMyGameModeBase* GameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (GameMode == nullptr) return;

	UCharacterClassInfo* ClassInfo = GameMode->CharacterClassInfo;

	auto SecContextHandle = ASC->MakeEffectContext();
	SecContextHandle.AddSourceObject(WorldContextObject);
	auto SecSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->SecondaryAttributes,Level,SecContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecSpecHandle.Data.Get());

	auto PrimaryDefault = ClassInfo->GetEffectByClass(CharacterClass);
	auto PriContextHandle = ASC->MakeEffectContext();
	PriContextHandle.AddSourceObject(WorldContextObject);
	auto PriSpecHandle = ASC->MakeOutgoingSpec(PrimaryDefault.PrimaryAttributes,Level,PriContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PriSpecHandle.Data.Get());


	auto VitContextHandle = ASC->MakeEffectContext();
	VitContextHandle.AddSourceObject(WorldContextObject);
	auto VitSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->VitalAttributes,Level,VitContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitSpecHandle.Data.Get());
}

void UMyAbilitySystemBlueprintLibrary::GiveStartupAbilities(const UObject* WorldContextObject,
	UAbilitySystemComponent* ASC,ECharacterClass CharacterClass)
{
	const AMyGameModeBase* GameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));

	if (GameMode == nullptr) return;
	
	auto Abilities = GameMode->CharacterClassInfo->CommonAbilities;

	for (auto Ability : Abilities)
	{
		auto AbilitySpec = FGameplayAbilitySpec(Ability,Cast<ICombatInterface>(ASC->GetAvatarActor())->GetPlayerLevel());
		const UGameplayAbilityBase* AbilityBase = Cast<UGameplayAbilityBase>(AbilitySpec.Ability);
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityBase->StartupInputTag);
		ASC->GiveAbility(AbilitySpec);
	}

	// class abilities
	const FCharacterClassDefaultInfo CharacterClassInfo = GameMode->CharacterClassInfo->GetEffectByClass(CharacterClass);
	//class GA
	for (auto Ability : CharacterClassInfo.StartupAbilities)
	{
		auto AbilitySpec = FGameplayAbilitySpec(Ability,Cast<ICombatInterface>(ASC->GetAvatarActor())->GetPlayerLevel());
		ASC->GiveAbility(AbilitySpec);
	}
	
}

bool UMyAbilitySystemBlueprintLibrary::IsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRPGGameplayEffectContext* REffectContext = static_cast<const FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->IsBlockedHit();
	}
	return false;
}

bool UMyAbilitySystemBlueprintLibrary::IsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRPGGameplayEffectContext* REffectContext = static_cast<const FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->IsCriticalHit();
	}
	return false;
}

void UMyAbilitySystemBlueprintLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool BinIsBlockedHit)
{
	if ( FRPGGameplayEffectContext* REffectContext = static_cast< FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetIsBlockedHit(BinIsBlockedHit);
	}
}

void UMyAbilitySystemBlueprintLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool BinIsCriticaHit)
{
	if ( FRPGGameplayEffectContext* REffectContext = static_cast< FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetIsCriticalHit(BinIsCriticaHit);
	}
}

void UMyAbilitySystemBlueprintLibrary::GetLivePlayersWithRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	
	TArray<FOverlapResult> Overlaps;
	
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull);
	World->OverlapMultiByObjectType(Overlaps,SphereOrigin,FQuat::Identity,FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType
		::AllDynamicObjects),FCollisionShape::MakeSphere(Radius),SphereParams);
	
	for (FOverlapResult& Overlap : Overlaps)
	{
		if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()) )
		{
			OutOverlappingActors.AddUnique(Overlap.GetActor());
		}
	}
}

float UMyAbilitySystemBlueprintLibrary::GetScalableFloatValue(const FScalableFloat ScalableFloat, const int32 Level)
{
	return ScalableFloat.GetValueAtLevel(Level);
}
