#ifndef _SCF_SOCKET_H_
#define _SCF_SOCKET_H_

/************************************************************
    Includes
*************************************************************/
#include <string>

/************************************************************
    Types
*************************************************************/
typedef void (*ReceiveDataCallback_t)(char* pData, unsigned int numberOfData);

/************************************************************
    Class definition
*************************************************************/

class SCFSocket
{
 public:
    virtual ~SCFSocket() {};

    /** Function opens the socket and listens on the port.
        @return The socketHandler.*/
    virtual int OpenSocket(std::string ip, int port, ReceiveDataCallback_t receiveCallback) = 0;

    /** Write data to socket **/
    virtual bool WriteData(int socketHandler, const char* pData, unsigned int numberOfData) = 0;
};

#endif
