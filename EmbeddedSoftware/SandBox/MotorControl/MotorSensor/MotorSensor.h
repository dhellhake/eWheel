/* 
* MotorSensor.h
*
* Created: 12.05.2018 10:18:16
* Author: dominik hellhake
*/


#ifndef __MOTORSENSOR_H__
#define __MOTORSENSOR_H__

#include "sam.h"
#include "..\..\LowLevel\System\System.h"

enum class HALL_STATE 
{	//State decoding: 0vUVW
	HALL_STATE_1 = 0b101,
	HALL_STATE_2 = 0b100,
	HALL_STATE_3 = 0b110,
	HALL_STATE_4 = 0b010,
	HALL_STATE_5 = 0b011,
	HALL_STATE_6 = 0b001,
	UNDEFINED_1  = 0b000,
	UNDEFINED_2  = 0b111
};

class MotorSensor
{
	//variables
	public:
		volatile static HALL_STATE HallState;
	protected:
	private:
	
	//functions
	public:
		static void InitEIC();	//ToBeProtected
		
		static void HallState_Update();
		
		static inline HALL_STATE MASKToState(uint8_t mask)
		{
			switch (mask)
			{
				case (uint8_t)HALL_STATE::HALL_STATE_1:
				return HALL_STATE::HALL_STATE_1;
				break;
				case (uint8_t)HALL_STATE::HALL_STATE_2:
				return HALL_STATE::HALL_STATE_2;
				break;
				case (uint8_t)HALL_STATE::HALL_STATE_3:
				return HALL_STATE::HALL_STATE_3;
				break;
				case (uint8_t)HALL_STATE::HALL_STATE_4:
				return HALL_STATE::HALL_STATE_4;
				break;
				case (uint8_t)HALL_STATE::HALL_STATE_5:
				return HALL_STATE::HALL_STATE_5;
				break;
				case (uint8_t)HALL_STATE::HALL_STATE_6:
				return HALL_STATE::HALL_STATE_6;
				break;
				case (uint8_t)HALL_STATE::UNDEFINED_1:
				return HALL_STATE::UNDEFINED_1;
				break;
				case (uint8_t)HALL_STATE::UNDEFINED_2:
				return HALL_STATE::UNDEFINED_2;
				break;
			}
			return HALL_STATE::UNDEFINED_1;
		}
	protected:
		MotorSensor();
		
		
		~MotorSensor();
	private:
		MotorSensor( const MotorSensor &c );
		MotorSensor& operator=( const MotorSensor &c );		
}; //MotorSensor

#endif //__MOTORSENSOR_H__
