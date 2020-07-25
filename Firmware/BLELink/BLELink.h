/* 
* BLELink.h
*
* Created: 02.07.2020 17:56:12
* Author: dominik hellhake
*/
#ifndef __BLELINK_H__
#define __BLELINK_H__

#include "sam.h"
#include "..\Task.h"
#include "BLE.h"


#define BLE_RCV_BUF_SIZE	128

class BLELink : public Task
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual RUN_RESULT Run(uint32_t timeStamp);
	virtual RUN_RESULT Setup(uint32_t timeStamp);

	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:	
	uint8_t ReceiveBufferIndex = 0;
	uint8_t ReceiveBuffer[BLE_RCV_BUF_SIZE];
	
	private:
	BLE_STATE State;
	
	void ResponseIMUValSet();
	void EraseFlash();
}; //BLELink

extern BLELink BLE;

#endif //__BLELINK_H__
