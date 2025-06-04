// why


#include "UI/HUD/MyHUD.h"
#include "UI/Widget/MyUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"



UOverlayWidgetController* AMyHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
    if (OverlayWidgetController == nullptr) 
    {

        OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
        OverlayWidgetController->SetWdigetControllerParams(WCParams);
    }
    return OverlayWidgetController;
}

UAttributeMenuWidgetController* AMyHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
    if (AttributeMenuWidgetController == nullptr) 
    {

        AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
        AttributeMenuWidgetController->SetWdigetControllerParams(WCParams);
    }
    AttributeMenuWidgetController->BindCallbacksToDependencies();
    return AttributeMenuWidgetController;
}

//把widget实例化 ，把controller实例化，并且都set好.
void AMyHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
    checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass δ���ã�����HUD��������"));
    checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass δ���ã�����HUD��������"));
    
    UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
    OverlayWidget = Cast<UMyUserWidget>(Widget);
//
    const FWidgetControllerParams WidgetControllerParams(PC,PS, ASC, AS);
    OverlayWidgetController = GetOverlayWidgetController(WidgetControllerParams);   

    OverlayWidget->SetWidgetController(OverlayWidgetController);//the widget set
    
    OverlayWidgetController->BroadcastInitialValues();
    OverlayWidgetController->BindCallbacksToDependencies();

    Widget->AddToViewport();
    
//MenuWidget
    //check(AttributeMenuWidgetControllerClass,TEXT("AttributeMenuWidgetControllerClass no define"));
    
}
