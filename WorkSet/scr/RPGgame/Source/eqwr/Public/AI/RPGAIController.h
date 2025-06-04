// why

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "RPGAIController.generated.h"

/**
 * 
 */
UCLASS()
class EQWR_API ARPGAIController : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	ARPGAIController();
};
