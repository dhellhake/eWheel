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

class Executable
{
	public:
	volatile bool TraceEnabled = false;

	virtual bool CanExecute(uint32_t timeStamp) { return false; };

	virtual RUN_RESULT Run(uint32_t timeStamp) { return RUN_RESULT::NOT_IMPLEMENTED; };
		
	virtual void Propagate() {};
		
	virtual void EnableTrace() {};
	
	virtual void DisableTrace() {};
}; //Executable

#endif /* EXECUTABLE_H_ */