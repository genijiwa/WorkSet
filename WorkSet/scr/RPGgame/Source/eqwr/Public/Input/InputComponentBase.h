// why

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "InputConfig.h"
#include "InputComponentBase.generated.h"

/**
 * 
 */
UCLASS()
class EQWR_API UInputComponentBase : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	

	template<class UserClass , typename PressFuncT , typename ReleaseFuncT , typename HoldFuncT >
	void BindActionAbilities(const UInputConfig* InputConfig,UserClass* Object , PressFuncT PressFunc, ReleaseFuncT ReleaseFunc, HoldFuncT HoldFunc);
};

template<class UserClass , typename PressFuncT , typename ReleaseFuncT , typename HoldFuncT >
	void UInputComponentBase::BindActionAbilities(const UInputConfig* InputConfig,UserClass* Object ,
		PressFuncT PressFunc, ReleaseFuncT ReleaseFunc, HoldFuncT HoldFunc)
{
	check(InputConfig);

	for (auto Config : InputConfig->AbilityInputActions)
	{
		if (Config.InputAction && Config.InputTag.IsValid())
		{
			if (PressFunc){BindAction(Config.InputAction,ETriggerEvent::Started,Object,PressFunc,Config.InputTag);}

			if (HoldFunc){BindAction(Config.InputAction,ETriggerEvent::Triggered,Object,HoldFunc,Config.InputTag);}

			if (ReleaseFunc){BindAction(Config.InputAction,ETriggerEvent::Completed,Object,ReleaseFunc,Config.InputTag);}
		}
	}
}//input SETTING  playercontrooler apply.


