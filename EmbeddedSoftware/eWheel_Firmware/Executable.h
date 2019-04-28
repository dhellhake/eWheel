/*
 * Executable.h
 *
 * Created: 08.07.2018 09:44:01
 *  Author: Dominik Hellhake
 */
#ifndef EXECUTABLE_H_
#define EXECUTABLE_H_

#include "samc21.h"

enum class RUN_RESULT
{
	SUCCESS,
	ERROR,
	NOT_IMPLEMENTED
};

enum class TASK_STATUS
{
	WAIT,
	SUSPEND,
	COMPLETE	
};

class Executable
{
	public:
	volatile TASK_STATUS Status = TASK_STATUS::WAIT;
	
	volatile bool TraceEnabled = false;
	
	volatile uint32_t LastExecuted = 0x00;
	
	virtual bool IsReady(uint32_t timeStamp) { return false; }
	
	virtual RUN_RESULT Run(uint32_t timeStamp) { return RUN_RESULT::NOT_IMPLEMENTED; };
		
	virtual void EnableTrace() {};
	
	virtual void DisableTrace() {};
}; //Executable

#endif /* EXECUTABLE_H_ */