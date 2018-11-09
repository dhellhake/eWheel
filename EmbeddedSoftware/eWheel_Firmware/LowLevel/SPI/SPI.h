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
		static void InitSERCOM0();
		static void InitSERCOM2();
		static inline uint8_t SERCOM0_TransmitByte(uint8_t byte)
		{
			while(SERCOM0->SPI.INTFLAG.bit.DRE == 0);
			SERCOM0->SPI.DATA.reg = byte;
			while(SERCOM0->SPI.INTFLAG.bit.RXC == 0);
			return SERCOM0->SPI.DATA.reg;
		}
		static inline uint8_t SERCOM2_TransmitByte(uint8_t byte)
		{
			while(SERCOM2->SPI.INTFLAG.bit.DRE == 0);
			SERCOM2->SPI.DATA.reg = byte;
			while(SERCOM2->SPI.INTFLAG.bit.RXC == 0);
			return SERCOM2->SPI.DATA.reg;
		}
}; //USART

#endif /* SPI_H_ */