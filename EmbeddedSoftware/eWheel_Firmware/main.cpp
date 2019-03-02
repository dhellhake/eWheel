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
#include "LSM9D/LSM9D.h"
#include "ADS/ADS.h"
#include "GP2Y/GP2Y.h"
#include "AT45DB/AT45DB.h"

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
	for (uint32_t x = 0; x < 4000000; x++) {}
	SSD1306 mainOLED;
	mainOLED.Clear();
	System::TaskPool[0] = &mainOLED;
	
	/* Initialize BLDC Motor Sensor */
	motorSensor = MotorSensor();
	motorSensor.Run();
	//motorSensor.OLED = &mainOLED;
	
	/* Initialize BLDC Motor Controller */
	motorController = MotorController();
	motorController.Drive_SetPhase(motorSensor.HallState);
		
	/* Initialize the lsm9ds1 sensor */
	LSM9D gyro;
	gyro.OLED = &mainOLED;
	System::TaskPool[1] = &gyro;
	
	/* Initialize the Analog Sensor */
	ADS ads;
	//ads.OLED = &mainOLED;
	System::TaskPool[2] = &ads;
	
	/* Initialize infrared distance sensor GP2Y */
	GP2Y ir;
	ads.IR = &ir;
	//ir.OLED = &mainOLED;
	System::TaskPool[3] = &ir;
	
	/* Initialize infrared distance sensor GP2Y */
	AT45DB at45;
	System::TaskPool[4] = &at45;
		
	uint64_t t1 = System::GetElapsedMilis();
    while (1) 
    {
		if ((t1 + 10) <= System::GetElapsedMilis())
		{
			t1+= 10;
			
			for (uint8_t ti = 0; ti < TASKPOOL_SIZE; ti++)
			{
				if (System::TaskPool[ti]->CanExecute())
				{
					if (System::TaskPool[ti]->Run() == RUN_RESULT::SUCCESS)
					{					
						System::TaskPool[ti]->Propagate();
						break;
					}
					
				}
			}
		}
    }
}
