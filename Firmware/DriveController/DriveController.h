/* 
* ESC.h
*
* Created: 14.04.2019 15:02:13
* Author: Dominik Hellhake
*/
#ifndef __ESC_H__
#define __ESC_H__

#include "sam.h"
#include "..\Task.h"

class DriveController : public Task
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual RUN_RESULT Run(uint32_t timeStamp);
	virtual RUN_RESULT Setup(uint32_t timeStamp);

	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/		
	public:
		
		// Duty Cycle [0.0 to 1.0]
		float Tar_Duty;
		
		DriveController();
		
		void Stop();
		
		inline void ResetTarValues()
		{
			this->Tar_Duty = 0.00f;
		}		
		
		inline void SetTar_Duty(float value)
		{
			this->ResetTarValues();
			
			if (value > 1.0f)
				this->Tar_Duty = 1.0f;
			else if (value < -1.0f)
				this->Tar_Duty = -1.0f;
			else
				this->Tar_Duty = value;
		}
		
		void ReceiveByte(uint8_t data);
		
	private:	
		
}; //DriveController

extern DriveController Drive;

#endif //__ESC_H__
