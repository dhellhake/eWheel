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

class MotorDriver
{
	//variables
	public:
		volatile static uint8_t PhaseDuty;
	protected:
	private:

	//functions
	public:
		static void InitTC0();	//ToBeProtected
		static void InitTC1();	//ToBeProtected
		static void InitTC3();	//ToBeProtected
		static void SetSlowDrive();
		static void SetDrive();
		
		static inline void SetUHigh()
		{
			TC1->COUNT8.CC[0].reg = 0x00;						// U_Low off
			TC1->COUNT8.CC[1].reg = MotorDriver::PhaseDuty;		// U_High on
		}		
		static inline void SetUOpen()
		{
			TC1->COUNT8.CC[0].reg = 0x00;						// U_Low off
			TC1->COUNT8.CC[1].reg = 0x00;						// U_High off
		}		
		static inline void SetULow()
		{
			TC1->COUNT8.CC[1].reg = 0x00;						// U_High off
			TC1->COUNT8.CC[0].reg = MotorDriver::PhaseDuty;		// U_Low on
		}
	
		static inline void SetVHigh()
		{
			TC0->COUNT8.CC[0].reg = 0x00;						// V_Low off
			TC0->COUNT8.CC[1].reg = MotorDriver::PhaseDuty;		// V_High on
		}
		static inline void SetVOpen()
		{
			TC0->COUNT8.CC[0].reg = 0x00;						// V_Low off
			TC0->COUNT8.CC[1].reg = 0x00;						// V_High off
		}
		static inline void SetVLow()
		{
			TC0->COUNT8.CC[1].reg = 0x00;						// V_High off
			TC0->COUNT8.CC[0].reg = MotorDriver::PhaseDuty;		// V_Low on
		}
	
		static inline void SetWHigh()
		{
			TC3->COUNT8.CC[0].reg = 0x00;						// W_Low off
			TC3->COUNT8.CC[1].reg = MotorDriver::PhaseDuty;		// W_High on
		}
		static inline void SetWOpen()
		{
			TC3->COUNT8.CC[0].reg = 0x00;						// W_Low off
			TC3->COUNT8.CC[1].reg = 0x00;						// W_High off
		}
		static inline void SetWLow()
		{
			TC3->COUNT8.CC[1].reg = 0x00;						// W_High off
			TC3->COUNT8.CC[0].reg = MotorDriver::PhaseDuty;		// W_Low on
		}
		
	protected:
		MotorDriver();
		~MotorDriver();
	
	private:
		MotorDriver( const MotorDriver &c );
		MotorDriver& operator=( const MotorDriver &c );

}; //MotorDriver

#endif //__MOTORDRIVER_H__
