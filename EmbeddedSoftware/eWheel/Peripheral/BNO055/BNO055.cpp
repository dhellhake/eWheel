/* 
* BNO055.cpp
*
* Created: 21.05.2020 17:29:12
* Author: dominik hellhake
*/
#include "BNO055.h"
#include "..\SAM4S\USART\USARTlib.h"

BNO055 IMU;

BNO055::BNO055()
{
	this->Config.opr_mode = 0x08;
	
	this->ConfigState = BNO_CONFIG_STATE::RESET;
}

void BNO055::Update()
{
	if (this->ConfigState != BNO_CONFIG_STATE::READY)
	{
		this->ConfigRoutine();
		return;
	}
	
	this->ReadRegister(BNO_REG_ADR::ACC_DATA_X_LSB, 24);
	this->ComState = BNO_COM_STATE::UPDATE_DATA;
}

void BNO055::ConfigRoutine()
{
	uint8_t data [1] { 0x00 };
	
	data[0] = this->Config.opr_mode;
	this->WriteRegister(BNO_REG_ADR::OPR_MODE, 1, data);
	this->ConfigState = BNO_CONFIG_STATE::READY;	
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
			this->AccX = ((int16_t)(((int16_t)data[1] << 8) | data[0])) / 100.0;
			this->AccY = ((int16_t)(((int16_t)data[3] << 8) | data[2])) / 100.0;
			this->AccZ = ((int16_t)(((int16_t)data[5] << 8) | data[4])) / 100.0;
			this->GyroX = ((int16_t)(((int16_t)data[13] << 8) | data[12])) / 16.0;
			this->GyroY = ((int16_t)(((int16_t)data[15] << 8) | data[14])) / 16.0;
			this->GyroZ = ((int16_t)(((int16_t)data[17] << 8) | data[16])) / 16.0;
			this->Yaw = ((int16_t)(((int16_t)data[19] << 8) | data[18])) / 16.0;
			this->Roll = ((int16_t)(((int16_t)data[21] << 8) | data[20])) / 16.0;
			this->Pitch = ((int16_t)(((int16_t)data[23] << 8) | data[22])) / 16.0;
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


void BNO055::ReadRegister(BNO_REG_ADR addr, uint8_t length)
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

void BNO055::WriteRegister(BNO_REG_ADR addr, uint8_t length, uint8_t *data)
{
	length = 1;
	uint8_t buffer[5] = {
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
