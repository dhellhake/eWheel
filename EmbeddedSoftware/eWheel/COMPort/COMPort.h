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
		
		void ResponseFlashPage(uint16_t pageIndex);
		void SetFlashPage(uint16_t pageIndex, uint8_t *data);
		
		void RequestDriveConfig();
		void WriteDriveConfig(uint8_t *data);
		
	public:
		COMPort();
		
		BLECmd ReceivedCommand = BLECmd::INVALID;
		uint8_t ReceiveBuffer[536];
		
}; //CC41A

extern COMPort ComLink;

#endif //__CC41A_H__
