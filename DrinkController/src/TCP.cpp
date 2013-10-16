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
#include <signal.h>
#include <errno.h>

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
static TCPWorkingData_t S_tcpWorkingData;
static pthread_mutex_t S_threadMutex;

/************************************************************
    Local function prototypes
*************************************************************/
static void* TCPReceiveHandler(void* pArg);

/************************************************************
    Public functions 
*************************************************************/
TCP::~TCP( )
{

}

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
      if (write( socketHandler, pData, numberOfData ) != -1)
	{
	  ret = true;
	}
      else
	{
	  fprintf( stderr, "Sending failed error:%s\n", strerror( errno ) );
	}
    }
  return ret;
}

void TCP::WaitUntillConnected( )
{
  bool isConnected = false;
  while (!isConnected)
    {
      pthread_mutex_lock( &S_threadMutex );
      isConnected = S_tcpWorkingData.connected;
      pthread_mutex_unlock( &S_threadMutex );
      if (!isConnected)
	{
	  sleep( 1u );
	}
    }
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
  int err = -1;
  struct hostent *server;

  bzero( (char *) &addr, sizeof( addr ) );
  addr.sin_family = AF_INET;
  addr.sin_port =  htons( pData->port );
  server = gethostbyname( pData->ip.c_str() );
  bcopy( (const char *)server->h_addr,
	 (char *)&addr.sin_addr.s_addr,
	 pData->ip.length( ) );
  
  /* Wait for the server to connect */
  fprintf( stderr, "Waiting for server:%s port:%u\n",(char *)&addr.sin_addr.s_addr,  addr.sin_port);
  
  while (err == -1)
    {
      err = connect( pData->socketFileDescriptor, (struct sockaddr *) &addr, sizeof(addr) );
      fprintf( stderr, "Waiting for server %d error:%s\n", err, strerror( errno ) );
      sleep( 1u );
    }
  
  fprintf( stderr, "Connected to server\n" );
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
      sleep( 1u );
    }
  return NULL;
}


