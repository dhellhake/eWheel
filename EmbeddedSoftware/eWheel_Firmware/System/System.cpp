/* 
* System.cpp
*
* Created: 08.05.2018 18:22:30
* Author: Dominik Hellhake
*/
#include "System.h"

System eWheel;


System::System()
{
	/* Initialize the SSD1306 OLED driver */
	for (uint32_t x = 0; x < 7000000; x++) {}
	eWheel.OLED.DeviceInitialization();
	eWheel.TaskPool[0] = &eWheel.OLED;

	/* Initialize FlashMemory */
	//at45.OLED = &eWheel.OLED;
	eWheel.TaskPool[4] = &eWheel.TraceLink;

	/* Initialize BLDC Motor Sensor */
	eWheel.BLDCSensor.EnableInterrupt();
	//motorSensor.OLED = &eWheel.OLED;

	/* Initialize BLDC Motor Controller */
	eWheel.BLDCController.SetHallState(eWheel.BLDCSensor.HallState);

	/* Initialize the lsm9ds1 sensor */
	eWheel.Orientation.TraceLink = &eWheel.TraceLink;
	//gyro.OLED = &eWheel.OLED;
	eWheel.TaskPool[1] = &eWheel.Orientation;

	/* Initialize the Analog Sensor */
	eWheel.AnalogSensor.OLED = &eWheel.OLED;
	eWheel.TaskPool[2] = &eWheel.AnalogSensor;

	/* Initialize infrared distance sensor GP2Y */
	eWheel.AnalogSensor.IR = &eWheel.InfraRedSensor;
	//ir.OLED = &eWheel.OLED;
	eWheel.TaskPool[3] = &eWheel.InfraRedSensor;	
}

void System::SetLED(bool state)
{
	if (state)
	PORT->Group[0].OUTSET.reg = PORT_PA28;
	else
	PORT->Group[0].OUTSET.reg = PORT_PA28;
}