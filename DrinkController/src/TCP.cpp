/************************************************************
    Includes
*************************************************************/
#include "../inc/TCP.h"
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

/************************************************************
    Local types
*************************************************************/
typedef struct
{
  std::string ip;
  int port;
  ReceiveDataCallback_t receiveCallback;
  int socketFileDescriptor;
  bool connected;
} TCPWorkingData_t;

/************************************************************
     Local variables 
*************************************************************/
TCPWorkingData_t S_tcpWorkingData;
pthread_mutex_t S_threadMutex;

/************************************************************
    Local function prototypes
*************************************************************/
static void* TCPReceiveHandler(void* pArg);

/************************************************************
    Public functions 
*************************************************************/
int TCP::OpenSocket(std::string ip, int port, ReceiveDataCallback_t receiveCallback)
{
  int socketFileDescriptor = socket( AF_INET, SOCK_STREAM, 0);
  pthread_mutex_init( &S_threadMutex, NULL );

  S_tcpWorkingData.ip = ip;
  S_tcpWorkingData.port = port;
  S_tcpWorkingData.receiveCallback = receiveCallback;
  S_tcpWorkingData.socketFileDescriptor = socketFileDescriptor;
  S_tcpWorkingData.connected = false;

  if (pthread_create( &threadId,
		      NULL,
		      &TCPReceiveHandler,
		      (void*)&S_tcpWorkingData ) != 0)
    {
      fprintf( stderr, "Konnte Thread nicht erzeugen\n" );
    }
  return socketFileDescriptor;
}

bool TCP::WriteData(int socketHandler, const char* pData, unsigned int numberOfData)
{
  bool isConnected = false;
  bool ret = false;
  pthread_mutex_lock( &S_threadMutex );
  isConnected = S_tcpWorkingData.connected;
  pthread_mutex_unlock( &S_threadMutex );

  if (isConnected)
    {
      write( socketHandler, pData, numberOfData );
    }
  return ret;
}

/************************************************************
    Local functions
*************************************************************/
static void* TCPReceiveHandler(void* pArg)
{
  TCPWorkingData_t* pData = (TCPWorkingData_t*) pArg;
  struct sockaddr_in addr;
  char buffer[1500];
  int numberOfBytesRead;

  bzero( (char *) &addr, sizeof( addr ) );
  addr.sin_family = AF_INET;
  addr.sin_port = htons( pData->port );
  bcopy( (char *)pData->ip.c_str(),
	 (char *)&addr.sin_addr.s_addr,
	 pData->ip.length( ) );
  
  /* Wait for the server to connect */
  while (connect( pData->socketFileDescriptor, (struct sockaddr *) &addr, sizeof(addr) ) == -1)
    {
      fprintf( stderr, "Waiting for server\n" );
      sleep( 1u );
    }

  pthread_mutex_lock( &S_threadMutex );
  pData->connected = true;
  pthread_mutex_unlock( &S_threadMutex );

  for (;;)
    {
      numberOfBytesRead = read( pData->socketFileDescriptor, buffer, 1500 );
      if ((numberOfBytesRead != 0) && (pData->receiveCallback != NULL))
	{
	  pData->receiveCallback( buffer, numberOfBytesRead );
	} 
    }
  return NULL;
}

