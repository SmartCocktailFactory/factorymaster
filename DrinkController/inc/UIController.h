#ifndef _UI_CONTROLLER_H_
#define _UI_CONTROLLER_H_

/************************************************************
    Includes
*************************************************************/
#include "../inc/LiquidDeliverySystem.h"
#include "../inc/HueCom.h"

/************************************************************
    Defines
*************************************************************/
#define MAX_NUMBER_LED_LAMPS (3u)

/************************************************************
    Types
*************************************************************/

typedef enum
    {
        E_UIState_Unknown,
        E_UIState_Starting,
        E_UIState_Alarm,
        E_UIState_Delivering,
        E_UIState_DeliveringDone,
        E_UIState_WaitingForOrder,
        E_UIState_GlassMissing,
    } UIGlobalState_e;


    typedef enum
    {
        E_UIState_Bottle_Unknown,
        E_UIState_Bottle_Empty,
        E_UIState_Bottle_DeliveringDone,
        E_UIState_Bottle_Delivering
    } UIBottleState_e;
    
/************************************************************
    Class definition
*************************************************************/

class UIController
{
 public:
    static UIController* GetInstance()
    {
        if (instanz == NULL)
        {
            instanz = new UIController( );
        }
        return instanz;
    }

    void UpdateGlobalState(UIGlobalState_e newState);
    void UpdateBottleState(LiquidDeliverySystemIndex_e index, UIBottleState_e newState);
    
 protected:
    UIController( );
        
 private:
    typedef enum
    {
        E_HueColor_Red = 0,
        E_HueColor_Cyan = 32767,
        E_HueColor_Yellow = 10922,
        E_HueColor_Green = 29120,
        E_HueColor_Blue = 43690,
        E_HueColor_Orange = 5500,
        E_HueColor_Purple = 60000
    } HueColor_e;
    
    static UIController *instanz;
    HueCom* pHue;
    UIGlobalState_e globalState;
    bool AllowedUpdateBottleState( );
    void UpdateLamp(LiquidDeliverySystemIndex_e index, HueColor_e color, int saturation, int brightness);
    void UpdateAllLamps(HueColor_e color, int saturation, int brightness);
};

#endif
