// why


#include "UI/WidgetController/MyWidgetController.h"



void UMyWidgetController::SetWdigetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UMyWidgetController::BroadcastInitialValues()
{
}

void UMyWidgetController::BindCallbacksToDependencies()
{
}
