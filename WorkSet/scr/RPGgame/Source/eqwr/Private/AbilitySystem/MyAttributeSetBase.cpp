// why


#include "AbilitySystem/MyAttributeSetBase.h"

#include <functional>

#include "EditorCategoryUtils.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "LevelInstance/LevelInstanceTypes.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/MyAbilitySystemBlueprintLibrary.h"
#include "AssetManager/MyGameplayTags.h"
#include "Character/enemy.h"
#include "Game/MyGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HeroPlayerController.h"
#include "Player/PlayerStateBase.h"


class AMyGameModeBase;

UMyAttributeSetBase::UMyAttributeSetBase()
{
	
	const FMyGameplayTags& GameplayTag = FMyGameplayTags::Get();
	TagtoAttribute.Add(GameplayTag.Attributes_Primary_Intelligence,GetIntelligenceAttribute);
	TagtoAttribute.Add(GameplayTag.Attributes_Primary_Resilience,GetResilienceAttribute);
	TagtoAttribute.Add(GameplayTag.Attributes_Primary_Strength,GetStrengthAttribute);
	TagtoAttribute.Add(GameplayTag.Attributes_Primary_Vigor,GetVigorAttribute);
	TagtoAttribute.Add(GameplayTag.Attributes_Secondary_Armor,GetArmorAttribute);
	TagtoAttribute.Add(GameplayTag.Attributes_Secondary_ArmorPenetration,GetArmorPenetrationAttribute);
	TagtoAttribute.Add(GameplayTag.Attributes_Secondary_BlockChance,GetBlockChanceAttribute);
	TagtoAttribute.Add(GameplayTag.Attributes_Secondary_HealthRegeneration,GetHealthRegenerationAttribute);
	TagtoAttribute.Add(GameplayTag.Attributes_Secondary_MaxHealth,GetMaxHealthAttribute);
	TagtoAttribute.Add(GameplayTag.Attributes_Secondary_ManaRegeneration,GetManaRegenerationAttribute);
	TagtoAttribute.Add(GameplayTag.Attributes_Secondary_MaxMana,GetMaxManaAttribute);
	TagtoAttribute.Add(GameplayTag.Attributes_Secondary_CriticalHitChance,GetCriticalHitChanceAttribute);
	TagtoAttribute.Add(GameplayTag.Attributes_Secondary_CriticalHitDamage,GetCriticalHitDamageAttribute);
	TagtoAttribute.Add(GameplayTag.Attributes_Secondary_CriticalHitResistance,GetCriticalHitResistanceAttribute);
}

void UMyAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSetBase,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSetBase, MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSetBase, Mana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSetBase, MaxMana,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSetBase, Intelligence,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSetBase, Resilience,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSetBase, Strength,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSetBase, Vigor,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSetBase, Armor, COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSetBase, ArmorPenetration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSetBase, BlockChance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSetBase, HealthRegeneration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSetBase, MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSetBase, ManaRegeneration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSetBase, MaxMana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSetBase, CriticalHitChance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSetBase, CriticalHitDamage,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSetBase, CriticalHitResistance,COND_None,REPNOTIFY_Always);


}

void UMyAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f,GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f,GetMaxMana());
	}
}

void UMyAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data,Props);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(),0.f,GetMaxHealth()));
		//UE_LOG(LogTemp, Warning, TEXT("%s 的生命值发生了修改，当前生命值：%f"), *Props.TargetAvatarActor->GetName(), GetHealth());
	}
	if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s 的Max生命值发生了修改，当前生命值：%f"), *Props.TargetAvatarActor->GetName(), GetMaxHealth());
	}
	
	if(Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(),0.f,GetMaxMana()));
	}


	/*XPChanged*/
	if (Data.EvaluatedData.Attribute == GetInXPAttribute())
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s 的InXP发生了修改，当前InXp：%f"), *Props.TargetAvatarActor->GetName(), GetInXP());
		
		APlayerStateBase* PS = Cast<APlayerStateBase>(Props.TargetController->PlayerState);
		check(PS);
		
		PS->AddPlayerXP(GetInXP());
		
		SetInXP(0);
	}
	/*End*/
	
	/*Damage*/
	if (Data.EvaluatedData.Attribute == GetInComingDamageAttribute())
	{
		const float LocalIncomingDamage = GetInComingDamage();
		SetInComingDamage(0.f);

		if (LocalIncomingDamage > 0)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth,0.f,GetMaxHealth()));
			const bool bFatal = NewHealth <= 0.f;

			if (!bFatal)
			{
				FGameplayTagContainer Container;
				Container.AddTag(FMyGameplayTags::Get().HitReact);
				
				Props.TargetACS->TryActivateAbilitiesByTag(Container);
				
				

				// auto Abilities = Props.TargetACS->GetActivatableAbilities();
				// for (auto Ability : Abilities)
				// {
				// 	Props.TargetACS->TryActivateAbility(Ability.Handle);
				// }
			}
			else
			{
				Cast<ICombatInterface>(Props.TargetCharacter)->Die();

				const AMyGameModeBase* GameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(this));
				if (GameMode == nullptr) return;
				
				int32 RewardXP = GameMode->CharacterClassInfo->GetRewardXPByClass(Cast<Aenemy>(Props.TargetCharacter)->CharacterClass);

				FGameplayEventData Payload;
				Payload.EventMagnitude = RewardXP;
				Payload.EventTag = FMyGameplayTags::Get().Attributes_Meta_IncomingXP;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter,FMyGameplayTags::Get().Attributes_Meta_IncomingXP,
					Payload);//the ability in the actor
			}
			//Damage Text
		}
		
		if (auto PC = Cast<AHeroPlayerController> (Props.SourceController))
		{
			bool bIsBlockedHit = UMyAbilitySystemBlueprintLibrary::IsBlockedHit(Props.EffectContextHandle);
			bool bIsCriticalHit = UMyAbilitySystemBlueprintLibrary::IsCriticalHit(Props.EffectContextHandle);
			PC->ShowDamageText(LocalIncomingDamage,Props.TargetCharacter,bIsBlockedHit,bIsCriticalHit);
		}
		
		//UE_LOG(LogTemp, Warning, TEXT("%s 的生命值发生了修改，当前生命值：%f"), *Props.TargetAvatarActor->GetName(), GetHealth());
	
	}
	/*Damage End*/
	
	
}

