/* 
* SSD1306.h
*
* Created: 30.05.2018 19:34:55
* Author: dominik hellhake
*/


#ifndef __SSD1306_H__
#define __SSD1306_H__

#include <math.h>
#include "..\..\LowLevel\I2C\I2C.h"

#define SLAVE_ADDR 0b0111100

class SSD1306 : public I2C
{
//variables
public:
protected:
private:
	static uint8_t Blank[32];
	static uint8_t CMDBuffer[27];
	static uint8_t ASCII[320];
	
	COMPILER_ALIGNED(16)
	static DmacDescriptor DMAC_Descriptors[9];

//functions
public:
	static void Init();
	static void Clear();
	static void WriteInt(uint32_t number, uint8_t row);
protected:
	SSD1306();
	
	
	~SSD1306();
private:
	static void SetAddress(uint8_t row);
	static void InitDMADescriptors();

	SSD1306( const SSD1306 &c );
	SSD1306& operator=( const SSD1306 &c );

}; //SSD1306

#endif //__SSD1306_H__
