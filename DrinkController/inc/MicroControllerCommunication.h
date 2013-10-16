

#ifndef MICRO_CONTROLLER_CONTROLLER_H
#define MICRO_CONTROLLER_CONTROLLER_H
#include "../inc/UDP.h"

class MicroControllerCommunication
{     
    public:
        void SendGetVersion(void);
        void SendDeliverIceCube(char amount);
        void SendSetLEDtext(char* text);
        void SendLEDControl(char controlRegister);
        void SendGetMainStatus(void);

         MicroControllerCommunication();
         ~MicroControllerCommunication();

        UDP* udp;
        int socket;
    private:


};


#endif
