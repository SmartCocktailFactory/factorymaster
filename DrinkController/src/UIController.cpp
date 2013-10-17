/************************************************************
    Includes
*************************************************************/
#include "../inc/UIController.h"
#include "../inc/HueCom.h"
#include <unistd.h>
#include <stdio.h>

/************************************************************
    Local types
*************************************************************/

/************************************************************
     Local variables 
*************************************************************/
UIController* UIController::instanz = NULL;

/************************************************************
    Local function prototypes
*************************************************************/

/************************************************************
    Public functions 
*************************************************************/
UIController::UIController( )
{
    globalState = E_UIState_Unknown;
    pHue = new HueCom( );
    pHue->init( );
}

void UIController::UpdateGlobalState(UIGlobalState_e newState)
{
    if (globalState != newState)
    {
        switch (newState)
        {
            case E_UIState_Unknown:
                UpdateAllLamps( E_HueColor_Purple, 255, 50 );
                break;
                
            case E_UIState_Alarm:
                UpdateAllLamps( E_HueColor_Red, 255, 50 );
                break;
                
            case E_UIState_Delivering:
                UpdateAllLamps( E_HueColor_Yellow, 255, 50 );
                break;
                
            case E_UIState_DeliveringDone:
                UpdateAllLamps( E_HueColor_Green, 255, 50 );
                break;
            
            case E_UIState_WaitingForOrder:
                UpdateAllLamps( E_HueColor_Cyan, 255, 50 );
                break;
            
            case E_UIState_Starting:
                UpdateAllLamps( E_HueColor_Blue, 255, 50 );
                
                break;
            case E_UIState_GlassMissing:
                UpdateAllLamps( E_HueColor_Orange, 255, 50 );
                break;
        }
        globalState = newState;
    }
}

void UIController::UpdateBottleState(LiquidDeliverySystemIndex_e index, UIBottleState_e newState)
{
    if (AllowedUpdateBottleState( ))
    {
        
    }
}
    
/************************************************************
    Local functions
*************************************************************/
bool UIController::AllowedUpdateBottleState( )
{
    bool ret = false;
    if (globalState == E_UIState_Delivering)
    {
        ret = true;
    }
    return ret;
}

void UIController::UpdateLamp(LiquidDeliverySystemIndex_e index, HueColor_e color,
                              int saturation, int brightness)
{
    pHue->writeHUE( (int)index, (int) color, saturation, brightness );
}
   
void UIController::UpdateAllLamps(HueColor_e color, int saturation, int brightness)
{
    for (int i = (int)E_LiquidDeliverySystemIndex_1;
         i < (int)E_LiquidDeliverySystemIndex_Invalid; i++)
    {
        UpdateLamp( (LiquidDeliverySystemIndex_e)(i + 1),
                    color, saturation, brightness );
    } 
}