void UMyAttributeSetBase::SetEffectProperties(const FGameplayEffectModCallbackData& Data,FEffectProperties& Props)
{
	Props.EffectContextHandle = Data.EffectSpec.GetEffectContext();
	Props.SourceACS = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceACS) &&  Props.SourceACS->AbilityActorInfo.IsValid()&&Props.SourceACS->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceACS->GetAvatarActor();
		Props.SourceController = Props.SourceACS->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawm = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawm->GetController();//比如MOBA游戏中玩家和AI控制的英雄角色，OwnerActor是PlayerState、AvatarActor是HeroCharacter。
				//部分情况下OwnerActor和AvatarActor可以是角色Actor。不过想像一下你控制的英雄角色死亡然后重生的过程，如果此时要保留死亡前的Attributes或者GameplayEffects，
				//那么最理想的做法是将ASC交给PlayerState。
				
			}//ACS的owner和avatar，controller控制character或pawm,playstate是acs的owner，character是acs的avatar
		}
		if (Props.SourceController){
			
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetACS = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
	
}

//��д����麯������������ͬ����doreplifetiome����
//do replifetimeע�����.�������ֵ�ı䣬ͬ�����ͻ���.������ֵ��replictusing ����.��������gameplayatrribute��.maybe��¼����ֵ�������߼�����
//����old<new�������ֵ.Ҫ����.
//GAMEPLAYATTRIBUTE_REPNOTIFY 1.�����¾�ֵ 2.Onattributechange ��ί�У��¼��Ĺ㲥.֮�����onAttributechange.adddynamic().
//OnRep_Health()�ǻص�������������ɻ��洫��������ͬ��������һ����Ҫ���Ͳ�Ҫ����.
//�¼���ue��һ�����ڵײ�ĸ߼�ί��.
void UMyAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSetBase, Health, OldHealth);
}

//oldvalue是系统自动回调自动传的，名字自己定义，old准确而已
void UMyAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSetBase, MaxHealth, OldMaxHealth);
}

void UMyAttributeSetBase::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	//现有dorep通知，后有reptsing识别attribute变化调用OnRep_，game repnoty宏做了底层的事，他在回调函数里但是作用是在外》
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSetBase, Mana, OldMana);
}

void UMyAttributeSetBase::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSetBase, MaxMana, OldMaxMana);
}

void UMyAttributeSetBase::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSetBase, Strength, OldStrength);
}

void UMyAttributeSetBase::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSetBase, Intelligence, OldIntelligence);
}

void UMyAttributeSetBase::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSetBase, Resilience, OldResilience);
}

void UMyAttributeSetBase::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSetBase, Vigor, OldVigor);
}

void UMyAttributeSetBase::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSetBase, Armor, OldArmor);
}

void UMyAttributeSetBase::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSetBase,ArmorPenetration,OldArmorPenetration);
}

void UMyAttributeSetBase::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSetBase, BlockChance, OldBlockChance);
}

void UMyAttributeSetBase::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSetBase,CriticalHitDamage,OldCriticalHitDamage);
}

void UMyAttributeSetBase::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSetBase,CriticalHitChance,OldCriticalHitChance);
}

void UMyAttributeSetBase::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSetBase,CriticalHitResistance,OldCriticalHitResistance);

}

void UMyAttributeSetBase::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSetBase,HealthRegeneration,OldHealthRegeneration);
}

void UMyAttributeSetBase::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSetBase,ManaRegeneration,OldManaRegeneration);

}

