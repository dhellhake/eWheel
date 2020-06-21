/*
 * Executable.h
 *
 * Created: 08.07.2018 09:44:01
 *  Author: Dominik Hellhake
 */
#ifndef TASK_H_
#define TASK_H_

#include "sam.h"

enum class RUN_RESULT
{
	SUCCESS,
	IDLE,
	ERROR,
	NOT_IMPLEMENTED
};

class Task
{
	public:	
		virtual RUN_RESULT Run(uint32_t timeStamp) { return RUN_RESULT::NOT_IMPLEMENTED; };
}; //Task

#endif /* TASK_H_ */