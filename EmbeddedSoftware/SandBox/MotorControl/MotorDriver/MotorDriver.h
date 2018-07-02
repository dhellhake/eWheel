/* 
* MotorDriver.h
*
* Created: 12.05.2018 13:25:12
* Author: dominik hellhake
*/


#ifndef __MOTORDRIVER_H__
#define __MOTORDRIVER_H__

#include "sam.h"
#include "..\..\LowLevel\System\System.h"
#include "..\MotorSensor\MotorSensor.h"

#define U_HIGH_PATT_Pos		3
#define V_HIGH_PATT_Pos		2
#define W_HIGH_PATT_Pos		0
#define U_LOW_PATT_Pos		7
#define V_LOW_PATT_Pos		6
#define W_LOW_PATT_Pos		4

class MotorDriver
{
	//variables
	public:
		volatile static uint32_t PhaseDuty;
	protected:
	private:

	//functions
	public:
		static void InitTCC();	//ToBeProtected
		
		static inline void Drive_SetDuty(uint32_t duty)
		{
			TCC0->CC[0].reg	= duty;
		}
	
		static inline void Drive_SetPhase(HALL_STATE state)
		{
			switch (state)
			{
				case HALL_STATE::HALL_STATE_1:
				TCC0->PATT.reg =	((	(1 << U_HIGH_PATT_Pos)						|
										(1 << U_LOW_PATT_Pos)						|
										(0 << V_HIGH_PATT_Pos)						|
										(0 << V_LOW_PATT_Pos)						|
										(1 << W_HIGH_PATT_Pos)						|
										(1 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) |
									((	(0 << U_HIGH_PATT_Pos)						|
										(1 << U_LOW_PATT_Pos)						|
										(0 << V_HIGH_PATT_Pos)						|
										(0 << V_LOW_PATT_Pos)						|
										(0 << W_HIGH_PATT_Pos)						|
										(0 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos);
				break;
				case HALL_STATE::HALL_STATE_2:
				TCC0->PATT.reg =	((	(1 << U_HIGH_PATT_Pos)						|
									(1 << U_LOW_PATT_Pos)						|
									(0 << V_HIGH_PATT_Pos)						|
									(0 << V_LOW_PATT_Pos)						|
									(1 << W_HIGH_PATT_Pos)						|
									(1 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) |
								((	(0 << U_HIGH_PATT_Pos)						|
									(0 << U_LOW_PATT_Pos)						|
									(0 << V_HIGH_PATT_Pos)						|
									(0 << V_LOW_PATT_Pos)						|
									(0 << W_HIGH_PATT_Pos)						|
									(1 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos);
				break;
				case HALL_STATE::HALL_STATE_3:
				TCC0->PATT.reg =	((	(0 << U_HIGH_PATT_Pos)						|
										(0 << U_LOW_PATT_Pos)						|
										(1 << V_HIGH_PATT_Pos)						|
										(1 << V_LOW_PATT_Pos)						|
										(1 << W_HIGH_PATT_Pos)						|
										(1 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) |
									((	(0 << U_HIGH_PATT_Pos)						|
										(0 << U_LOW_PATT_Pos)						|
										(0 << V_HIGH_PATT_Pos)						|
										(0 << V_LOW_PATT_Pos)						|
										(0 << W_HIGH_PATT_Pos)						|
										(1 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos);
				break;
				case HALL_STATE::HALL_STATE_4:
				TCC0->PATT.reg =	((	(0 << U_HIGH_PATT_Pos)						|
										(0 << U_LOW_PATT_Pos)						|
										(1 << V_HIGH_PATT_Pos)						|
										(1 << V_LOW_PATT_Pos)						|
										(1 << W_HIGH_PATT_Pos)						|
										(1 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) |
									((	(0 << U_HIGH_PATT_Pos)						|
										(0 << U_LOW_PATT_Pos)						|
										(0 << V_HIGH_PATT_Pos)						|
										(1 << V_LOW_PATT_Pos)						|
										(0 << W_HIGH_PATT_Pos)						|
										(0 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos);
				break;
				case HALL_STATE::HALL_STATE_5:
				TCC0->PATT.reg =	((	(1 << U_HIGH_PATT_Pos)						|
										(1 << U_LOW_PATT_Pos)						|
										(1 << V_HIGH_PATT_Pos)						|
										(1 << V_LOW_PATT_Pos)						|
										(0 << W_HIGH_PATT_Pos)						|
										(0 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) |
									((	(0 << U_HIGH_PATT_Pos)						|
										(0 << U_LOW_PATT_Pos)						|
										(0 << V_HIGH_PATT_Pos)						|
										(1 << V_LOW_PATT_Pos)						|
										(0 << W_HIGH_PATT_Pos)						|
										(0 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos);
				break;
				case HALL_STATE::HALL_STATE_6:
				TCC0->PATT.reg =	((	(1 << U_HIGH_PATT_Pos)						|
										(1 << U_LOW_PATT_Pos)						|
										(1 << V_HIGH_PATT_Pos)						|
										(1 << V_LOW_PATT_Pos)						|
										(0 << W_HIGH_PATT_Pos)						|
										(0 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) |
									((	(0 << U_HIGH_PATT_Pos)						|
										(1 << U_LOW_PATT_Pos)						|
										(0 << V_HIGH_PATT_Pos)						|
										(0 << V_LOW_PATT_Pos)						|
										(0 << W_HIGH_PATT_Pos)						|
										(0 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos);
				break;
				case HALL_STATE::UNDEFINED_1:
				case HALL_STATE::UNDEFINED_2:
				TCC0->PATT.reg =	((	(1 << U_HIGH_PATT_Pos)						|
										(1 << U_LOW_PATT_Pos)						|
										(1 << V_HIGH_PATT_Pos)						|
										(1 << V_LOW_PATT_Pos)						|
										(1 << W_HIGH_PATT_Pos)						|
										(1 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) |
									((	(0 << U_HIGH_PATT_Pos)						|
										(0 << U_LOW_PATT_Pos)						|
										(0 << V_HIGH_PATT_Pos)						|
										(0 << V_LOW_PATT_Pos)						|
										(0 << W_HIGH_PATT_Pos)						|
										(0 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos);
				break;
			}
		}
				
	protected:
		MotorDriver();
		~MotorDriver();
		
	private:			
		MotorDriver( const MotorDriver &c );
		MotorDriver& operator=( const MotorDriver &c );

}; //MotorDriver

#endif //__MOTORDRIVER_H__
