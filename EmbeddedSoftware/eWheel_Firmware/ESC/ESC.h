/* 
* ESC.h
*
* Created: 14.04.2019 15:02:13
* Author: Dominik Hellhake
*/
#ifndef __ESC_H__
#define __ESC_H__

#include "samc21.h"
#include "..\Executable.h"

class ESC : public Executable
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual bool CanExecute()
	{
		return false;
	};
	
	virtual RUN_RESULT Run(uint32_t timeStamp);
	virtual void Propagate();


	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:
	ESC();
}; //ESC

#endif //__ESC_H__
