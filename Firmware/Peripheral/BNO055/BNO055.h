/* 
* BNO055.h
*
* Created: 21.05.2020 17:29:12
* Author: dominik hellhake
*/
#ifndef __BNO055_H__
#define __BNO055_H__

#include "sam.h"
#include "..\..\Task.h"

#define BNO_USART			USART1
#define BNO_RCV_BUF_SIZE	128

enum class BNO_REG_ADR_0
{
	ACC_DATA_X_LSB = 0x08,
	TEMP = 0x35,
	
	AXIS_MAP_CONFIG = 0x41,
	
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


class BNO055 : public Task
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual RUN_RESULT Run(uint32_t timeStamp);
	virtual RUN_RESULT Setup(uint32_t timeStamp);

	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	private:
		uint8_t InterruptStatus = 0x00;
	
		BNO_COM_STATE ComState = BNO_COM_STATE::IDLE;
		uint8_t ReceiveBufferIndex = 0;
		uint8_t ReceiveBuffer[BNO_RCV_BUF_SIZE];		
		
		void ReceivedAcknowledge(BNO_COM_ACK ackStatus);
		void ReceivedResponse(uint8_t length, uint8_t *data);
		
		void ReadRegister(BNO_REG_ADR_0 addr, uint8_t length);
		void WriteRegister(BNO_REG_ADR_0 addr, uint8_t length, uint8_t *data);
	public:
		float Avl_Yaw;
		float Avl_Roll;
		float Avl_Pitch;
		
		float Avl_AccX;
		float Avl_AccY;
		float Avl_AccZ;
		float Avl_GyroX;
		float Avl_GyroY;
		float Avl_GyroZ;
		float Avl_LinAccX;
		float Avl_LinAccY;
		float Avl_LinAccZ;
		float Avl_GravX;
		float Avl_GravY;
		float Avl_GravZ;
		
		int8_t Avl_Temp_IMU;
		uint8_t ST_Calib_IMU;
		
		BNO055();
				
		void ReceiveByte(uint8_t data);
}; //BNO055

extern BNO055 IMU;

#endif //__BNO055_H__
