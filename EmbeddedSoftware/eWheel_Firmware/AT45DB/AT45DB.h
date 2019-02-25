/* 
* AT45DB.h
*
* Created: 25.02.2019 20:08:29
* Author: Domi
*/


#ifndef __AT45DB_H__
#define __AT45DB_H__

#include "..\LowLevel\SPI\SPI.h"
#include "..\LowLevel\System\System.h"

#include "..\Executable.h"

class AT45DB : public Executable
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual bool CanExecute() { return true; };
	virtual RUN_RESULT Run();
	virtual void Propagate();


	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:
	AT45DB();
	private:
	uint8_t ReadBytes(uint8_t address, uint8_t *dest, uint8_t count);
	
}; //AT45DB

#endif //__AT45DB_H__
