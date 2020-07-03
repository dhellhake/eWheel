/*
 * CC41A.h
 *
 * Created: 29.03.2020 16:57:19
 *  Author: Dominik Hellhake
 */
#ifndef CC41A_H_
#define CC41A_H_

#include "sam.h"
#include "..\..\BLELink\BLELink.h"

#define CC41A_UART					UART0
#define CC41A_RCV_BUF_SIZE			64

class CC41A
{
	private:
		uint8_t ReceiveBufferIndex = 0;
		uint8_t ReceiveBuffer[CC41A_RCV_BUF_SIZE];
		
	public:	
		CC41A();
		void ReceiveByte(uint8_t data);
		void WriteMessage(BLE_CMD cmd, uint16_t length, uint8_t *data);
};
extern CC41A BLEPort;

#endif /* CC41A_H_ */