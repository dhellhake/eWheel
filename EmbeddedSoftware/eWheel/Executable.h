/*
 * Executable.h
 *
 * Created: 08.07.2018 09:44:01
 *  Author: Dominik Hellhake
 */
#ifndef EXECUTABLE_H_
#define EXECUTABLE_H_

#include "sam.h"

enum class RUN_RESULT
{
	SUCCESS,
	IDLE,
	ERROR,
	NOT_IMPLEMENTED
};

class Executable
{
	public:	
		virtual RUN_RESULT Run(uint32_t timeStamp) { return RUN_RESULT::NOT_IMPLEMENTED; };
		
		virtual void EnableTrace() { this->TraceEnabled = true; };
		
		virtual void DisableTrace() { this->TraceEnabled = false; };
		
		uint32_t LAST_RUNNED = 0;
		
		bool TraceEnabled = false;
}; //Executable

#endif /* EXECUTABLE_H_ */