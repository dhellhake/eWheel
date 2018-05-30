/* 
* I2C.h
*
* Created: 30.05.2018 18:19:17
* Author: domin
*/


#ifndef __I2C_H__
#define __I2C_H__

#include "sam.h"
#include "..\System\System.h"

class I2C
{
//variables
public:
protected:
private:
	static uint8_t *buffer_ref;
	static uint8_t tx_index;
	static uint16_t tx_count;

	static volatile bool tx_complete;

//functions
public:
	static void InitSERCOM();
	
	static void StartTransmitt(uint8_t slave_addr, uint8_t *bufferRef, uint16_t count);
	
	static inline void Handler()
	{
		/* Master on bus interrupt checking */
		if (SERCOM4->I2CM.INTFLAG.bit.MB)
		{
			if (I2C::tx_index == I2C::tx_count)
			{ /* After transferring the last byte stop condition will be sent */
				SERCOM4->I2CM.CTRLB.bit.CMD = 0x3;
				while(SERCOM4->I2CM.SYNCBUSY.bit.SYSOP);
				I2C::tx_complete = true;
				I2C::tx_index = 0;
			}
			else
			{ /* placing the data from transmitting buffer to DATA register*/
				SERCOM4->I2CM.DATA.reg = I2C::buffer_ref[I2C::tx_index++];
				while(SERCOM4->I2CM.SYNCBUSY.bit.SYSOP);
			}
		}
	}
	
	static inline void Wait()
	{
		while(!I2C::tx_complete);
	}
protected:
	I2C();
	~I2C();
private:
	I2C( const I2C &c );
	I2C& operator=( const I2C &c );
}; //I2C

#endif //__I2C_H__
