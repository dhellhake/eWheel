/* 
* LSM9D.cpp
*
* Created: 26.06.2018 23:08:02
* Author: dominik hellhake
*/


#include "LSM9D.h"

// static class member
float LSM9D::Yaw = 0;
float LSM9D::Pitch = 0;
float LSM9D::Roll = 0;

void LSM9D::Init()
{
	//Init SPI-SERCOM interface
	SPIPort::InitSERCOM();
	
	//Configure CS-Pin as Output
	PORT->Group[0].DIRSET.reg = PORT_PA12;
	PORT->Group[0].OUTSET.reg = PORT_PA12;
	
	uint8_t resp = LSM9D::ReadRegister(0x0F);
}

void LSM9D::Update()
{
	//int16_t accX = LSM9D::ReadRegisterWord(LSM6DS3_OUTX_L_XL);
	//int16_t accY = LSM9D::ReadRegisterWord(LSM6DS3_OUTY_L_XL);
	//int16_t accZ = LSM9D::ReadRegisterWord(LSM6DS3_OUTZ_L_XL);
	
	//LSM9D::Pitch = (atan2(accX, sqrt(accY * accY + accZ * accZ)) * 180.0) / M_PI;
	//LSM9D::Roll = (atan2(-accY, accZ) * 180.0) / M_PI;
	//LSM6D::Yaw = 180 * atan(accZ / sqrt(accX * accX + accZ * accZ)) / M_PI;
}

uint8_t LSM9D::ReadRegister(uint8_t address)
{
	uint8_t result = 0x00;
	//CS Low
	PORT->Group[0].OUTCLR.reg = PORT_PA12;
	
	SPIPort::TransmitByte(address | 0x80);
	result = SPIPort::TransmitByte(0x00);
	
	//CS High
	PORT->Group[0].OUTSET.reg = PORT_PA12;
	
	return result;
}

int16_t LSM9D::ReadRegisterWord(uint8_t address)
{
	uint8_t result[2];
	//CS Low
	PORT->Group[0].OUTCLR.reg = PORT_PA12;
	
	SPIPort::TransmitByte(address | 0x80);
	result[0] = SPIPort::TransmitByte(0x00);
	result[1] = SPIPort::TransmitByte(0x00);
	
	//CS High
	PORT->Group[0].OUTSET.reg = PORT_PA12;
	
	return (int16_t)result[0] | int16_t(result[1] << 8);
}

uint8_t LSM9D::WriteRegister(uint8_t address, uint8_t data)
{
	uint8_t result = 0x00;
	//CS Low
	PORT->Group[0].OUTCLR.reg = PORT_PA12;
	
	SPIPort::TransmitByte(address & 0x7F);
	result = SPIPort::TransmitByte(data);
	
	//CS High
	PORT->Group[0].OUTSET.reg = PORT_PA12;
	
	return result;
}