// why


#include "Character//HeroCharacter.h"
#include"GameFramework/CharacterMovementComponent.h"
#include "Player/PlayerStateBase.h"
#include "AbilitySystem/AbilitySystemComponentBase.h"
#include "AbilitySystem/MyAttributeSetBase.h"
#include "player/HeroPlayerController.h"
#include "UI/HUD/MyHUD.h"
#include "AbilitySystem/MyAttributeSetBase.h"

AHeroCharacter::AHeroCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true; //����Ϊtrue����ɫ�����ƶ��ķ�����ת
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f); //��ת����
	GetCharacterMovement()->bConstrainToPlane = true; //Լ����ƽ��
	GetCharacterMovement()->bSnapToPlaneAtStart = true; //����������һ��Ϊtrue���Ҵ�������Ϊtrue�����ڿ�ʼʱ��������

	//����ʹ�ÿ�����ѡ��Pitch��Yaw��Roll��ֵ��
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();

	AddCharacterAbilities();
}

void AHeroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
	//�ж����������Щʹ�����Ըı�ĺ�ֻ�Ǹı����ԣ���������Ҫ���³�ʼ��.
}

int32 AHeroCharacter::GetPlayerLevel()
{
	const APlayerStateBase* PlayerStateBase = GetPlayerState<APlayerStateBase>();
	check(PlayerStateBase);
	return PlayerStateBase->GetPlayerLevel();
}


//关卡的对象首先创建实例，而后cpp可以交互运用.
void AHeroCharacter::InitAbilityActorInfo()
{
	APlayerStateBase* PlayerStateBase = GetPlayerState<APlayerStateBase>();
	check(PlayerStateBase);
	//��playerstate��ȡ
	AbilitySystemComponent = PlayerStateBase->GetAbilitySystemComponent();//owning is playerstate
	AttributSet = PlayerStateBase->GetAttributeSet();
	//��ʼ��acs,ownerActor and avatarActor.
	AbilitySystemComponent->InitAbilityActorInfo(PlayerStateBase, this);

	Cast<UAbilitySystemComponentBase>(AbilitySystemComponent)->AbilityActorInfoSet();

	//get pc
	
	if (AHeroPlayerController* HeroPlayerController = Cast<AHeroPlayerController>(GetController()))
	{
		if (AMyHUD* HUD = Cast<AMyHUD>(HeroPlayerController->GetHUD()))
		{
			HUD->InitOverlay(HeroPlayerController, PlayerStateBase, AbilitySystemComponent, AttributSet);
		}
	}
	InitializeDefaultAttributes();
}