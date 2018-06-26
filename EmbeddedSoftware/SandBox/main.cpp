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

void EIC_Handler()
{
	if ((EIC->INTFLAG.reg & ((1 << 7) | (1 << 12) | (1 << 13))) != 0x00)
	{
		/* Hall Sensor caused interrupt */
		MotorControl::ElectronicPhase_Changed();
				
		EIC->INTFLAG.reg = (1 << 7) | (1 << 12) | (1 << 13);
	}
}

int main(void)
{
    /* Initialize the SAM system */
    System::Init();
	/* Initialize the SSD1306 OLED driver */
	//SSD1306::Init();
	/* Initialize the debug serial port */
	//DebugPort::Init();
	/* Initialize the lsm9ds1 sensor */
	LSM9D::Init();
	/* Initialize BLDC Motor Control */
	//MotorControl::Init();
	
	while(1){}
		
	SSD1306::Clear();
	
	uint32_t cnt = 12345678;
	uint32_t t1 = System::GetElapsedMilis();
	uint32_t t2;
	while (1)
	{	
		t2 = System::GetElapsedMilis();
		if ((t1 + 1000) < t2)
		{
			cnt++;
			
			t1 = SysTick->VAL;
			SSD1306::WriteInt(cnt, 0);
			t1 = SysTick->VAL;
			
			t1 = System::GetElapsedMilis();
		}
	}
}

