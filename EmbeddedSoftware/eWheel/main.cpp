/*
 * eWheel.cpp
 *
 * Created: 13.05.2020 21:45:06
 * Author : dominik hellhake
 */ 
#include "sam.h"
#include "Peripheral/System/System.h"
#include "Peripheral/SPI/SPIlib.h"
#include "Peripheral/PMC/PMClib.h"

volatile uint8_t data[6] = { 0 };
void TestW25Q()
{	
	data[0] = 0x9f;
	
	SPI_TransferData((uint8_t*)data, 4);
	
	data[0]++;
}

int main(void)
{		
	while (1)
	{
	}
}
