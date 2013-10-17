#include "../inc/UDP.h"
#include "../inc/MicroControllerCommunication.h"
#include <stdio.h>
#include <string.h>


static pthread_mutex_t S_mutex;
static pthread_cond_t S_cond;



static char responseBuffer[100];
static void receiveCallback(char* a, unsigned int b);
void MicroControllerCommunication::SendGetVersion(void){
   char command[4] = {0,0,0,1};
   udp->WriteData(socket, command,4);



}
void MicroControllerCommunication::SendDeliverIceCube(char amount){
    char command[5] = {0,0,0,3,4};
    command[4] = amount;
    udp->WriteData(socket, command,5);
    char expected [5] = {0,0,0,4,0};
    pthread_mutex_lock(&S_mutex);
    pthread_cond_wait(&S_cond,&S_mutex);
    printf("response %d\n", memcmp(expected,responseBuffer, 5));
    pthread_mutex_unlock(&S_mutex);
}
void MicroControllerCommunication::SendSetLEDtext(char* text){
    char command[4] = {0,0,0,4};
    udp->WriteData(socket, command,4);
}
void MicroControllerCommunication::SendLEDControl(char controlRegister){

}
void MicroControllerCommunication::SendGetMainStatus(void){

}

static void receiveCallback(char* a, unsigned int b){
    printf("Callback receiving: %s, len = %d\n", a,b);
    pthread_mutex_lock(&S_mutex);
    memcpy(responseBuffer, a, b);
    pthread_cond_signal(&S_cond);
    pthread_mutex_unlock(&S_mutex);
}

MicroControllerCommunication::MicroControllerCommunication(){
    udp = new UDP();
    socket = udp->OpenSocket("192.168.1.30", 777, &receiveCallback);
}

MicroControllerCommunication::~MicroControllerCommunication(){
    delete(udp);
}

