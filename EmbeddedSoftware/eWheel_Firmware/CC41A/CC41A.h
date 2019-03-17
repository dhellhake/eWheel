/* 
* CC41A.h
*
* Created: 09.03.2019 18:06:43
* Author: Dominik Hellhake
*/
#ifndef __CC41A_H__
#define __CC41A_H__

#include "samc21.h"
#include "..\Executable.h"

enum class DEBUG_CMD
{
	SetLED = 65,
	UnSetLED = 66
};

#define RECEIVE_BUFFER_SIZE 64

class CC41A : public Executable
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual bool CanExecute() 
	{
		if (this->ReceiveBufferIndex == 0)
			return false;
		
		switch ((DEBUG_CMD)this->ReceiveBuffer[0])
		{
			case DEBUG_CMD::SetLED:
				return true;
			break;
			case DEBUG_CMD::UnSetLED:
				return true;
			break;
		}		
	};
	virtual RUN_RESULT Run(uint32_t timeStamp);
	virtual void Propagate();


	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	private:	
		uint8_t ReceiveBufferIndex = 0;
		uint8_t ReceiveBuffer[RECEIVE_BUFFER_SIZE];	
	public:	
		CC41A();
		void ReceiveByte(uint8_t data);
}; //CC41A

#endif //__CC41A_H__
