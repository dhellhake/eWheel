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
#include "..\..\Executable.h"

#define SEND_BUFFER_SIZE 536
#define RECEIVE_BUFFER_SIZE 128

enum class BLEPacketFlags : uint8_t
{
	ACK = 1,
	FIN = 2
};
enum class BLESwc : uint8_t
{
	INVALID = 0,
	SELF = 1,
	ComLink = 2
};


class CC41A : public Executable
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual RUN_RESULT Run(uint32_t timeStamp);

	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	private:	
		uint8_t ReceiveBufferIndex = 0;
		
		COMPILER_ALIGNED(4)
		uint8_t ReceiveBuffer[RECEIVE_BUFFER_SIZE];
		
		bool ClearToSend = true;
		BLESwc SourceSwc;
		BLESwc DestinationSwc;
		uint16_t BytesToSend = 0;
		uint16_t SendBufferIndex = 0;
		uint8_t SendBuffer[SEND_BUFFER_SIZE];
		
		void SendACK(BLESwc sSwc, BLESwc dSwc);
		void SendDataPacket(uint8_t *data, uint16_t length, uint16_t sequence, BLESwc sSwc, BLESwc dSwc, uint8_t flags);
		void ReceivedDataPacket(uint8_t *data);
		
		inline uint16_t GetCRC32Length(uint16_t length)
		{
			uint16_t result = length % 4;
			if (result != 0)
				return length + (4 - result);
			else
				return length;
		}
	public:	
		CC41A();
		void ReceiveByte(uint8_t data);
		
		void SendData(uint8_t *data, uint16_t length, BLESwc sSwc, BLESwc dSwc);
};
extern CC41A BLEDevice;

#endif /* CC41A_H_ */