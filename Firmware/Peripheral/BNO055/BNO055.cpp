/* 
* BNO055.cpp
*
* Created: 21.05.2020 17:29:12
* Author: dominik hellhake
*/
#include "BNO055.h"
#include "..\SAM4S\USART\USARTlib.h"
#include "..\System\System.h"

BNO055 IMU;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT BNO055::Run(uint32_t timeStamp)
{
	RUN_RESULT result = RUN_RESULT::IDLE;
		
	this->ReadRegister(BNO_REG_ADR_0::ACC_DATA_X_LSB, 46);
	this->ComState = BNO_COM_STATE::UPDATE_DATA;
	
	return result;
}

RUN_RESULT BNO055::Setup(uint32_t timeStamp)
{
	uint8_t data [1] { 0x08 };
	
	data[0] = 0x06;
	this->WriteRegister(BNO_REG_ADR_0::AXIS_MAP_CONFIG, 1, data);
	
	data[0] = 0x02;
	this->WriteRegister(BNO_REG_ADR_0::AXIS_MAP_SIGN, 1, data);
	
	data[0] = 0x08;
	this->WriteRegister(BNO_REG_ADR_0::OPR_MODE, 1, data);
	
	
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
BNO055::BNO055()
{
}

void BNO055::ReceivedAcknowledge(BNO_COM_ACK ackStatus)
{		
	switch(this->ComState)
	{
		default:
			this->ComState = BNO_COM_STATE::IDLE;
		break;
	}
}

void BNO055::ReceivedResponse(uint8_t length, uint8_t *data)
{	
	switch(this->ComState)
	{
		case BNO_COM_STATE::UPDATE_DATA:
			this->Avl_AccX = ((int16_t)(((int16_t)data[1] << 8) | data[0])) / 100.0;
			this->Avl_AccY = ((int16_t)(((int16_t)data[3] << 8) | data[2])) / 100.0;
			this->Avl_AccZ = ((int16_t)(((int16_t)data[5] << 8) | data[4])) / 100.0;
			this->Avl_GyroX = ((int16_t)(((int16_t)data[13] << 8) | data[12])) / 16.0;
			this->Avl_GyroY = ((int16_t)(((int16_t)data[15] << 8) | data[14])) / 16.0;
			this->Avl_GyroZ = ((int16_t)(((int16_t)data[17] << 8) | data[16])) / 16.0;
			this->Avl_Yaw = ((int16_t)(((int16_t)data[19] << 8) | data[18])) / 16.0;
			this->Avl_Roll = ((int16_t)(((int16_t)data[21] << 8) | data[20])) / 16.0;
			this->Avl_Pitch = ((int16_t)(((int16_t)data[23] << 8) | data[22])) / 16.0;			
			this->Avl_LinAccX = ((int16_t)(((int16_t)data[33] << 8) | data[32])) / 100.0;
			this->Avl_LinAccY = ((int16_t)(((int16_t)data[35] << 8) | data[34])) / 100.0;
			this->Avl_LinAccZ = ((int16_t)(((int16_t)data[37] << 8) | data[36])) / 100.0;
			this->Avl_GravX = ((int16_t)(((int16_t)data[39] << 8) | data[38])) / 100.0;
			this->Avl_GravY = ((int16_t)(((int16_t)data[41] << 8) | data[40])) / 100.0;
			this->Avl_GravZ = ((int16_t)(((int16_t)data[43] << 8) | data[42])) / 100.0;
			this->Avl_Temp_IMU = (int8_t)data[44];
			this->ST_Calib_IMU = data[45];
		break;
		default:
			this->ComState = BNO_COM_STATE::IDLE;
		break;
	}
}

void BNO055::ReceiveByte(uint8_t data)
{
	this->ReceiveBuffer[this->ReceiveBufferIndex++] = data;
	
	switch (this->ReceiveBuffer[0])
	{
		case 0xEE:	//Read/Write Acknowledge
			if (this->ReceiveBufferIndex >= 2)
			{
				this->ReceivedAcknowledge((BNO_COM_ACK)this->ReceiveBuffer[1]);				
				this->ReceiveBufferIndex = 0;
			}
		break;
		case 0xBB:	// Read Response
			if (this->ReceiveBufferIndex >= 2)
				if (this->ReceiveBufferIndex >= (this->ReceiveBuffer[1] + 2))
				{
					this->ReceivedResponse(this->ReceiveBuffer[1], &this->ReceiveBuffer[2]);
					this->ReceiveBufferIndex = 0;
				}
		break;
		default:	// Buffer misalignment
			this->ReceiveBufferIndex = 0;
		break;
	}
}


void BNO055::ReadRegister(BNO_REG_ADR_0 addr, uint8_t length)
{
	uint8_t buffer[4] = {
		0xAA,
		0x01,
		(uint8_t)addr,
		length
	};
	
	for (uint8_t x = 0; x < 4; x++)
		USART_SendByte(BNO_USART, buffer[x]);
}

void BNO055::WriteRegister(BNO_REG_ADR_0 addr, uint8_t length, uint8_t *data)
{
	length = 1;
	uint8_t buffer[4] = {
		0xAA,
		0x00,
		(uint8_t)addr,
		length
	};
	
	for (uint8_t x = 0; x < 4; x++)
		USART_SendByte(BNO_USART, buffer[x]);
	for (uint8_t x = 0; x < length; x++)
		USART_SendByte(BNO_USART, data[x]);
}
