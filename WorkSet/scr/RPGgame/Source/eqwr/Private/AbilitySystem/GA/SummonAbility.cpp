// why


#include "AbilitySystem/GA/SummonAbility.h"

#include "AI/RPGAIController.h"

TSubclassOf<APawn> USummonAbility::RandomGetClass()
{
	return SpawnClass[FMath::RandRange(0, SpawnClass.Num() - 1)];
}

TArray<FVector> USummonAbility::GetSpawnLocation()
{
	TArray<FVector> SpawnLocation;

	FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	FVector FaceDirection = GetAvatarActorFromActorInfo()->GetActorForwardVector();

	float DeltaSpread = SpawnSpread / (Num-1.f);
	FVector LeftSpread = FaceDirection.RotateAngleAxis(-SpawnSpread/2.f, FVector::UpVector);

	for (int i = 0; i < Num; i++)
	{
		float Distance = FMath::RandRange(MinDistance, MaxDistance);
		FVector Spread = LeftSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		FVector CurLocation = Location + Spread * Distance;
		
		SpawnLocation.Add(CurLocation);
	}
	return SpawnLocation;
}
