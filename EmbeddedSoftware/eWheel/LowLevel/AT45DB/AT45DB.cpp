/* 
* AT45DB.cpp
*
* Created: 25.02.2019 20:08:29
* Author: Dominik Hellhake
*/
#include <string.h>
#include <stdlib.h>
#include "AT45DB.h"
#include "..\Device/SERCOM/SERCOM.h"

AT45DB Flash;

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
AT45DB::AT45DB()
{
	this->PageWrite_Ind = 1;
}

///<summary>
///Uses the Status Register Read Command (0xD7) to read the devises ready-status (Bit 7)
///</summary>
///<returns>1 if device is ready</returns>
bool AT45DB::IsReady()
{
	//CS Low
	PORT->Group[0].OUTCLR.reg = PORT_PA08;
	
	SERCOM0_TransmitByte(OP_STATUS);
	uint8_t result = SERCOM0_TransmitByte(0x00);
	
	//CS High
	PORT->Group[0].OUTSET.reg = PORT_PA08;
	
	return (result >> 7);
}

void AT45DB::Erase()
{
	//CS Low
	PORT->Group[0].OUTCLR.reg = PORT_PA08;
	
	SERCOM0_TransmitByte(OP_CHIP_ERASE_1);
	SERCOM0_TransmitByte(OP_CHIP_ERASE_2);
	SERCOM0_TransmitByte(OP_CHIP_ERASE_3);
	SERCOM0_TransmitByte(OP_CHIP_ERASE_4);
	
	//CS High
	PORT->Group[0].OUTSET.reg = PORT_PA08;	
}

///<summary>
///Uses the Main Memory Page Read Command (0xD2) to read 528 bytes out of a certain page
///</summary>
///<param name="pageIndex">Index of the page to write to (0 to 8192)</param>
///<param name="dest">data-buffer holding at least 528 bytes to write the page content to</param>
///<returns>1 on success</returns>
uint8_t AT45DB::MemPage_Read(uint16_t pageIndex, FlashPage *page)
{
	uint32_t op_addr = 0x00000000;
	op_addr |= (OP_PAGE_READ_MEM << 24);
	op_addr |= (pageIndex << 10) & 0b11111111111110000000000;
	
	uint8_t tmp[4];
	memcpy(tmp, (const char *) &(op_addr), sizeof(uint32_t));
	
	page->PageIndex = pageIndex;
	
	//CS Low
	PORT->Group[0].OUTCLR.reg = PORT_PA08;
	
	// Send OP-Code D2 & page/byte address
	for (uint8_t x = 0; x < 4; x++)
		SERCOM0_TransmitByte(tmp[3 - x]);
	
	// Send dont-care bytes
	for (uint8_t x = 0; x < 4; x++)
		SERCOM0_TransmitByte(0x00);
	
	// Read page
	for (uint16_t x = 0; x < FLASHPAGE_SIZE; x++)
		page->Data[x] = SERCOM0_TransmitByte(0x00);
	
	//CS High
	PORT->Group[0].OUTSET.reg = PORT_PA08;
		
	return 1;
}

uint8_t AT45DB::MemPage_Write(FlashPage *page, bool primBuff)
{
	if (this->PageWrite_Ind >= FLASHPAGE_CNT)
		this->PageWrite_Ind = 0;
		
	this->PageWrite_Ind++;
	return this->MemPage_Write(this->PageWrite_Ind - 1, page, primBuff);
}

///<summary>
///Uses the Main Memory Page Program through Buffer Command (0x82) to write 528 bytes to a certain page
///</summary>
///<param name="pageIndex">Index of the page to write to (0 to 8192)</param>
///<param name="data">data-buffer containing 528 bytes to write to page</param>
///<param name="data">true if Buffer 1 should be used. False for Buffer 2</param>
///<returns>1 on success</returns>
uint8_t AT45DB::MemPage_Write(uint16_t pageIndex, FlashPage *page, bool primBuff)
{	
	uint32_t op_addr = 0x00000000;
	
	if (primBuff)
		op_addr |= (OP_PAGE_PROG_BUFF_1 << 24);
	else
		op_addr |= (OP_PAGE_PROG_BUFF_2 << 24);
	
	op_addr |= (pageIndex << 10) & 0b11111111111110000000000;
	
	uint8_t tmp[4];
	memcpy(tmp, (const char *) &(op_addr), sizeof(uint32_t));
	
	//CS Low
	PORT->Group[0].OUTCLR.reg = PORT_PA08;
	
	// Send OP-Code & page/byte address
	for (uint8_t x = 0; x < 4; x++)
		SERCOM0_TransmitByte(tmp[3 - x]);
	
	// Write page
	for (uint16_t x = 0; x < FLASHPAGE_SIZE; x++)
		SERCOM0_TransmitByte(page->Data[x]);
	
	//CS High
	PORT->Group[0].OUTSET.reg = PORT_PA08;
	
	return 1;
}