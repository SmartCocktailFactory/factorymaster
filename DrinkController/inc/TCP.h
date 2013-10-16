#ifndef _TCP_
#define _TCP_

/************************************************************
    Includes
*************************************************************/
#include <pthread.h>
#include "SCFSocket.h"

class TCP : public SCFSocket
{
 public:
  virtual ~TCP( );

  /** Function opens the socket and listens on the port.
      @return The socketHandler.*/
  int OpenSocket(std::string ip, int port, ReceiveDataCallback_t receiveCallback);

  /** Write data to socket **/
  bool WriteData(int socketHandler, const char* pData, unsigned int numberOfData);

  /** Wait untill the connection is established. */
  void WaitUntillConnected( );

 private:
    /* Variables */
  pthread_t threadId;

};

#endif
