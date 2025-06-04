// why


#include "Character/enemy.h"
#include <eqwr/eqwr.h>
#include <AbilitySystem/AbilitySystemComponentBase.h>
#include <AbilitySystem/MyAttributeSetBase.h>

#include "NativeGameplayTags.h"
#include "AbilitySystem/MyAbilitySystemBlueprintLibrary.h"
#include "AssetManager/MyGameplayTags.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Tests/AutomationCommon.h"
#include "UI/Widget/MyUserWidget.h"


void Aenemy::Hightlight()
{	
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void Aenemy::UnHightlight()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void Aenemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* Aenemy::GetCombatTarget_Implementation()
{
	return CombatTarget;
}

Aenemy::Aenemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponentBase>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributSet = CreateDefaultSubobject<UMyAttributeSetBase>("AttributSet");//this attribute

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	//
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

int32 Aenemy::GetPlayerLevel()
{
	return Level;
}

void Aenemy::InitializeDefaultAttributes() const
{
	UMyAbilitySystemBlueprintLibrary::InitializeDefaultAttributes(this,CharacterClass,Level,AbilitySystemComponent);
}

void Aenemy::Die()
{
	SetLifeSpan(LifeSpan);
	HealthBar->DestroyComponent();
	if (AIController) AIController->GetBlackboardComponent()->SetValueAsBool(FName("bDead"),true);
	Super::Die();
}

void Aenemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;

	//component run the asset under component to run.
	AIController = Cast<ARPGAIController>(NewController);
	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
	BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AIController->RunBehaviorTree(BehaviorTree);
	
	AIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"),CharacterClass != ECharacterClass::Warrior);
	AIController->GetBlackboardComponent()->SetValueAsBool(FName("BeHit"), false);
}

void Aenemy::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAbilitySystemComponentBase>(AbilitySystemComponent)->AbilityActorInfoSet();

	if(HasAuthority())
	{
		InitializeDefaultAttributes();	
	}

	UE_LOG(LogTemp, Warning, TEXT("%s 的Stength为 %f"), *this->GetName(), Cast<UMyAttributeSetBase>(AttributSet)->GetStrength());

	AbilitySystemComponent->RegisterGameplayTagEvent(FMyGameplayTags::Get().HitReact,EGameplayTagEventType::NewOrRemoved).
	AddUObject(this,&ThisClass::HitReactTagChanged);//be hit

	if (HasAuthority()) UMyAbilitySystemBlueprintLibrary::GiveStartupAbilities(this,AbilitySystemComponent,CharacterClass);

	//healthbar
	
	
	if (UMyUserWidget* UserWidget = Cast<UMyUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		UserWidget->SetWidgetController(this);// bind delegat .inportant
	}

	if (const UMyAttributeSetBase* AS = Cast<UMyAttributeSetBase>(AttributSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)//the delegate value
			{
				OnHealthChanged.Broadcast(Data.NewValue);	
			}
			);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});
		//inite
		OnHealthChanged.Broadcast(AS->GetHealth());
		OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());
	}

	
}

void Aenemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsHiting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsHiting? 0.f : BaseSpeed;
	if (AIController && AIController->GetBlackboardComponent())
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(FName("BeHit"), bIsHiting);
	}
}
