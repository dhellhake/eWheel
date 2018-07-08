/*
 * SPI.h
 *
 * Created: 30.04.2018 15:49:37
 *  Author: dominik hellhake
 */ 

#ifndef SPI_H_
#define SPI_H_

#include "sam.h"
#include "..\System\System.h"

class SPIPort
{
	//functions
	public:
		static void InitSERCOM();
		static inline uint8_t TransmitByte(uint8_t byte)
		{
			while(SERCOM0->SPI.INTFLAG.bit.DRE == 0);
			SERCOM0->SPI.DATA.reg = byte;
			while(SERCOM0->SPI.INTFLAG.bit.RXC == 0);
			return SERCOM0->SPI.DATA.reg;
		}
}; //USART

#endif /* SPI_H_ */