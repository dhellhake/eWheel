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
#define RECEIVE_BUFFER_SIZE 64

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

enum class BLECmd : uint8_t
{
	INVALID = 0,
	ACK_CMD = 1,
	
	SET_FLASHPAGE = 2,
	REQUEST_FLASHPAGE = 3,
	RESPONSE_FLASHPAGE = 3,
	
	SET_DRIVECONFIG = 63,
	REQUEST_DRIVECONFIG = 64,
	RESPONSE_DRIVECONFIG = 65
};

typedef struct 
{
	BLESwc SrcSwc;
	BLESwc DstSwc;
	uint16_t Sequence;
	uint16_t Length;
	uint8_t Flags;
	BLECmd Cmd;
	uint32_t CRC32;
	uint8_t Data[RECEIVE_BUFFER_SIZE];
} BLEPacket;


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
		BLEPacket ReceivedPacket;
		
		bool ClearToSend = true;
		BLECmd Command;
		BLESwc SourceSwc;
		BLESwc DestinationSwc;
		uint16_t BytesToSend = 0;
		
		uint16_t SendBufferIndex = 0;
		uint8_t SendBuffer[SEND_BUFFER_SIZE];
		
		void SendACK(BLESwc sSwc, BLESwc dSwc);
		void SendDataPacket(uint8_t cmd, uint8_t *data, uint16_t length, uint16_t sequence, BLESwc sSwc, BLESwc dSwc, uint8_t flags);
		void ReceivedDataPacket(BLEPacket *pkt);
		
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
		
		void SendData(BLECmd cmd, uint8_t *data, uint16_t length, BLESwc sSwc, BLESwc dSwc);
};
extern CC41A BLEDevice;

#endif /* CC41A_H_ */