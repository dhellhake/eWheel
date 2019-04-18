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
	/* Initialize VESC */	
	
	/* Initialize the CC41A DebugLink */

	/* Initialize FlashMemory */
	//eWheel.TraceLink.OLED = &eWheel.OLED;

	/* Initialize the lsm9ds1 sensor */
	eWheel.Orientation.TraceLink = &eWheel.DebugLink;
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