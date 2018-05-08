/* 
* LSM6D.cpp
*
* Created: 06.05.2018 15:52:49
* Author: domin
*/


#include "LSM6D.h"

// default constructor
LSM6D::LSM6D()
{
} //LSM6D

void LSM6D::Init()
{
	//Init SPI-SERCOM interface
	SPIPort::InitSERCOM();
		
	//Configure CS-Pin as Output
	PORT->Group[0].DIRSET.reg = PORT_PA09;
	PORT->Group[0].OUTSET.reg = PORT_PA09;
	
	
	//20ms boot time
	
	//INT1/2_CTRL->DRDY_G (DataReady on G subsequent to XL)
	
	//Accel
	//CTRL6_C->XL_HM_MODE = 0		Enable High-Performance Mode for Accel
	//CTRL1_XL->ODR_XL = 1000		1660Hz data rate for Accel
	//CTRL1_XL->BW_XL = 10			analog filter cutoff frequence = 100hz (00=400;01=200;10=100;11=50)
	//CTRL4_C->XL_BW_SCAL_ODR = 1   Select CTRL1_XL->BW_XL as source for analog filter bandwith
	
	//CTRL10_C->FUNC_EN = 0			disable embedded functions
	//TAP_CFG->SLOPE_FDS = 1		enable HP and LPF2 for Accel
	//CTRL8_XL->HP_SLOPE_XL_EN = 0	disable Slope filter
	//CTRL8_XL->LPF2_XL_EN = 0		disable low pass filter
	
	//Gyro
	//CTRL7_G->G_HM_MODE = 0		Enable High-Performance Mode for Gyro
	//CTRL7_G->HP_G_EN = 1			enable high pass filter
	//CTRL7_G->HPCF_G = 10			high pass filter cutoff = 2.07Hz (00=0,0081;01=0,0324;10=2,07;11=16,32)
	//CTRL7_G->HP_G_RST = 1			reset dc offset of Gyro
	//CTRL2_G->ODR_G = 1000			1660Hz data rate for Gyro
	
	uint8_t status;
	
	//Trigger SW-Reset
	status = LSM6D::WriteRegister(LSM6DS3_CTRL3_C, 0x1);
	
	//Configure Accelerometer
	status = LSM6D::WriteRegister(LSM6DS3_CTRL6_C, 0x0);						//CTRL6_C->XL_HM_MODE = 0		Enable High-Performance Mode for Accel
	status = LSM6D::WriteRegister(LSM6DS3_CTRL1_XL, (0b0100 << 4) |				//CTRL1_XL->ODR_XL = 100		104Hz data rate for Accel
													(0b1 << 0));				//CTRL1_XL->BW_XL = 10			analog filter cutoff frequence = 100hz (00=400;01=200;10=100;11=50)
	status = LSM6D::WriteRegister(LSM6DS3_CTRL4_C,	(0b1 << 7));				//CTRL4_C->XL_BW_SCAL_ODR = 1   Select CTRL1_XL->BW_XL as source for analog filter bandwith
	status = LSM6D::WriteRegister(LSM6DS3_TAP_CFG,	(0b1 << 4));				//TAP_CFG->SLOPE_FDS = 1		enable HP and LPF2 for Accel
	
	//Configure Gyro
	status = LSM6D::WriteRegister(LSM6DS3_CTRL2_G,	(0b0100 << 4));				//CTRL2_G->ODR_G = 100			104Hz data rate for Gyro
	status = LSM6D::WriteRegister(LSM6DS3_CTRL7_G,	(0b1 << 6) |				//CTRL7_G->HP_G_EN = 1			enable high pass filter
													(0b10 << 4) |				//CTRL7_G->HPCF_G = 10			high pass filter cutoff = 2.07Hz (00=0,0081;01=0,0324;10=2,07;11=16,32)
													(0b1 << 3));				//CTRL7_G->HP_G_RST = 1			reset dc offset of Gyro
}

uint8_t LSM6D::ReadRegister(uint8_t address)
{
	uint8_t result = 0x00;
	//CS Low
	PORT->Group[0].OUTCLR.reg = PORT_PA09;
	
	SPIPort::TransmitByte(address | 0x80);
	result = SPIPort::TransmitByte(0x00);
	
	//CS High
	PORT->Group[0].OUTSET.reg = PORT_PA09;
	
	return result;
}

int16_t LSM6D::ReadRegisterWord(uint8_t address)
{
	uint8_t result[2];
	//CS Low
	PORT->Group[0].OUTCLR.reg = PORT_PA09;
	
	SPIPort::TransmitByte(address | 0x80);
	result[0] = SPIPort::TransmitByte(0x00);
	result[1] = SPIPort::TransmitByte(0x00);
	
	//CS High
	PORT->Group[0].OUTSET.reg = PORT_PA09;
	
	return (int16_t)result[0] | int16_t(result[1] << 8);
}

uint8_t LSM6D::WriteRegister(uint8_t address, uint8_t data)
{
	uint8_t result = 0x00;
	//CS Low
	PORT->Group[0].OUTCLR.reg = PORT_PA09;
	
	SPIPort::TransmitByte(address & 0x7F);
	result = SPIPort::TransmitByte(data);
	
	//CS High
	PORT->Group[0].OUTSET.reg = PORT_PA09;
	
	return result;
}

// default destructor
LSM6D::~LSM6D()
{
} //~LSM6D
