/* 
* MotorDriver.h
*
* Created: 12.05.2018 13:25:12
* Author: Dominik Hellhake
*/
#ifndef __MOTORDRIVER_H__
#define __MOTORDRIVER_H__

#include "samc21.h"
#include "BLDCPattern.h"
#include "..\MotorSensor\MotorSensor.h"

#define U_HIGH_PATT_Pos		3
#define V_HIGH_PATT_Pos		2
#define W_HIGH_PATT_Pos		0
#define U_LOW_PATT_Pos		7
#define V_LOW_PATT_Pos		6
#define W_LOW_PATT_Pos		4

enum class CONTROLLER_STATE
{
	IDLE,
	DRIVE	
};



class MotorController
{
	private:	
		volatile CONTROLLER_STATE ControllerState = CONTROLLER_STATE::IDLE;
	
	public:
		volatile uint32_t PhaseDuty = 250;
		
		void SetState(CONTROLLER_STATE state);
					
		void SetDuty(uint32_t duty);
		
		void SetHallState(HALL_STATE state);
	
		inline void Drive_SetHallState(HALL_STATE state)
		{
			switch (state)
			{
				case HALL_STATE::HALL_STATE_1:
					TCC0->PATT.reg = BLDC_PATTERN_STEP1;
				break;
				case HALL_STATE::HALL_STATE_2:
					TCC0->PATT.reg = BLDC_PATTERN_STEP2;
				break;
				case HALL_STATE::HALL_STATE_3:
					TCC0->PATT.reg = BLDC_PATTERN_STEP3;
				break;
				case HALL_STATE::HALL_STATE_4:
					TCC0->PATT.reg = BLDC_PATTERN_STEP4;
				break;
				case HALL_STATE::HALL_STATE_5:
					TCC0->PATT.reg = BLDC_PATTERN_STEP5;
				break;
				case HALL_STATE::HALL_STATE_6:
					TCC0->PATT.reg = BLDC_PATTERN_STEP6;
				break;
				case HALL_STATE::UNDEFINED_1:
				case HALL_STATE::UNDEFINED_2:
					TCC0->PATT.reg = BLDC_PATTERN_IDLE;
				break;
			}
		}

		MotorController();
		~MotorController();
}; //MotorDriver

#endif //__MOTORDRIVER_H__
