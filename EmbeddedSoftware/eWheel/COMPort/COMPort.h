/* 
* CC41A.h
*
* Created: 09.03.2019 18:06:43
* Author: Dominik Hellhake
*/
#ifndef __CC41A_H__
#define __CC41A_H__

#include "samc21.h"
#include "..\Executable.h"
#include "..\Utilities.h"

enum class DEBUG_CMD
{
	SetDriveConfig = 63,
	GetDriveConfig = 64,
	
	SetLED = 65
};

typedef struct {
	PackageType _type;
	uint32_t _timeStamp;
	uint8_t _length;
	uint8_t* _data;
} DataPackage;

#define RECEIVE_BUFFER_SIZE 1024

class COMPort : public Executable
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual RUN_RESULT Run(uint32_t timeStamp);

	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	private:
		uint32_t LastReported;
		
		uint32_t ReceiveBufferIndex = 0;
		uint8_t ReceiveBuffer[RECEIVE_BUFFER_SIZE];
		
		void ReadDriveConfig(uint32_t timeStamp);
		void WriteDriveConfig(uint8_t *data);
		
		void SendESCTrace(uint32_t timeStamp);
		void SendChassisTrace(uint32_t timeStamp);
		void SendDriveTrace(uint32_t timeStamp);
	public:
		COMPort();
		void ReceiveByte(uint8_t data);
		void SendDataPackage(DataPackage *pkg);
}; //CC41A

extern COMPort Bluetooth;

#endif //__CC41A_H__
