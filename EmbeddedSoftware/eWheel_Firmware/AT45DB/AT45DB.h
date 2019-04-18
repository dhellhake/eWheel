/* 
* AT45DB.h
*
* Created: 25.02.2019 20:08:29
* Author: Dominik Hellhake
*/
#ifndef __AT45DB_H__
#define __AT45DB_H__

#include "..\Executable.h"


#define OP_STATUS			0xD7
#define OP_PAGE_READ_MEM	0xD2
#define OP_PAGE_PROG_BUFF_1 0x82
#define OP_PAGE_PROG_BUFF_2 0x85
#define OP_PAGE_ERASE		0x81
#define OP_CHIP_ERASE_1		0xC7
#define OP_CHIP_ERASE_2		0x94
#define OP_CHIP_ERASE_3		0x80
#define OP_CHIP_ERASE_4		0x9A

enum TraceType {
	Orientation = 1	
};

typedef struct {
	TraceType _type;
	uint16_t _position;
	uint8_t _data[524];
} TracePage;




#define TRACEBUFFER_SIZE 4

typedef struct {
	uint8_t _read;
	uint8_t _write;
	TracePage* _page[TRACEBUFFER_SIZE];
} TraceBuffer;

#define TraceBuffer_init(buffer)         { buffer._read = 0; buffer._write = 0; }
#define TraceBuffer_available(buffer)    ( buffer._read != buffer._write )
#define TraceBuffer_GetPage(buffer) (																			\
		(TraceBuffer_available(buffer)) ?																		\
		buffer._page[buffer._read = (buffer._read + 1) & (TRACEBUFFER_SIZE-1)] : 0							\
)
#define TraceBuffer_AddPage(buffer, data) {																		\
		uint8_t tmphead = ( buffer._write + 1 ) & (TRACEBUFFER_SIZE-1);			/* calculate buffer index */    \
		if(tmphead != buffer._read) {											/* if buffer is not full */     \
			buffer._page[tmphead] = data;										/* store data in buffer */      \
			buffer._write = tmphead;											/* store new index */           \
		}																										\
}

class AT45DB : public Executable
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual bool CanExecute() { return TraceBuffer_available(this->Buffer) && IsReady(); };
	virtual RUN_RESULT Run(uint32_t timeStamp);
	virtual void Propagate();


	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:		
		AT45DB();
		uint8_t AddTracePage(TracePage *page);
		uint8_t TracePage_Read(uint16_t pageIndex, TracePage *page);
		void EraseTrace();
		bool IsReady();
		
	private:
		uint16_t PageIndex = 0;
		TraceBuffer Buffer;
		
		uint8_t TracePage_Write(uint16_t pageIndex, TracePage *page, bool primBuff);	
}; //AT45DB

#endif //__AT45DB_H__
