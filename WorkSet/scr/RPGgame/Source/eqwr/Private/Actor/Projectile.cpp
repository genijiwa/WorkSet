// why


#include "Actor/Projectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagsSettings.h"
#include "NiagaraFunctionLibrary.h"
#include "Chaos/AABBTree.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "DataWrappers/ChaosVDQueryDataWrappers.h"
#include "eqwr/eqwr.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_PROJECTILE);//new type
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);//overlap or ray
	Sphere->SetCollisionResponseToChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);

	Sphere->OnComponentBeginOverlap.AddDynamic(this,&AProjectile::OnSphereOverlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);

	LoopSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopSound,GetRootComponent());
}

void AProjectile::Destroyed()
{
	if (!HasAuthority() && !bHit)
	{
		Impact();
		bHit = true;
	}
	Super::Destroyed();
}

void AProjectile::Impact() const  //fix the fuction ,before fix the return
{
	UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation(),FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,GetActorLocation());

	//change sound
	if (LoopSoundComponent) LoopSoundComponent->Stop();
}

void AProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* TheOwner = GetOwner();
	AActor* TempActor = GetInstigator();
	if (OtherActor && OtherActor != this && OtherActor != GetInstigator())
	{
		Impact();

		if (HasAuthority())
		{
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
			{
				TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectHandle.Data.Get());
			}
			Destroy();
		}
		else
		{
			bHit = true;
		}
	}
}



