/*
 * CC41A.h
 *
 * Created: 29.03.2020 16:57:19
 *  Author: Dominik Hellhake
 */
#ifndef CC41A_H_
#define CC41A_H_

#include "samc21.h"
#include "..\..\Utilities.h"

#define RECEIVE_BUFFER_SIZE 128

typedef struct 
{
	uint16_t _type;
	uint16_t _length;
	uint8_t _data[64 + 4];
} DataPacket;

class CC41A
{	
	private:	
		uint32_t ReceiveBufferIndex = 0;
		uint8_t ReceiveBuffer[RECEIVE_BUFFER_SIZE];
	
	public:	
		CC41A();
		void ReceiveByte(uint8_t data);
		void SendDataPacket(DataPacket *pkg);
		
		void ReceivedDataPacket(uint8_t *data);
};
extern CC41A BLEDevice;

#endif /* CC41A_H_ */