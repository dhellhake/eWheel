/* 
* AT45DB.h
*
* Created: 25.02.2019 20:08:29
* Author: Dominik Hellhake
*/
#ifndef __AT45DB_H__
#define __AT45DB_H__

#include "samc21.h"


#define FLASHBUFFER_SIZE 4

#define FLASHPAGE_CNT		8192
#define FLASHPAGE_SIZE		528

#define OP_STATUS			0xD7
#define OP_PAGE_READ_MEM	0xD2
#define OP_PAGE_PROG_BUFF_1 0x82
#define OP_PAGE_PROG_BUFF_2 0x85
#define OP_PAGE_ERASE		0x81
#define OP_CHIP_ERASE_1		0xC7
#define OP_CHIP_ERASE_2		0x94
#define OP_CHIP_ERASE_3		0x80
#define OP_CHIP_ERASE_4		0x9A


typedef struct {
	uint16_t PageIndex;
	uint8_t Data[FLASHPAGE_SIZE];
} FlashPage;

class AT45DB
{
	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:		
		AT45DB();
		void Erase();
		bool IsReady();
		static uint8_t MemPage_Read(uint16_t pageIndex, FlashPage *page);
		static uint8_t MemPage_Write(uint16_t pageIndex, FlashPage *page, bool primBuff);
		
		uint8_t MemPage_Write(FlashPage *page, bool primBuff);
	private:
		uint16_t PageWrite_Ind;		
		
}; //AT45DB

extern AT45DB Flash;

#endif //__AT45DB_H__
