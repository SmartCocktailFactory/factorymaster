#include "../inc/UDP.h"
#include "../inc/MicroControllerCommunication.h"
#include <stdio.h>
static void receiveCallback(char* a, unsigned int b);
void MicroControllerCommunication::SendGetVersion(void){
   char command[4] = {0,0,0,1};
   udp->WriteData(socket,command,4);
}
void MicroControllerCommunication::SendDeliverIceCube(char amount){
    char command[4] = {0,0,0,4};
    udp->WriteData(socket,command,4);
}
void MicroControllerCommunication::SendSetLEDtext(char* text){
    char command[4] = {0,0,0,4};
    udp->WriteData(socket,command,4);
   
}
void MicroControllerCommunication::SendLEDControl(char controlRegister){

}
void MicroControllerCommunication::SendGetMainStatus(void){

}

static void receiveCallback(char* a, unsigned int b){
    printf("Callback receiving: %s, len = %d\n", a,b);
}

MicroControllerCommunication::MicroControllerCommunication(){
    udp = new UDP();
    socket = udp->OpenSocket("192.168.1.40", 777, &receiveCallback);
}

MicroControllerCommunication::~MicroControllerCommunication(){
    delete(udp);

}






