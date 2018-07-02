/* 
* LSM9D.cpp
*
* Created: 26.06.2018 23:08:02
* Author: dominik hellhake
*/


#include "LSM9D.h"

// static class member
float LSM9D::Pitch = 0;
float LSM9D::Roll = 0;

void LSM9D::Init()
{
	//Init SPI-SERCOM interface
	SPIPort::InitSERCOM();
	
	//Configure CS-Pin as Output
	PORT->Group[0].DIRSET.reg = PORT_PA12;
	PORT->Group[0].OUTSET.reg = PORT_PA12;
	
	/* CTRL_REG5_XL (0x1F) */
	//[DEC_1][DEC_0][Zen_XL][Yen_XL][Zen_XL][0][0][0]
	LSM9D::WriteRegister(CTRL_REG5_XL, (1<<5) | (1<<4) | (1<<3));
	
	/* CTRL_REG6_XL (0x20) */
	//[ODR_XL2][ODR_XL1][ODR_XL0][FS1_XL][FS0_XL][BW_SCAL_ODR][BW_XL1][BW_XL0]
	LSM9D::WriteRegister(CTRL_REG6_XL, (0x1 << 5) | (0x2 << 3));
	
	/* CTRL_REG7_XL (0x21) */
	// [HR][DCF1][DCF0][0][0][FDS][0][HPIS1]
	LSM9D::WriteRegister(CTRL_REG7_XL, 0x00);
}

void LSM9D::Update()
{	
	uint8_t status[1];
	LSM9D::ReadBytes(STATUS_REG_1, status, 1);
	
	if ((status[0] && 0x1) != 0x00)
	{
		uint8_t tmp[6];
		
		if (LSM9D::ReadBytes(OUT_X_L_XL, tmp, 6) != 0x00)
		{	
			float	ax_val = ((int16_t)((tmp[1] << 8) | tmp[0])) * SENSITIVITY_ACCELEROMETER_4;
			float	ay_val = ((int16_t)((tmp[3] << 8) | tmp[2])) * SENSITIVITY_ACCELEROMETER_4;
			float	az_val = ((int16_t)((tmp[5] << 8) | tmp[4])) * SENSITIVITY_ACCELEROMETER_4;
						
			LSM9D::Pitch = (atan2(ax_val, (float)sqrt((float)(ay_val * ay_val) + (float)(az_val * az_val))) * (float)180.0) / M_PI;
			LSM9D::Roll = (atan2(-ay_val, az_val) * 180.0) / M_PI;
		}		
	}
}

uint8_t LSM9D::ReadBytes(uint8_t address, uint8_t *dest, uint8_t count)
{	
	//CS Low
	PORT->Group[0].OUTCLR.reg = PORT_PA12;
	
	SPIPort::TransmitByte(address | 0x80);
	for (uint8_t x = 0; x < count; x++)
		dest[x] = SPIPort::TransmitByte(0x00);
	
	//CS High
	PORT->Group[0].OUTSET.reg = PORT_PA12;
	
	return 1;
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