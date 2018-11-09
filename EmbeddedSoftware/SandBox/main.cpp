/*
 * SandBox.cpp
 *
 * Created: 30.04.2018 15:10:53
 * Author : dominik hellhake
 */ 
#include "sam.h"
#include "LowLevel/System/System.h"
#include "DebugPort/DebugPort.h"
#include "LSM9D/LSM9D.h"
#include "MotorControl/MotorControl.h"
#include "SystemMonitor/SSD1306/SSD1306.h"

#include "LowLevel/SPI/SPI.h"


void EIC_Handler()
{
	if ((EIC->INTFLAG.reg & ((1 << 7) | (1 << 12) | (1 << 13))) != 0x00)
	{
		/* Hall Sensor caused interrupt */
		MotorControl::ElectronicPhase_Changed();
		
		EIC->INTFLAG.reg = (1 << 7) | (1 << 12) | (1 << 13);
	}
}

int16_t WriteRegister(uint8_t *data)
{
	uint32_t t1 = SysTick->VAL;
	uint8_t result[4] = {0x00, 0x00, 0x00, 0x00};
		
	//CS Low
	PORT->Group[0].OUTCLR.reg = PORT_PA02;
	
	while ((t1 - 500) < SysTick->VAL){}
	
	result[0] = SPIPort::SERCOM2_TransmitByte(data[0]);
	result[1] = SPIPort::SERCOM2_TransmitByte(data[1]);
	result[2] = SPIPort::SERCOM2_TransmitByte(data[0]);
	result[3] = SPIPort::SERCOM2_TransmitByte(data[1]);
	
	//CS High
	PORT->Group[0].OUTSET.reg = PORT_PA02;
	
	return ((result[0] << 8) | result[1]);
}

int main(void)
{
    /* Initialize the SAM system */
    System::Init();
	/* Initialize the SSD1306 OLED driver */
	SSD1306::Init();
	/* Initialize the debug serial port */
	//DebugPort::Init();
	/* Initialize the lsm9ds1 sensor */
	//LSM9D::Init();
	/* Initialize BLDC Motor Control */
	//MotorControl::Init();
	
	int16_t tmp;
	uint32_t cnt = 2;
	uint32_t t1 = System::GetElapsedMilis();
	uint32_t t2;
	while (1)
	{	
		t2 = System::GetElapsedMilis();
		if ((t1 + 200) < t2)
		{			
			
			
			t1 = System::GetElapsedMilis();
		}
	}
}

