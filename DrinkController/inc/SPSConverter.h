#ifndef _SPS_CONVERTER_H__
#define _SPS_CONVERTER_H_

/************************************************************
    Includes
*************************************************************/
#include <string>

/************************************************************
    Defines
*************************************************************/
#define DWORD unsigned int
#define	METHOD_GET	1
#define METHOD_PUT 2
#define METHOD_RESPONSE 3

#define KEY_Command 1
#define KEY_Volume	2
#define KEY_StationID	3
#define KEY_Status 4

#define CMD_NONE  0
#define CMD_DELIVER 1
#define CMD_ERROR_ACK 2
#define CMD_PREPARE_FOR_NEXT_DRINK 3

#define EVENT_NOT_READY 0
#define EVENT_READY 1
#define EVENT_PREFILLING 2
#define EVENT_DELIVER_LIQUID 3
#define EVENT_ERROR 4

/************************************************************
    Types
*************************************************************/
#pragma pack(1)
typedef struct
{	
	DWORD  dwMethod;		// 0=GET, 1=PUT
	DWORD  dwKey;			// 
	DWORD  dwValue;			// value
} SCF_Data;


typedef struct
{
	DWORD dwHead;
	DWORD cbLength;
	DWORD nS;
	DWORD nR;
} SCF_Header;

typedef struct
{
	SCF_Header header;
	SCF_Data   data;
} SCF_Frame;

#pragma pack()

/************************************************************
    Class definition
*************************************************************/

class SPSConverter
{
 public:
    void ConvertIntoSendCommand( DWORD method, DWORD key, DWORD value, SCF_Frame* pFrame );
    void ConvertReceivedData(DWORD *method, DWORD *key, DWORD *value, const char* pRecData);

 private:
    void ConvertToRemote( SCF_Frame* frame);
    void ConvertToLocal( SCF_Frame* frame);
#ifdef __LITTLE_ENDIAN__
#else
    void SwapFrame( SCF_Frame* frame);
#endif
};

#endif
