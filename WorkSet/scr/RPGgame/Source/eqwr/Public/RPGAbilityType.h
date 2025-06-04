#pragma once

#include "GameplayEffectTypes.h"
#include "RPGAbilityType.generated.h"


USTRUCT(BlueprintType)
struct FRPGGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

public:
	bool IsBlockedHit() const{ return bIsBlockedHit;};
	bool IsCriticalHit() const{ return bIsCriticalHit;};

	void SetIsBlockedHit(const bool IsBlockedHit){ bIsBlockedHit = IsBlockedHit;};
	void SetIsCriticalHit(const bool IsCriticalHit){ bIsCriticalHit = IsCriticalHit;};

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGameplayEffectContext::StaticStruct();
	}

	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	//deep copy
	virtual FRPGGameplayEffectContext* Duplicate() const override
	{
		FRPGGameplayEffectContext* NewContext = new FRPGGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
};

// can read
template<>
struct TTypeTraits<FRPGGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FRPGGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};


