/* 
* Chassis.h
*
* Created: 28.04.2019 20:30:50
* Author: Dominik Hellhake
*/
#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "samc21.h"
#include "..\Executable.h"

#define CHASSIS_FLASH_PAGE_BLOCK_CNT	40
#define CHASSIS_FLASH_PAGE_BLOCK_SIZE	12

enum class ChassisState
{
	DroppedOver = 0,
	Starting = 1,
	Normal = 2
};

class Chassis : public Executable
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual RUN_RESULT Run(uint32_t timeStamp);
	
	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:	
		float Chassis_Pitch = 0;
		float Chassis_Roll = 0;
		float Road_Pitch = 0;
		float Road_Roll = 0;
		
		ChassisState State = ChassisState::DroppedOver;
		uint32_t DropOver_Dbnc = 0;
		
		Chassis();
		void SetLED(bool state);
	
	private:		
		uint32_t PageSequence_Ind = 0;
		uint8_t PageBlock_Ind = 0;
		
		inline bool IsRolledOver()
		{
			return this->Chassis_Roll <= -30.0f || this->Chassis_Roll >= 30.0f;
		}
		inline bool IsPitchedOver()
		{
			return this->Chassis_Pitch <= -18.0f || this->Chassis_Pitch >= 18.0f;
		}
		inline bool IsStartPosition()
		{
			return !this->IsRolledOver() && this->Chassis_Pitch > 18.0f;
		}
}; //Chassis

extern Chassis Board;

#endif //__CHASSIS_H__
