/*
 * SPI.h
 *
 * Created: 30.04.2018 15:49:37
 *  Author: dominik hellhake
 */ 


#ifndef SPI_H_
#define SPI_H_

#include "sam.h"
#include "..\system.h"

class SPIPort
{
	//variables
	public:
	protected:
	private:

	//functions
	public:	
	static void SPIInit();
	static uint8_t SendByte(uint8_t byte);
	protected:
	private:
	SPIPort();
	~SPIPort();
	SPIPort( const SPIPort &c );
	SPIPort& operator=( const SPIPort &c );

}; //USART

#endif /* SPI_H_ */