// why


#include "Actor/MyEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "DSP/BufferDiagnostics.h"

AMyEffectActor::AMyEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}


// Called when the game starts or when spawned
void AMyEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyEffectActor::OnOverlap(AActor* TargetActor)
{
	//enemy
	if (TargetActor->ActorHasTag("Enemy")&&!bApplytoEnemy) return;
	
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor,InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap){
		ApplyEffectToTarget(TargetActor,DurationGameplayEffectClass);
	}
	if (InfinityEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor,InfinityGameplayEffectClass);
	}
}

void AMyEffectActor::OnEndOverlap(AActor* TargetActor)
{

	//enemy
	if (TargetActor->ActorHasTag("Enemy")&&!bApplytoEnemy) return;
	//各个情况应用，方便蓝图.
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,DurationGameplayEffectClass);
	}
	if (InfinityEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,InfinityGameplayEffectClass);
	}

	//移除，针对infinity.
	if (InfinityEffectRemovalPolicy == EEffectRemovalPolicy::RemovalOnOverlap)
	{
		
		const IAbilitySystemInterface* ASCInterface = CastChecked<IAbilitySystemInterface>(TargetActor);
		UAbilitySystemComponent* TargetASC = ASCInterface->GetAbilitySystemComponent();

		if (!IsValid(TargetASC)) return;

		//记录了
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		
		//移除角色身上effct
		for (auto HandlePair : ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key,1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
	//active句柄很重要，记录.//
	//记录更改
		for (auto& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.Remove(Handle);
		}	
	}
}

void AMyEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	const IAbilitySystemInterface* ASCInterface = CastChecked<IAbilitySystemInterface>(TargetActor);
	UAbilitySystemComponent* TargetASC = ASCInterface->GetAbilitySystemComponent();
	
	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();

	EffectContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass,ActorLevel, EffectContextHandle);

	//有移除的时候.
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	
	if (bIsInfinite && InfinityEffectRemovalPolicy == EEffectRemovalPolicy::RemovalOnOverlap)
	{
		ActiveEffectHandles.Add(ActiveGameplayEffectHandle,TargetASC);
	}
	else if (bDestroyOnEffectApplication)
	{
		Destroy();
	}
	//end
}


