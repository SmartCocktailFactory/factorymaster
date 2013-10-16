#ifndef _LIQUID_DELIVERY_SYSTEM_H_
#define _LIQUID_DELIVERY_SYSTEM_H_

#include <string>
#include "TCP.h"

enum LiquidDeliverySystemIndex_e
  {
    E_LiquidDeliverySystemIndex_1,
    E_LiquidDeliverySystemIndex_2,
    E_LiquidDeliverySystemIndex_3,
    E_LiquidDeliverySystemIndex_Invalid
  };

class LiquidDeliverySystem
{
 public:
  LiquidDeliverySystem(std::string ip, unsigned int port);
  void DeliverVolume(LiquidDeliverySystemIndex_e stationId, unsigned int volumeToDeliverInMl);
  bool CheckDeliveryDoneSuccessfull( );

 private:
  TCP* pSpsConnection;
  int spsHandlerId;

  void UpdateStationId(LiquidDeliverySystemIndex_e id);
  void UpdateVolume(unsigned int volumeToDeliverInMl);
  bool WaitForCmdAccepted(unsigned int method, unsigned int key);
  void PrepareforNextTransfer( );
  void StartDeliver( );
  unsigned int GetStatus();
  void AckError();
  void WaitForGlass();
};

#endif
