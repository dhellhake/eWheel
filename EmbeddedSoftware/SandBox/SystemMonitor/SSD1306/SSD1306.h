/* 
* SSD1306.h
*
* Created: 30.05.2018 19:34:55
* Author: domin
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
static uint8_t FrameBuffer[513];
static uint8_t ASCII[320];

//functions
public:
	static void Init();
	static void Update();
	static void WriteInt(uint16_t number, uint8_t row);
protected:
	SSD1306();
	
	
	~SSD1306();
private:
	static void ClearPage(uint8_t page);
	static void WriteChar(uint8_t *chr, uint16_t index);

	SSD1306( const SSD1306 &c );
	SSD1306& operator=( const SSD1306 &c );

}; //SSD1306

#endif //__SSD1306_H__
