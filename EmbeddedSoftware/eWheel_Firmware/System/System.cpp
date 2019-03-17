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
	/* Initialize the CC41A DebugLink */
	
	/* Initialize the SSD1306 OLED driver */
	for (uint32_t x = 0; x < 7000000; x++) {}
	eWheel.OLED.DeviceInitialization();

	/* Initialize FlashMemory */
	//eWheel.TraceLink.OLED = &eWheel.OLED;

	/* Initialize BLDC Motor Sensor */
	eWheel.BLDCSensor.EnableInterrupt();
	eWheel.BLDCSensor.OLED = &eWheel.OLED;

	/* Initialize BLDC Motor Controller */
	eWheel.BLDCController.SetHallState(eWheel.BLDCSensor.HallState);

	/* Initialize the lsm9ds1 sensor */
	eWheel.Orientation.TraceLink = &eWheel.TraceLink;
	//eWheel.Orientation.OLED = &eWheel.OLED;

	/* Initialize the Analog Sensor */
	//eWheel.AnalogSensor.OLED = &eWheel.OLED;

	/* Initialize infrared distance sensor GP2Y */
	eWheel.AnalogSensor.IR = &eWheel.InfraRedSensor;
	//ir.OLED = &eWheel.OLED;
}

void System::SetLED(bool state)
{
	if (state)
		PORT->Group[0].OUTSET.reg = PORT_PA28;
	else
		PORT->Group[0].OUTCLR.reg = PORT_PA28;
}