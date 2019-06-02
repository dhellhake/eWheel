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
	IDLE,
	ERROR,
	NOT_IMPLEMENTED
};

enum class TASK_STATUS
{
	RUNNING,
	COMPLETE	
};

class Executable
{
	public:	
		virtual RUN_RESULT Run(uint32_t timeStamp) { return RUN_RESULT::NOT_IMPLEMENTED; };
		
		virtual void EnableTrace() {};
		
		virtual void DisableTrace() {};
		
		virtual void Reset() { this->TaskStatus = TASK_STATUS::RUNNING; }
		
		TASK_STATUS TaskStatus = TASK_STATUS::RUNNING;
		
		uint32_t LAST_RUNNED = 0;
}; //Executable

#endif /* EXECUTABLE_H_ */