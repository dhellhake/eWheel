/*
 * Executable.h
 *
 * Created: 08.07.2018 09:44:01
 *  Author: dominik hellhake
 */ 


#ifndef EXECUTABLE_H_
#define EXECUTABLE_H_

enum class RUN_RESULT
{
	SUCCESS,
	ERROR,
	NOT_IMPLEMENTED
};

class Executable
{		
	//functions
	public:

	virtual bool CanExecute() { return false; };

	virtual RUN_RESULT Run() { return RUN_RESULT::NOT_IMPLEMENTED; };
		
	virtual void Propagate() {};

}; //Executable

#endif /* EXECUTABLE_H_ */