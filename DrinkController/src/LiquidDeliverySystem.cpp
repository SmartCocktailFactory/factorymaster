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
static pthread_mutex_t S_count_mutex;
static pthread_cond_t S_count_threshold_cv;

/************************************************************
    Local function prototypes
*************************************************************/
static void TcpReceiveHandler(char* pData, unsigned int numberOfData);

/************************************************************
    Public functions 
*************************************************************/
LiquidDeliverySystem::LiquidDeliverySystem(std::string ip, unsigned int port)
{
  pthread_mutex_init( &S_count_mutex, NULL );
  pthread_cond_init ( &S_count_threshold_cv, NULL );
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

bool LiquidDeliverySystem::CheckDeliveryDoneSuccessfull( )
{
   pthread_mutex_lock( &S_count_mutex );
   pthread_cond_wait( &S_count_threshold_cv, &S_count_mutex );
   pthread_mutex_unlock( &S_count_mutex );
   return true;
}
/************************************************************
    Local functions
*************************************************************/
static void TcpReceiveHandler(char* pData, unsigned int numberOfData)
{
  fprintf( stderr, "Received: num:%u %s\n", numberOfData, pData );
   pthread_mutex_lock( &S_count_mutex );
   pthread_cond_signal( &S_count_threshold_cv );
   pthread_mutex_unlock( &S_count_mutex );
}
