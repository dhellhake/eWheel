/* 
* CC41A.h
*
* Created: 09.03.2019 18:06:43
* Author: Dominik Hellhake
*/
#ifndef __CC41A_H__
#define __CC41A_H__

#include "samc21.h"
#include "..\LowLevel\CC41A\CC41A.h"
#include "..\Executable.h"
#include "..\Utilities.h"

enum class DEBUG_CMD
{
	SetDriveConfig = 63,
	GetDriveConfig = 64,
	
	SetLED = 65
};

enum class ComPortCMD : uint8_t
{
	INVALID = 0,
	ACK_CMD = 1,
	READ_FLASHPAGE = 2,
	WRITE_FLASHPAGE =3
};

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
		
		void ReadFlashPage(uint16_t pageIndex);
		void WriteFlashPage(uint16_t pageIndex, uint8_t *data);
		
		void ReadDriveConfig(uint32_t timeStamp);
		void WriteDriveConfig(uint8_t *data);
		
	public:
		COMPort();
		
		bool PacketReceived = false;
		uint8_t ReceiveBuffer[536];
		
}; //CC41A

extern COMPort ComLink;

#endif //__CC41A_H__
