// why


#include "Player/HeroPlayerController.h"
#include"EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MovieSceneTracksComponentTypes.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/MyAbilitySystemBlueprintLibrary.h"
#include "AssetManager/MyGameplayTags.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "Input/InputComponentBase.h"
#include "Kismet/GameplayStatics.h"

void AHeroPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector  ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector  RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>()) {
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);//����Ĳ���ȫΪ�����addmovrmentInput������.
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AHeroPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	//����һ֡5�ֿ������С�㷨.
	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());//inside say what is object
	//simple
	if (ThisActor !=  LastActor)
	{
		if (ThisActor){ThisActor->Hightlight();};
		if (LastActor){LastActor->UnHightlight();};
	} 
}



AHeroPlayerController::AHeroPlayerController()
{
	bReplicates = true;
	LastActor = nullptr;
	ThisActor = nullptr;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AHeroPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

	AutoRun();
}

UAbilitySystemComponentBase* AHeroPlayerController::GetASC()
{
	if (AbilitySystemComponent == nullptr)
	{
		AbilitySystemComponent = Cast<UAbilitySystemComponentBase>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return AbilitySystemComponent;
}

void AHeroPlayerController::OnPresse(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FMyGameplayTags::Get().InputTag_RMB))
	{
		bTarget = ThisActor != nullptr;
		bAutoRun = false;
		FollowTime = 0.f;
	}
}

void AHeroPlayerController::AutoRun()
{
	if (bAutoRun)
	{
		APawn* ControlledPawn = GetPawn<APawn>();
		check(ControlledPawn);
		
		FVector TarLocation=Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(),ESplineCoordinateSpace::World);
		const FVector TarDirection = Spline->FindDirectionClosestToWorldLocation(TarLocation,ESplineCoordinateSpace::World);//the point qiexian
		
		ControlledPawn->AddMovementInput(TarDirection);

		//radium
		
		auto CurLength = (TarLocation - CachedDirection).Size2D();//fuck
		if (CurLength <= AutoRunRadius)
		{
			bAutoRun = false;
		}
	}
}

void AHeroPlayerController::OnRelease(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTag(FMyGameplayTags::Get().InputTag_RMB))
	{
		if (GetASC())
		{
		GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}
	
	if (bTarget)
	{
		GetASC()->AbilityInputTagReleased(InputTag);
	}
	else
	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPresstime && ControlledPawn != nullptr)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,
				ControlledPawn->GetActorLocation(),CachedDirection))
			{
				Spline->ClearSplinePoints();//per once
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc,ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Orange, false, 5.f);
				}
				
				CachedDirection = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				bAutoRun = true;
			}
		}
	}
}

void AHeroPlayerController::OnHold(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FMyGameplayTags::Get().InputTag_RMB))
	{
		if (GetASC() != nullptr)
		{
			GetASC()->AbilityInputTagHold(InputTag);
		}
		return;
	}
	
	if (bTarget)
	{
		if (GetASC() != nullptr)
		{
			GetASC()->AbilityInputTagHold(InputTag);
		}
		return;
	}
	else
	{
		//followtime druge.
		FollowTime +=GetWorld()->GetDeltaSeconds();
		
		APawn* ControlledPawn = GetPawn<APawn>();
		check(ControlledPawn);
		
		if (GetHitResultUnderCursor(ECC_Visibility, false, CursorHit))
		{
			CachedDirection = CursorHit.ImpactPoint;
		}
		auto CurPisition = ControlledPawn->GetActorLocation();
		auto TarDirection = CachedDirection - CurPisition;
		ControlledPawn->AddMovementInput(TarDirection);
	}
}

void AHeroPlayerController::ShowDamageText_Implementation(float DamageNum, ACharacter* TargetCharacter,bool bIsBlockedHit , bool bIsCriticalHit)
{
	UE_LOG(LogTemp, Log, TEXT("ShowDamageText called for Damage: %f"), DamageNum)
	
	if(IsValid(TargetCharacter) && DamageWidgetComponentClass)
	{
		
		UDamageWidgetComponent* DamageWidgetComponent = NewObject<UDamageWidgetComponent>(TargetCharacter,DamageWidgetComponentClass);
		
			
		DamageWidgetComponent->RegisterComponent();// space in pinmu no changjing!!!!
		//location effect
		DamageWidgetComponent->AttachToComponent(TargetCharacter->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
		DamageWidgetComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		
		DamageWidgetComponent->SetDamageText(DamageNum,bIsBlockedHit , bIsCriticalHit);
		
	}
}

void AHeroPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (!IsLocalController()){return;};
	
	check(CharacterContext);//check ָ���Ƿ�Ϊ�գ�������.
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());//getlocalplayer�Ǵ���ĺ���.
	
	check(Subsystem);
	Subsystem->AddMappingContext(CharacterContext, 0);//����������Լ����ȼ�and Enhancedinputlocalplayersubsystem

	//���
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;//one struct.
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);

	SetInputMode(InputModeData);//struct to controller
}

void AHeroPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UInputComponentBase* EnhandceInputComponent = CastChecked<UInputComponentBase>(InputComponent);

	EnhandceInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHeroPlayerController::Move);
	//BindAction�������ã�ÿ�� moveaction trigger��::move����.

	EnhandceInputComponent->BindActionAbilities(InputConfig, this,
		&ThisClass::OnPresse,&ThisClass::OnRelease,&ThisClass::OnHold);
}


//control�Ǵ������Ľ�ɫ�������Ľ�ɫ�Ǹ������Ǹ���.so GetControl(),
