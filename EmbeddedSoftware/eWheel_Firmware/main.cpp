/*
 * eWheel_Firmware
 *
 * Created: 08.07.2018 08:52:44
 * Author : dominik hellhake
 */ 
#include "sam.h"
#include "LowLevel/System/System.h"
#include "SSD1306/SSD1306.h"
#include "MotorController/MotorController.h"
#include "MotorSensor/MotorSensor.h"

MotorSensor motorSensor;
MotorController motorController;


void EIC_Handler()
{
	if ((EIC->INTFLAG.reg & ((1 << 7) | (1 << 12) | (1 << 13))) != 0x00)
	{
		/* Hall Sensor caused interrupt */
		motorSensor.Run();
		motorController.Drive_SetPhase(motorSensor.HallState);
		motorSensor.Propagate();
		
		EIC->INTFLAG.reg = (1 << 7) | (1 << 12) | (1 << 13);
	}
}

int main(void)
{
	/* Initialize the SAM system */
	System::Init();
	
	/* Initialize the SSD1306 OLED driver */
	SSD1306 mainOLED;
	mainOLED.Clear();
	System::TaskPool[0] = &mainOLED;
	
	/* Initialize BLDC Motor Sensor */
	motorSensor = MotorSensor();
	motorSensor.Run();
	motorSensor.OLED = &mainOLED;
	
	/* Initialize BLDC Motor Controller */
	motorController = MotorController();
	motorController.Drive_SetPhase(motorSensor.HallState);
	
	
	
    while (1) 
    {
		for (uint8_t ti = 0; ti < System::TaskPoolCount; ti++)
			if (System::TaskPool[ti]->CanExecute())
			{
				System::TaskPool[ti]->Run();
				System::TaskPool[ti]->Propagate();
			}
    }
}
