/************************************************************
    Includes
*************************************************************/
#include "../inc/UDP.h"
#include <sys/types.h> /* See NOTES */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <errno.h>
/************************************************************
    Local types
*************************************************************/
/************************************************************
     Local variables 
*************************************************************/
UDPWorkingData_t S_udpWorkingData;
static pthread_mutex_t S_threadMutex;

/************************************************************
    Local function prototypes
*************************************************************/
static void* UDPReceiveHandler(void* pArg);

/************************************************************
    Public functions 
*************************************************************/
UDP::~UDP(){
}

int UDP::OpenSocket(std::string ip, int port, ReceiveDataCallback_t receiveCallback)
{
  int socketFileDescriptor = socket( AF_INET, SOCK_DGRAM, 0);
  pthread_mutex_init( &S_threadMutex, NULL );

  S_udpWorkingData.ip = ip;
  S_udpWorkingData.port = port;
  S_udpWorkingData.receiveCallback = receiveCallback;
  S_udpWorkingData.socketFileDescriptor = socketFileDescriptor;

  if (pthread_create( &threadId,
		      NULL,
		      &UDPReceiveHandler,
		      (void*)&S_udpWorkingData ) != 0)
    {
      fprintf( stderr, "Konnte Thread nicht erzeugen\n" );
    }

   struct hostent *hp;
   server.sin_family = AF_INET;
   hp = gethostbyname(ip.c_str());
   if (hp==0) fprintf(stderr, "Unknown host");

   bcopy((char *)hp->h_addr, 
         (char *)&server.sin_addr,
         hp->h_length);
   server.sin_port = htons(port);
   
  return socketFileDescriptor;
}


bool UDP::WriteData(int socketHandler, const char* pData, unsigned int numberOfData)
{
  bool ret = false;
  printf("Writing, handler %d, data %x, size %d\n",socketHandler, *pData, numberOfData);
  if(-1 == sendto(socketHandler,pData,numberOfData,0,(const struct sockaddr *)&server,sizeof(struct sockaddr_in))){    
      fprintf(stderr, "Write failed %s\n", strerror(errno));
  }
  return ret;
}

/************************************************************
    Local functions
*************************************************************/
static void* UDPReceiveHandler(void* pArg)
{
  UDPWorkingData_t* pData = (UDPWorkingData_t*) pArg;
  struct sockaddr_in addr;
  char buffer[1500];
  int numberOfBytesRead;

  bzero( (char *) &addr, sizeof( addr ) );
  addr.sin_family = AF_INET;
  addr.sin_port = htons( pData->port );
  bcopy( (char *)pData->ip.c_str(),
	 (char *)&addr.sin_addr.s_addr,
	 pData->ip.length( ) );
  
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

