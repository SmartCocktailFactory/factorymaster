#ifndef _UDP_
#define _UDP_

/** Includes **/
#include <pthread.h>
#include "SCFSocket.h"
#include <netinet/in.h>
#include <sys/socket.h>
typedef struct
{
  std::string ip;
  int port;
  ReceiveDataCallback_t receiveCallback;
  int socketFileDescriptor;
  struct from;

} UDPWorkingData_t;



class UDP 
{
 public:
  /** Function opens the socket and listens on the port.
      @return The socketHandler.*/
  int OpenSocket(std::string ip, int port, ReceiveDataCallback_t receiveCallback);

  /** Write data to socket **/
  bool WriteData(const char* pData, unsigned int numberOfData);
  virtual ~UDP();
 private:
    /* Variables */
  pthread_t threadId;
  struct sockaddr_in server;

  int socket_d;
};

#endif
