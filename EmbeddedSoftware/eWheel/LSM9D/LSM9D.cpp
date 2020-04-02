/* 
* LSM9D.cpp
*
* Created: 26.06.2018 23:08:02
* Author: Dominik Hellhake
*/
#include <math.h>
#include "LSM9D.h"
#include "..\LowLevel\Device\SERCOM\SERCOM.h"

LSM9D Gyro;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT LSM9D::Run(uint32_t timeStamp)
{	
	float	ax_val = ((int16_t)((this->OUT_X_L_XL_Raw[1] << 8) | this->OUT_X_L_XL_Raw[0])) * SENSITIVITY_ACCELEROMETER_2;
	float	ay_val = ((int16_t)((this->OUT_X_L_XL_Raw[3] << 8) | this->OUT_X_L_XL_Raw[2])) * SENSITIVITY_ACCELEROMETER_2;
	float	az_val = ((int16_t)((this->OUT_X_L_XL_Raw[5] << 8) | this->OUT_X_L_XL_Raw[4])) * SENSITIVITY_ACCELEROMETER_2;
	
	this->Roll = (atan2(ax_val, (float)sqrt((float)(ay_val * ay_val) + (float)(az_val * az_val))) * (float)180.0) / M_PI;
	this->Pitch = (atan2(-ay_val, az_val) * 180.0) / M_PI;
	
	this->Roll += ROLL_OFFSET;
	this->Pitch += PITCH_OFFSET;
		
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
LSM9D::LSM9D()
{
	for (uint32_t x = 0; x < 1000000; x++) {}
			
	/* CTRL_REG5_XL (0x1F) */
	//[DEC_1][DEC_0][Zen_XL][Yen_XL][Zen_XL][0][0][0]
	this->WriteRegister(CTRL_REG5_XL, (0x0 << 6) | (1<<5) | (1<<4) | (1<<3));
	
	/* CTRL_REG6_XL (0x20) */
	//[ODR_XL2][ODR_XL1][ODR_XL0][FS1_XL][FS0_XL][BW_SCAL_ODR][BW_XL1][BW_XL0]
	this->WriteRegister(CTRL_REG6_XL, (0x5 << 5) | (0x0 << 3) | (0x0 << 2) | (0x3 << 0));
	
	/* CTRL_REG7_XL (0x21) */
	// [HR][DCF1][DCF0][0][0][FDS][0][HPIS1]
	this->WriteRegister(CTRL_REG7_XL, (0x1 << 7) | (0x2 << 5));
	
	/* INT1_CTRL (0x0C) */
	// [INT1_IG_G][INT1_IG_XL][INT1_FSS5][INT1_OVR][INT1_FTH][INT1_ Boot][INT1_DRDY_G][INT1_DRDY_XL]
	this->WriteRegister(INT1_CTRL, 0x01);
	
	
	EIC->INTENSET.reg |= (1 << 13);
}

void LSM9D::ReadValues()
{
	this->ReadBytes(OUT_X_L_XL, this->OUT_X_L_XL_Raw, 6);	
}


uint8_t LSM9D::ReadBytes(uint8_t address, uint8_t *dest, uint8_t count)
{	
	//CS Low
	PORT->Group[0].OUTCLR.reg = PORT_PA12;
	
	SERCOM2_TransmitByte(address | 0x80);
	for (uint8_t x = 0; x < count; x++)
		dest[x] = SERCOM2_TransmitByte(0x00);
	
	//CS High
	PORT->Group[0].OUTSET.reg = PORT_PA12;
	
	return 1;
}

uint8_t LSM9D::WriteRegister(uint8_t address, uint8_t data)
{
	uint8_t result = 0x00;
	//CS Low
	PORT->Group[0].OUTCLR.reg = PORT_PA12;
	
	SERCOM2_TransmitByte(address & 0x7F);
	result = SERCOM2_TransmitByte(data);
	
	//CS High
	PORT->Group[0].OUTSET.reg = PORT_PA12;
	
	return result;
}