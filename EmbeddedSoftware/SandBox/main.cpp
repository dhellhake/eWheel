/*
 * SandBox.cpp
 *
 * Created: 30.04.2018 15:10:53
 * Author : dominik hellhake
 */ 


#include "sam.h"
#include "LowLevel/SPI/SPI.h"
#include "LowLevel/DebugPort/DebugPort.h"


int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	
	
	DebugPort debugPort;
	
	uint8_t bt = 0xAA;
	
	debugPort.SendByte(bt);	
		
	SPIPort::SPIInit();
	
	
	
	//Configure CS-Pin as Output
	PORT->Group[0].DIRSET.reg = PORT_PA09;
	PORT->Group[0].OUTSET.reg = PORT_PA09;
	
	PORT->Group[0].OUTCLR.reg = PORT_PA09;
	uint8_t result = SPIPort::SendByte(0x8F);
	result = SPIPort::SendByte(0x00);
	
	PORT->Group[0].OUTSET.reg = PORT_PA09;
	
	
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

