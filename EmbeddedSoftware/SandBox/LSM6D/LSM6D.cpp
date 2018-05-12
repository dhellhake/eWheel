/* 
* LSM6D.cpp
*
* Created: 06.05.2018 15:52:49
* Author: dominik hellhake
*/


#include "LSM6D.h"


// static class member
float LSM6D::Yaw = 0;
float LSM6D::Pitch = 0;
float LSM6D::Roll = 0;

// default constructor
LSM6D::LSM6D()
{
} //LSM6D

void LSM6D::Init()
{
	//Init SPI-SERCOM interface
	SPIPort::InitSERCOM();
		
	//Configure CS-Pin as Output
	PORT->Group[0].DIRSET.reg = PORT_PA12;
	PORT->Group[0].OUTSET.reg = PORT_PA12;
	
	//Configure INT1-Pin as Interrupt-Trigger Input
	PORT->Group[0].DIRCLR.reg = PORT_PA13;
	PORT->Group[0].PINCFG[PIN_PA13].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	System::SetPinPeripheralFunction(PINMUX_PA13A_EIC_EXTINT13);
			
	//Trigger SW-Reset
	LSM6D::WriteRegister(LSM6DS3_CTRL3_C, 0x1);
	
	LSM6D::WriteRegister(LSM6DS3_INT1_CTRL, (1 << 0));
	
	//Configure Accelerometer
	LSM6D::WriteRegister(LSM6DS3_CTRL6_C, 0x0);							//CTRL6_C->XL_HM_MODE = 0		Enable High-Performance Mode for Accel
	LSM6D::WriteRegister(LSM6DS3_CTRL1_XL,  (0b0001 << 4) |				//CTRL1_XL->ODR_XL = 100		13Hz data rate for Accel
											(0b1 << 0));				//CTRL1_XL->BW_XL = 10			analog filter cutoff frequence = 100hz (00=400;01=200;10=100;11=50)
	LSM6D::WriteRegister(LSM6DS3_CTRL4_C,	(0b1 << 7));				//CTRL4_C->XL_BW_SCAL_ODR = 1   Select CTRL1_XL->BW_XL as source for analog filter bandwith
	LSM6D::WriteRegister(LSM6DS3_TAP_CFG,	(0b1 << 4));				//TAP_CFG->SLOPE_FDS = 1		enable HP and LPF2 for Accel
	
	/* //Configure Gyro
	LSM6D::WriteRegister(LSM6DS3_CTRL2_G,	(0b0100 << 4));				//CTRL2_G->ODR_G = 100			104Hz data rate for Gyro
	LSM6D::WriteRegister(LSM6DS3_CTRL7_G,	(0b1 << 6) |				//CTRL7_G->HP_G_EN = 1			enable high pass filter
											(0b10 << 4) |				//CTRL7_G->HPCF_G = 10			high pass filter cutoff = 2.07Hz (00=0,0081;01=0,0324;10=2,07;11=16,32)
											(0b1 << 3));				//CTRL7_G->HP_G_RST = 1			reset dc offset of Gyro
	*/
		
	/* Pin-Config: INT1 on EXTINT13 */
	PORT->Group[0].DIRCLR.reg = PORT_PA13;
	PORT->Group[0].PINCFG[PIN_PA13].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	System::SetPinPeripheralFunction(PINMUX_PA13A_EIC_EXTINT13);
		
	//Enable Interrupt for PIN
	EIC->INTENSET.reg |= (1 << 13);											//Enable Interrupt on EXTINT13  for LSM6D-INT1
}


void LSM6D::Update() 
{
	int16_t accX = LSM6D::ReadRegisterWord(LSM6DS3_OUTX_L_XL);
	int16_t accY = LSM6D::ReadRegisterWord(LSM6DS3_OUTY_L_XL);
	int16_t accZ = LSM6D::ReadRegisterWord(LSM6DS3_OUTZ_L_XL);
		
	LSM6D::Pitch = (atan2(accX, sqrt(accY * accY + accZ * accZ)) * 180.0) / M_PI;
	LSM6D::Roll = (atan2(-accY, accZ) * 180.0) / M_PI;
	//LSM6D::Yaw = 180 * atan(accZ / sqrt(accX * accX + accZ * accZ)) / M_PI;
}


uint8_t LSM6D::ReadRegister(uint8_t address)
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

int16_t LSM6D::ReadRegisterWord(uint8_t address)
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

uint8_t LSM6D::WriteRegister(uint8_t address, uint8_t data)
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

// default destructor
LSM6D::~LSM6D()
{
} //~LSM6D
