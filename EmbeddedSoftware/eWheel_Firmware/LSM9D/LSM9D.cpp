/* 
* LSM9D.cpp
*
* Created: 26.06.2018 23:08:02
* Author: Dominik Hellhake
*/
#include <math.h>
#include "LSM9D.h"
#include "..\LowLevel\Device\SysTick\SysTick.h"
#include "..\LowLevel\SPI\SPI.h"

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT LSM9D::Run(uint32_t timeStamp)
{
	uint8_t status[1];
	this->ReadBytes(STATUS_REG_1, status, 1);
		
	if ((status[0] && 0x1) != 0x00)
	{
		uint8_t tmp[6];
		
		if (this->ReadBytes(OUT_X_L_XL, tmp, 6) != 0x00)
		{
			float	ax_val = ((int16_t)((tmp[1] << 8) | tmp[0])) * SENSITIVITY_ACCELEROMETER_4;
			float	ay_val = ((int16_t)((tmp[3] << 8) | tmp[2])) * SENSITIVITY_ACCELEROMETER_4;
			float	az_val = ((int16_t)((tmp[5] << 8) | tmp[4])) * SENSITIVITY_ACCELEROMETER_4;
			
			this->Pitch = (atan2(ax_val, (float)sqrt((float)(ay_val * ay_val) + (float)(az_val * az_val))) * (float)180.0) / M_PI;
			this->Roll = (atan2(-ay_val, az_val) * 180.0) / M_PI;

			return RUN_RESULT::SUCCESS;
		}
	}
	return RUN_RESULT::ERROR;
}

void LSM9D::Propagate()
{
	if (this->OLED != NULL)
	{
		if (this->Pitch < 0)
			this->OLED->SetRow(this->Pitch * -10, 1);
		else
			this->OLED->SetRow(this->Pitch * 10, 1);
	}
	
	if (this->TraceEnabled && this->TraceLink != NULL)
	{
		uint8_t data[8];
		float *f = (float*) data;
		f[0] = this->Pitch;
		f[1] = this->Roll;
				
		DataPackage pkg;
		pkg._timeStamp = GetElapsedMilis();
		pkg._type = PackageType::Orientation;
		pkg._length = 8;
		pkg._data = data;
		
		this->TraceLink->SendDataPackage(&pkg);
	}
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
LSM9D::LSM9D()
{	
	/* CTRL_REG5_XL (0x1F) */
	//[DEC_1][DEC_0][Zen_XL][Yen_XL][Zen_XL][0][0][0]
	this->WriteRegister(CTRL_REG5_XL, (1<<5) | (1<<4) | (1<<3));
	
	/* CTRL_REG6_XL (0x20) */
	//[ODR_XL2][ODR_XL1][ODR_XL0][FS1_XL][FS0_XL][BW_SCAL_ODR][BW_XL1][BW_XL0]
	this->WriteRegister(CTRL_REG6_XL, (0x1 << 5) | (0x2 << 3));
	
	/* CTRL_REG7_XL (0x21) */
	// [HR][DCF1][DCF0][0][0][FDS][0][HPIS1]
	this->WriteRegister(CTRL_REG7_XL, 0x00);
	
	/* INT1_CTRL (0x0C) */
	// [INT1_IG_G][INT1_IG_XL][INT1_FSS5][INT1_OVR][INT1_FTH][INT1_ Boot][INT1_DRDY_G][INT1_DRDY_XL]
	this->WriteRegister(INT1_CTRL, 0x01);
}

uint8_t LSM9D::ReadBytes(uint8_t address, uint8_t *dest, uint8_t count)
{	
	//CS Low
	PORT->Group[0].OUTCLR.reg = PORT_PA12;
	
	SPIPort::SERCOM2_TransmitByte(address | 0x80);
	for (uint8_t x = 0; x < count; x++)
		dest[x] = SPIPort::SERCOM2_TransmitByte(0x00);
	
	//CS High
	PORT->Group[0].OUTSET.reg = PORT_PA12;
	
	return 1;
}

uint8_t LSM9D::WriteRegister(uint8_t address, uint8_t data)
{
	uint8_t result = 0x00;
	//CS Low
	PORT->Group[0].OUTCLR.reg = PORT_PA12;
	
	SPIPort::SERCOM2_TransmitByte(address & 0x7F);
	result = SPIPort::SERCOM2_TransmitByte(data);
	
	//CS High
	PORT->Group[0].OUTSET.reg = PORT_PA12;
	
	return result;
}