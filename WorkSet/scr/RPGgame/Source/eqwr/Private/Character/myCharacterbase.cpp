// why


#include "Character/myCharacterbase.h"
#include <AbilitySystem/AbilitySystemComponentBase.h>
#include <AbilitySystem/MyAttributeSetBase.h>

#include "MovieSceneSection.h"
#include "AssetManager/MyGameplayTags.h"
#include "Components/CapsuleComponent.h"
#include "eqwr/eqwr.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AmyCharacterbase::AmyCharacterbase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");//ָ��ָ�����.
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));//editor��Ҳ��parent socket����.
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);//editor has ��ȫһ��colission enable,
	//����editor������enemy��hero����Ҫ�������ֻʣ�¹���������ö���,������ȷ��˼·Ӧ���Ǵӱ༭����cpp�࣬cpp�����ڱ༭�����Ǻ�˷�����ǰ�ˡ�

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_PROJECTILE,ECR_Overlap);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

}

UAbilitySystemComponent* AmyCharacterbase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AmyCharacterbase::AddCharacterAbilities()
{
	UAbilitySystemComponentBase* ASC = Cast<UAbilitySystemComponentBase>(GetAbilitySystemComponent());
	if (!HasAuthority()){return;}

	ASC->AddGameplayAbilities(StartupAbilities);
	ASC->AddPassiveAbilities(StartupPassiveAbilities);
}


FVector AmyCharacterbase::GetCombatSocketLocation_Implementation()
{
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

UAnimMontage* AmyCharacterbase::GetHitMontage_Implementation()
{
	return HitMontage;
}

FTaggedMontage AmyCharacterbase::GetMontageStructByTag_Implementation(const FGameplayTag& Tag)
{
	for (auto MontageStruct : AttackMontage)
	{
		if (MontageStruct.MontageTag.MatchesTagExact(Tag)) return MontageStruct;
	}
	return FTaggedMontage();
}

UNiagaraSystem* AmyCharacterbase::GetBloodNS_Implementation()
{
	return BloodNS;
}

void AmyCharacterbase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	
	MulticastHandleDeath();
}

bool AmyCharacterbase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AmyCharacterbase::GetAvatar_Implementation()
{
	return this;
}

FVector AmyCharacterbase::GetCombatSocketLocationByStruct_Implementation(const FTaggedMontage TaggedMontage) const
{
	if (TaggedMontage.MontageTag.MatchesTagExact(FMyGameplayTags::Get().Montage_Attack_Weapon))
	{
		return Weapon->GetSocketLocation(TaggedMontage.CombatTipSocketName);
	}
	else
	{
		return GetMesh()->GetSocketLocation(TaggedMontage.CombatTipSocketName);
	}
}

TArray<FTaggedMontage> AmyCharacterbase::GetAttackMontage_Implementation()
{
	return AttackMontage;
}


void AmyCharacterbase::MulticastHandleDeath_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this,DeathSound,GetActorLocation());
	
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Dissolve();

	bDead = true;
}

void AmyCharacterbase::Dissolve()
{
	TArray<UMaterialInstanceDynamic*> MaterialArray;
	
	if (IsValid(CharacterDissolveMaterial))
	{
		auto MaterialInstance = UMaterialInstanceDynamic::Create(CharacterDissolveMaterial,this);
		GetMesh()->SetMaterial(0,MaterialInstance);
		MaterialArray.Add(MaterialInstance);
	}

	if (IsValid(WeaponDissolveMaterial))
	{
		auto MaterialInstance = UMaterialInstanceDynamic::Create(CharacterDissolveMaterial,this);
		Weapon->SetMaterial(0,MaterialInstance);
		MaterialArray.Add(MaterialInstance);
	}
	StartDissolveTimeline(MaterialArray);
}

// Called when the game starts or when spawned
void AmyCharacterbase::BeginPlay()
{
	Super::BeginPlay();
	
}


void AmyCharacterbase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(EffectClass);

	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(EffectClass,1.0f,ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),GetAbilitySystemComponent());
}

void AmyCharacterbase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultSecondaryAttributes,1.f);
	ApplyEffectToSelf(DefaultPrimaryAttributes,1.f);
	ApplyEffectToSelf(DefaultVitalAttributes,1.f);
}


