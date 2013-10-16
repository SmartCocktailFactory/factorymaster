#include "../inc/LiquidDeliverySystem.h"
#include <stdio.h>

LiquidDeliverySystem::LiquidDeliverySystem(std::string ip, unsigned int port)
{

}

void LiquidDeliverySystem::DeliverVolume(LiquidDeliverySystemIndex_e stationId, 
					 unsigned int volumeToDeliverInMl)
{
  fprintf( stderr, "Station:%d DeliverVolume:%d \n", stationId, volumeToDeliverInMl );
}

