/* 
* LSM9D.h
*
* Created: 26.06.2018 23:08:02
* Author: dominik hellhake
*/


#ifndef __LSM9D_H__
#define __LSM9D_H__

#include <math.h>
#include "..\LowLevel\SPI\SPI.h"

class LSM9D : public SPIPort
{
//variables
public:
	static float Yaw;
	static float Pitch;
	static float Roll;
protected:
private:

//functions
public:
	static void Init();
	static void Update();
	static uint8_t ReadRegister(uint8_t address);
	static int16_t ReadRegisterWord(uint8_t address);
	static uint8_t WriteRegister(uint8_t address, uint8_t data);
protected:
private:
	LSM9D();
	~LSM9D();
	LSM9D( const LSM9D &c );
	LSM9D& operator=( const LSM9D &c );

}; //LSM9D

#endif //__LSM9D_H__
