/* 
* W25Q128.h
*
* Created: 24.05.2020 13:20:15
* Author: dominik hellhake
*/
#ifndef __W25Q128_H__
#define __W25Q128_H__

#include "sam.h"
#include "..\..\Task.h"

#define W25Q_SPI			SPI

#define W25Q_PAGE_SIZE		256
#define W25Q_PAGE_COUNT		65536
#define W25Q_SECTOR_COUNT	4096

#define BUFFER_SIZE 512
#define BUFFER_available(buffer)    ( buffer._read != buffer._write )

#define BUFFER_read(buffer)	 (														\
		(BUFFER_available(buffer)) ?												\
		buffer._buffer[buffer._read = (buffer._read + 1) & (BUFFER_SIZE-1)] : 0		\
)

#define BUFFER_write(buffer, data) {																\
	uint16_t tmphead = ( buffer._write + 1 ) & (BUFFER_SIZE-1);      /* calculate buffer index */    \
	if(tmphead != buffer._read) {									/* if buffer is not full */     \
		buffer._buffer[tmphead] = data;								/* store data in buffer */      \
		buffer._write = tmphead;									/* store new index */           \
	}																								\
}

typedef enum W25Q_RESULT
{
	SUCCESS,
	BUSY
} W25Q_RESULT;

typedef struct W25Q_PAGE
{
	uint16_t Index;
	uint8_t data[W25Q_PAGE_SIZE];	
} W25Q_PAGE;

typedef struct {
	uint16_t _read;
	uint16_t _write;
	uint8_t _buffer[BUFFER_SIZE];
} BUFFER_t;

class W25Q128 : public Task
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual RUN_RESULT Run(uint32_t timeStamp);
	virtual RUN_RESULT Setup(uint32_t timeStamp);

	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	private:
		BUFFER_t WriteBuffer;
	
		uint16_t PageWrite_Index;
	
		void WriteEnable();
		void WriteData();
		W25Q_RESULT PageProgram(uint16_t index, uint8_t *data);
		
	public:
	uint16_t BufferWriteCount;
		W25Q128();
	
		void ReadJEDEC();
		uint8_t ReadStatus();
		
		W25Q_RESULT ChipErase();
		W25Q_RESULT SectorErase(uint16_t index);
		W25Q_RESULT PageRead(uint16_t index, uint8_t *data);	
		
		void WriteData(uint8_t *data, uint8_t length);
		
		volatile inline bool IsBusy()
		{
			uint8_t status = this->ReadStatus();
			
			return (status & 0x01) == 0x01;
		}
		
}; //W25Q128

extern W25Q128 DataFlash;

#endif //__W25Q128_H__
