/* 
* LSM9D.h
*
* Created: 26.06.2018 23:08:02
* Author: Dominik Hellhake
*/
#ifndef __LSM9D_H__
#define __LSM9D_H__

#include "..\Executable.h"

// Register addresses
#define STATUS_REG_1					0x27
#define CTRL_REG5_XL					0x1F
#define CTRL_REG6_XL					0x20
#define CTRL_REG7_XL					0x21
#define OUT_X_L_XL						0x28
#define INT1_CTRL						0x0C

// Sensor Sensitivity Constants
#define SENSITIVITY_ACCELEROMETER_2		0.61
#define SENSITIVITY_ACCELEROMETER_4		1.22
#define SENSITIVITY_ACCELEROMETER_16	7.32

#define PITCH_OFFSET					2.41778326f
#define ROLL_OFFSET						-2.10467649f

class LSM9D : public Executable
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	public:	
		virtual RUN_RESULT Run(uint32_t timeStamp);
	
	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:
		float Pitch = 0;
		float Roll = 0;	
	public:
		LSM9D();
	private:
	
		uint8_t ReadBytes(uint8_t address, uint8_t *dest, uint8_t count);	
		uint8_t WriteRegister(uint8_t address, uint8_t data);
}; //LSM9D
extern LSM9D Gyro;

#endif //__LSM9D_H__
