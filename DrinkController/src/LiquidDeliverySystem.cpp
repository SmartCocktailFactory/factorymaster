/************************************************************
    Includes
*************************************************************/
#include "../inc/LiquidDeliverySystem.h"
#include "../inc/SPSConverter.h"
#include <unistd.h>
#include <stdio.h>

/************************************************************
    Local types
*************************************************************/

/************************************************************
     Local variables 
*************************************************************/
static pthread_mutex_t S_count_mutex;
static pthread_cond_t S_count_threshold_cv;
static unsigned int S_waitingMethod;
static unsigned int S_waitingKey;
static unsigned int S_waitingValue;

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
    pSpsConnection->WaitUntillConnected( );
    WaitForGlass();
    UpdateStationId( stationId );
    UpdateVolume( volumeToDeliverInMl );
    StartDeliver( );
}

void LiquidDeliverySystem::WaitForGlass()
{
    DWORD status = GetStatus();
    
    while (status != 1u /*Ready*/)
    {
        fprintf( stderr, "No Glass available" );
        if (status == 4u /* Error */)
        {
            AckError();
        }
        status = GetStatus();
        sleep( 1u );
    }
    fprintf( stderr, "Glass available" );
}

void LiquidDeliverySystem::WaitClassRemoved( )
{
    DWORD status = GetStatus();
    
    while (status != 0u /*Not Ready*/)
    {
        fprintf( stderr, "Please remove the glass" );
        if (status == 4u /* Error */)
        {
            AckError();
        }
        status = GetStatus();
        sleep( 1u );
    }
    fprintf( stderr, "Glass available" );
}

bool LiquidDeliverySystem::CheckDeliveryDoneSuccessfull( )
{
    DWORD status = GetStatus();
    bool ret = false;
    while ((status != 1u /*Ready*/) && (status != 4u /*Error*/))
    {
        status = GetStatus();
    }
    if (status == 4u)
    {
        ret = false;
    }
    return ret;
}

void LiquidDeliverySystem::AckError()
{
    SCF_Frame frame;
    SPSConverter sps;
    sps.ConvertIntoSendCommand( METHOD_PUT, KEY_Command, CMD_ERROR_ACK, &frame );
    pSpsConnection->WriteData( spsHandlerId, (char*) &frame, sizeof(frame) );
    fprintf( stderr, "Ack error... len:%lu\n", sizeof(frame) );
    WaitForCmdAccepted( METHOD_RESPONSE, KEY_Command );
}

void LiquidDeliverySystem::StartDeliver( )
{
    SCF_Frame frame;
    SPSConverter sps;
    sps.ConvertIntoSendCommand( METHOD_PUT, KEY_Command, CMD_DELIVER, &frame );
    pSpsConnection->WriteData( spsHandlerId, (char*) &frame, sizeof(frame) );
    fprintf( stderr, "Start deliver... len:%lu\n", sizeof(frame) );
    WaitForCmdAccepted( METHOD_RESPONSE, KEY_Command );
}

unsigned int LiquidDeliverySystem::GetStatus()
{
    SCF_Frame frame;
    SPSConverter sps;
    sps.ConvertIntoSendCommand( METHOD_GET, KEY_Status, 0, &frame );
    pSpsConnection->WriteData( spsHandlerId, (char*) &frame, sizeof(frame) );
    fprintf( stderr, "Get status len:%lu\n", sizeof(frame) );
    WaitForCmdAccepted( METHOD_RESPONSE, KEY_Status );
    return (unsigned int)S_waitingValue;
}

void LiquidDeliverySystem::PrepareforNextTransfer( )
{
    SCF_Frame frame;
    SPSConverter sps;
    sps.ConvertIntoSendCommand( METHOD_PUT, KEY_Command, CMD_PREPARE_FOR_NEXT_DRINK, &frame );
    pSpsConnection->WriteData( spsHandlerId, (char*) &frame, sizeof(frame) );
    fprintf( stderr, "Prepare for next transfer len:%lu\n", sizeof(frame) );
    WaitForCmdAccepted( METHOD_RESPONSE, KEY_Command );
}

void LiquidDeliverySystem::UpdateStationId(LiquidDeliverySystemIndex_e id)
{
    SCF_Frame frame;
    SPSConverter sps;
    sps.ConvertIntoSendCommand( METHOD_PUT, KEY_StationID, id + 1, &frame );
    pSpsConnection->WriteData( spsHandlerId, (char*) &frame, sizeof(frame) );
    fprintf( stderr, "Set station len:%lu\n", sizeof(frame) );
    WaitForCmdAccepted( METHOD_RESPONSE, KEY_StationID );
}

void LiquidDeliverySystem::UpdateVolume(unsigned int volumeToDeliverInMl)
{
    SCF_Frame frame;
    SPSConverter sps;
    sps.ConvertIntoSendCommand( METHOD_PUT, KEY_Volume, volumeToDeliverInMl, &frame );
    pSpsConnection->WriteData( spsHandlerId, (char*) &frame, sizeof(frame) );
    fprintf( stderr, "Set volume len:%lu\n", sizeof(frame) );
    WaitForCmdAccepted( METHOD_RESPONSE, KEY_Volume );
}

bool LiquidDeliverySystem::WaitForCmdAccepted(unsigned int method, unsigned int key)
{ 
    bool wait = true;
    while (wait)
    {
        pthread_mutex_lock( &S_count_mutex );
        pthread_cond_wait( &S_count_threshold_cv, &S_count_mutex );
        if ((S_waitingKey == key) && (S_waitingMethod == method))
        {
            wait = false;
        }
        pthread_mutex_unlock( &S_count_mutex );
    }
    return true;
}

/************************************************************
    Local functions
*************************************************************/
static void TcpReceiveHandler(char* pData, unsigned int numberOfData)
{
    SPSConverter sps;

    pthread_mutex_lock( &S_count_mutex );
    sps.ConvertReceivedData( &S_waitingMethod, &S_waitingKey, &S_waitingValue, pData );
    printf("server response : %u %u %u\n", S_waitingMethod, S_waitingKey, S_waitingValue );
    pthread_cond_signal( &S_count_threshold_cv );
    pthread_mutex_unlock( &S_count_mutex );
}
