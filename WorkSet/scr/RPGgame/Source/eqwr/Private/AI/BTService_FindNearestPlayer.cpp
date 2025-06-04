// why


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"
#include "VectorTypes.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AActor* OwnerActor = AIOwner->GetPawn();

	const FName TargetTag =OwnerActor->ActorHasTag(FName("Player"))? FName("Enemy"):FName("Player");
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(this,TargetTag,ActorsWithTag);

	AActor* ClosestActor = nullptr;
	float ClosestDistance = FLT_MAX;

	for (AActor* Actor : ActorsWithTag)
	{
		const float Distance = OwnerActor->GetDistanceTo(Actor);
		
		if (Distance < ClosestDistance)
		{
			ClosestActor = Actor;
			ClosestDistance = Distance;
		}
	}

	UBTFunctionLibrary::SetBlackboardValueAsObject(this,TargetToFollow,ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this,DistanceToMove,ClosestDistance);
}
