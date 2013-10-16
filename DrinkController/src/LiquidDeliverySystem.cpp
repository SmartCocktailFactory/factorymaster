/************************************************************
    Includes
*************************************************************/
#include "../inc/LiquidDeliverySystem.h"
#include <stdio.h>

/************************************************************
    Local types
*************************************************************/

/************************************************************
     Local variables 
*************************************************************/

/************************************************************
    Local function prototypes
*************************************************************/
static void TcpReceiveHandler(char* pData, unsigned int numberOfData);

/************************************************************
    Public functions 
*************************************************************/
LiquidDeliverySystem::LiquidDeliverySystem(std::string ip, unsigned int port)
{
  pSpsConnection = new TCP( );
  spsHandlerId = pSpsConnection->OpenSocket( ip, port, TcpReceiveHandler );
}

void LiquidDeliverySystem::DeliverVolume(LiquidDeliverySystemIndex_e stationId, 
					 unsigned int volumeToDeliverInMl)
{
  char buf[10];
  buf[0] = '1';
  buf[1] = '2';
  buf[2] = 0x00;
  fprintf( stderr, "Station:%d DeliverVolume:%d \n", stationId, volumeToDeliverInMl );
  pSpsConnection->WaitUntillConnected( );
  fprintf( stderr, "Want to send data\n" );
  pSpsConnection->WriteData( spsHandlerId, buf, 2u );
}

/************************************************************
    Local functions
*************************************************************/
static void TcpReceiveHandler(char* pData, unsigned int numberOfData)
{
  fprintf( stderr, "Received: num:%u %s\n", numberOfData, pData );
}
