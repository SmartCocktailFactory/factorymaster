#ifndef _LIQUID_DELIVERY_SYSTEM_H_
#define _LIQUID_DELIVERY_SYSTEM_H_

/************************************************************
    Includes
*************************************************************/
#include <string>
#include "TCP.h"

/************************************************************
    Types
*************************************************************/
enum LiquidDeliverySystemIndex_e
    {
        E_LiquidDeliverySystemIndex_1,
        E_LiquidDeliverySystemIndex_2,
        E_LiquidDeliverySystemIndex_3,
        E_LiquidDeliverySystemIndex_Invalid
    };

/************************************************************
    Class definition
*************************************************************/

class LiquidDeliverySystem
{
 public:
    static LiquidDeliverySystem* GetInstance(std::string ip, unsigned int port)
    {
        if (instanz == NULL)
        {
            instanz = new LiquidDeliverySystem( ip, port );
        }
        return instanz;
    }

    void DeliverVolume(LiquidDeliverySystemIndex_e stationId, unsigned int volumeToDeliverInMl);
    bool CheckDeliveryDoneSuccessfull( );
    void WaitClassRemoved();
    void WaitForGlass();
    
 protected:
    LiquidDeliverySystem(std::string ip, unsigned int port);
        
 private:
    TCP* pSpsConnection;
    int spsHandlerId;
    static LiquidDeliverySystem *instanz;
    
    void UpdateStationId(LiquidDeliverySystemIndex_e id);
    void UpdateVolume(unsigned int volumeToDeliverInMl);
    bool WaitForCmdAccepted(unsigned int method, unsigned int key);
    void PrepareforNextTransfer( );
    void StartDeliver( );
    unsigned int GetStatus();
    void AckError();
};

#endif
