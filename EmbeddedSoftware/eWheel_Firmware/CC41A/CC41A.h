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

class CC41A : public Executable
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual bool CanExecute() 
	{ 
		return true; 
	};
	virtual RUN_RESULT Run();
	virtual void Propagate();


	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:
		CC41A();
}; //CC41A

#endif //__CC41A_H__
