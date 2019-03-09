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


#define OP_STATUS			0xD7
#define OP_PAGE_READ_MEM	0xD2
#define OP_PAGE_PROG_BUFF_1 0x82
#define OP_PAGE_PROG_BUFF_2 0x85
#define OP_PAGE_ERASE		0x81

enum TraceType {
	Orientation = 1	
};

typedef struct {
	TraceType _type;
	uint16_t _position;
	uint8_t _data[524];
} TracePage;

class AT45DB : public Executable
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual bool CanExecute() { return IsReady(); };
	virtual RUN_RESULT Run();
	virtual void Propagate();


	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:
	AT45DB();
	private:
	bool IsReady();
	uint8_t TracePage_Read(uint16_t pageIndex, TracePage *page);
	uint8_t TracePage_Write(uint16_t pageIndex, TracePage *page, bool primBuff);
	
}; //AT45DB

#endif //__AT45DB_H__
