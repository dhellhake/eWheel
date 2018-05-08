/*
 * SandBox.cpp
 *
 * Created: 30.04.2018 15:10:53
 * Author : dominik hellhake
 */ 


#include "sam.h"
#include "LowLevel/System/System.h"
#include "DebugPort/DebugPort.h"
#include "LSM6D/LSM6D.h"


int main(void)
{
    /* Initialize the SAM system */
    System::Init();
	/* Initialize the debug serial port */
	DebugPort::Init();
	/* Initialize the lsm6d sensor */
	LSM6D::Init();
		

	uint8_t status;
    /* Replace with your application code */
    while (1) 
    {	    
	    do
	    {
		    status = LSM6D::ReadRegister(LSM6DS3_STATUS_REG);
	    } while (status == 0x0);
		
		
		if ((status & 0b1) != 0x0)
		{
			//int16_t value;			
			//value = LSM6D::ReadRegisterWord(LSM6DS3_OUTX_L_XL);
			//float output = (float)value * 0.061 * (2 >> 1) / 1000;
			
			
			uint8_t result[2];
			result[0] = LSM6D::ReadRegister(LSM6DS3_OUTX_L_XL);
			result[1] = LSM6D::ReadRegister(LSM6DS3_OUTX_H_XL);
						
			DebugPort::SendByte(0x1);
			DebugPort::SendByte(result[0]);
			DebugPort::SendByte(result[1]);	
			
			
			result[0] = LSM6D::ReadRegister(LSM6DS3_OUTY_L_XL);
			result[1] = LSM6D::ReadRegister(LSM6DS3_OUTY_H_XL);
			
			DebugPort::SendByte(0x2);
			DebugPort::SendByte(result[0]);
			DebugPort::SendByte(result[1]);	
			
			
			result[0] = LSM6D::ReadRegister(LSM6DS3_OUTZ_L_XL);
			result[1] = LSM6D::ReadRegister(LSM6DS3_OUTZ_H_XL);
			
			DebugPort::SendByte(0x3);
			DebugPort::SendByte(result[0]);
			DebugPort::SendByte(result[1]);
		}
		
		if ((status & 0b10) != 0x0)
		{
			//int16_t value;
			//value = LSM6D::ReadRegisterWord(LSM6DS3_OUTX_L_XL);
			//float output = (float)value * 0.061 * (2 >> 1) / 1000;
			
			
			uint8_t result[2];
			result[0] = LSM6D::ReadRegister(LSM6DS3_OUTX_L_G);
			result[1] = LSM6D::ReadRegister(LSM6DS3_OUTX_H_G);
			
			DebugPort::SendByte(0x4);
			DebugPort::SendByte(result[0]);
			DebugPort::SendByte(result[1]);
			
			
			result[0] = LSM6D::ReadRegister(LSM6DS3_OUTY_L_G);
			result[1] = LSM6D::ReadRegister(LSM6DS3_OUTY_H_G);
			
			DebugPort::SendByte(0x5);
			DebugPort::SendByte(result[0]);
			DebugPort::SendByte(result[1]);
			
			
			result[0] = LSM6D::ReadRegister(LSM6DS3_OUTZ_L_G);
			result[1] = LSM6D::ReadRegister(LSM6DS3_OUTZ_H_G);
			
			DebugPort::SendByte(0x6);
			DebugPort::SendByte(result[0]);
			DebugPort::SendByte(result[1]);
		}
		
    }
}

