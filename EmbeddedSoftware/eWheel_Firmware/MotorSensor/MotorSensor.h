/* 
* MotorSensor.h
*
* Created: 12.05.2018 10:18:16
* Author: dominik hellhake
*/

#ifndef __MOTORSENSOR_H__
#define __MOTORSENSOR_H__

#include "sam.h"
#include "..\LowLevel\System\System.h"
#include "..\Executable.h"
#include "..\SSD1306\SSD1306.h"

enum class HALL_STATE 
{	//State decoding: 0bUVW
	HALL_STATE_1 = 0b010,
	HALL_STATE_2 = 0b110,
	HALL_STATE_3 = 0b100,
	HALL_STATE_4 = 0b101,
	HALL_STATE_5 = 0b001,
	HALL_STATE_6 = 0b011,
	UNDEFINED_1  = 0b000,
	UNDEFINED_2  = 0b111
};

class MotorSensor : public Executable
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	public:
		virtual bool CanExecute() { return true; };		
		virtual RUN_RESULT Run();
		virtual void Propagate();
				
	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:
		MotorSensor();	
						
		static inline uint8_t StateToIndex(HALL_STATE state)
		{
			switch (state)
			{
				case HALL_STATE::HALL_STATE_1:
					return 1;
				case HALL_STATE::HALL_STATE_2:
					return 2;
				case HALL_STATE::HALL_STATE_3:
					return 3;
				case HALL_STATE::HALL_STATE_4:
					return 4;
				case HALL_STATE::HALL_STATE_5:
					return 5;
				case HALL_STATE::HALL_STATE_6:
					return 6;
				case HALL_STATE::UNDEFINED_1:
				case HALL_STATE::UNDEFINED_2:
					return 0;
				default:
					return 0;
			}
		}
	public:
		SSD1306 *OLED = NULL;
		volatile HALL_STATE HallState = HALL_STATE::UNDEFINED_1;

}; //MotorSensor

#endif //__MOTORSENSOR_H__
