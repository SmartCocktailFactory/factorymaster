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

typedef struct
{
  std::string ip;
  int port;
  ReceiveDataCallback_t receiveCallback;
  int socketFileDescriptor;
  struct sockaddr_in from;

} UDPWorkingData_t;

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


bool UDP::WriteData(int socket_d, const char* pData, unsigned int numberOfData)
{
  bool ret = false;
  printf("Writing, handler %d, data %x, size %d\n",socket_d, *pData, numberOfData);
  if(-1 == sendto(socket_d,pData,numberOfData,0,(const struct sockaddr *)&server,sizeof(struct sockaddr_in))){    
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
  //struct sockaddr_in addr;
  char buffer[1500];
  int numberOfBytesRead;
  socklen_t length; 
  for (;;)
    {
      //numberOfBytesRead = read( pData->socketFileDescriptor, buffer, 1500 );
        errno = 0;
        numberOfBytesRead = recvfrom(pData->socketFileDescriptor, buffer, 1500, 0, (struct sockaddr*)&(pData->from), &length);
        printf("length %d\n", numberOfBytesRead);
       if (errno)
       {
           printf("could not read from UDP socket\n %s\n", strerror(errno));
           fflush(stdout);
       }
       else{
        pData->receiveCallback(buffer, numberOfBytesRead);
       }
       sleep(1u); 
    }
  return NULL;
}

