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
#include "..\LowLevel\System\System.h"

// Register addresses
#define STATUS_REG_1					0x27
#define CTRL_REG5_XL					0x1F
#define CTRL_REG6_XL					0x20
#define CTRL_REG7_XL					0x21
#define OUT_X_L_XL						0x28

// Sensor Sensitivity Constants
#define SENSITIVITY_ACCELEROMETER_4		1.22


class LSM9D : public SPIPort
{
//variables
public:
	static float Pitch;
	static float Roll;
protected:
private:

//functions
public:
	static void Init();
	static void Update();
	
	static uint8_t ReadBytes(uint8_t address, uint8_t *dest, uint8_t count);
	
	static uint8_t WriteRegister(uint8_t address, uint8_t data);
protected:
private:
	LSM9D();
	~LSM9D();
	LSM9D( const LSM9D &c );
	LSM9D& operator=( const LSM9D &c );

}; //LSM9D

#endif //__LSM9D_H__
