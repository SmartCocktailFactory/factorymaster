/************************************************************
    Includes
*************************************************************/

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h> /* memset */
#include <unistd.h> /* close */
#include "../inc/SPSConverter.h"

/************************************************************
    Defines
*************************************************************/

#define MAGIC_NUMBER   0xBECF1234

/************************************************************
    Local types
*************************************************************/

/************************************************************
     Local variables 
*************************************************************/

/************************************************************
    Local function prototypes
*************************************************************/

DWORD SWAP( DWORD n ) 
{
	DWORD m = (((n) & 0xFF) << 24) +
        (((n) & 0xFF00) << 8) + (((n) & 0xFF0000) >> 8) + (((n) & 0xFF000000) >> 24);
	return m;
}

/************************************************************
    Public functions 
*************************************************************/

void SPSConverter::ConvertIntoSendCommand( DWORD method, DWORD key, DWORD value, SCF_Frame* pFrame )
{
    static DWORD nS = 0;
    memset( pFrame, 0, sizeof(SCF_Frame) );

    pFrame->data.dwMethod = method;
    pFrame->data.dwKey =  key;
    pFrame->data.dwValue = value;
  
    pFrame->header.dwHead = MAGIC_NUMBER;
    pFrame->header.cbLength = sizeof(pFrame->data);
    pFrame->header.nS = nS;
    pFrame->header.nR = 0;

    ConvertToRemote( pFrame );
    nS++;
}

void SPSConverter::ConvertReceivedData(DWORD *method, DWORD *key, DWORD *value, const char* pRecData)
{
    SCF_Frame*  pFrame;

    pFrame = (SCF_Frame*)pRecData;
    ConvertToLocal( pFrame );

    *method = pFrame->data.dwMethod;
    *key = pFrame->data.dwKey;
    *value = pFrame->data.dwValue;
}


/************************************************************
    Local functions
*************************************************************/

#define __LITTLE_ENDIAN__

#ifdef __LITTLE_ENDIAN__

void SPSConverter::ConvertToRemote( SCF_Frame* frame)
{
}

void SPSConverter::ConvertToLocal( SCF_Frame* frame)
{
}

#else

void SPSConverter::SwapFrame( SCF_Frame* frame)
{
    frame->data.dwMethod = SWAP(frame->data.dwMethod);
    frame->data.dwKey = SWAP(frame->data.dwKey);
    frame->data.dwValue = SWAP(frame->data.dwValue);
  
    frame->header.dwHead = SWAP(frame->header.dwHead);
    frame->header.cbLength = SWAP(frame->header.cbLength);
    frame->header.nS = SWAP(frame->header.nS);
    frame->header.nR = SWAP(frame->header.nR);
}

void SPSConverter::ConvertToLocal( SCF_Frame* frame)
{
    SwapFrame( frame );
}

void SPSConverter::ConvertToRemote( SCF_Frame* frame)
{
    SwapFrame( frame );
}

#endif
