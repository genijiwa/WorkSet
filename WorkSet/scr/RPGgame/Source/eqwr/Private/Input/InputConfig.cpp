// why


#include "Input/InputConfig.h"

const UInputAction* UInputConfig::FindInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (auto Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp,Error,TEXT("no Action tag[%s] in Inputconfig[%s]"),*InputTag.ToString(),*GetNameSafe(this));
	}
	return nullptr;
}
