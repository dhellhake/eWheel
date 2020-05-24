/* 
* BNO055.h
*
* Created: 21.05.2020 17:29:12
* Author: dominik hellhake
*/
#ifndef __BNO055_H__
#define __BNO055_H__

#include "sam.h"

#define BNO_USART			USART1
#define BNO_RCV_BUF_SIZE	128

typedef struct BNO_REG_VAL
{
	uint8_t opr_mode;
} BNO_REG_VAL;

enum class BNO_REG_ADR
{
	ACC_DATA_X_LSB = 0x08,
	
	OPR_MODE = 0x3D
};

enum class BNO_COM_ACK
{
		WRITE_SUCCESS = 0x01,
		READ_FAIL = 0x02,
		WRITE_FAIL = 0x03,
		REGMAP_INVALID_ADDRESS = 0x04,
		REGMAP_WRITE_DISABLED = 0x05,
		WRONG_START_BYTE = 0x06,
		BUS_OVER_RUN_ERROR = 0x07,
		MAX_LENGTH_ERROR = 0x08,
		MIN_LENGTH_ERROR = 0x09,
		RECEIVE_CHARACTER_TIMEOUT = 0x0A
};

enum class BNO_COM_STATE
{
	IDLE,
	UPDATE_DATA
};

enum class BNO_CONFIG_STATE
{
	RESET,
	READY
};

class BNO055
{
	private:
		uint8_t InterruptStatus = 0x00;
		
		BNO_REG_VAL Config;
		BNO_CONFIG_STATE ConfigState;
	
		BNO_COM_STATE ComState = BNO_COM_STATE::IDLE;
		uint8_t ReceiveBufferIndex = 0;
		uint8_t ReceiveBuffer[BNO_RCV_BUF_SIZE];
	
		void ConfigRoutine();
		
		
		void ReceivedAcknowledge(BNO_COM_ACK ackStatus);
		void ReceivedResponse(uint8_t length, uint8_t *data);
		
		void ReadRegister(BNO_REG_ADR addr, uint8_t length);
		void WriteRegister(BNO_REG_ADR addr, uint8_t length, uint8_t *data);
	public:
		float Yaw;
		float Roll;
		float Pitch;
		
		float AccX;
		float AccY;
		float AccZ;
		float GyroX;
		float GyroY;
		float GyroZ;
		
		
		BNO055();		
		void Update();
				
		void ReceiveByte(uint8_t data);
}; //BNO055

extern BNO055 IMU;

#endif //__BNO055_H__
