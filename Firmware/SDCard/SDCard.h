/*
* SDCard.h
*
* Created: 14.05.2021 17:33:49
* Author: dominik hellhake
*/
#ifndef __SDCARD_H__
#define __SDCARD_H__

#include "sam.h"
#include "Disk.h"
#include "..\Task.h"

#define RecordRuntime

typedef enum SDCardState
{
	NoCard =			0x00,
	CardDetect =		0x01,
	CardInitialized =	0x02,
	PartitionValid =	0x03,
	FileFound =			0x04,
	Ready =				0x05,
	Recording =			0x06,
	
	FileNotFound =		0xFE,
	CardError =			0xFF
} SDCardState;

typedef enum RecordType : uint16_t
{
	Runtime =			0xEE,
} RecordType;

typedef struct TargetFile
{
	uint16_t ClusterList[64] = {0};
	uint8_t ClusterListIndex = 0x00;
	uint16_t ClusterIndex = 0x00;
	uint8_t SectorIndex = 0x00;
} TargetFile;


#define SDCard_Buffer_Size	8192

typedef struct {
	uint16_t _read;
	uint16_t _write;
	uint8_t _buffer[SDCard_Buffer_Size];
} FIFO1k_t;

#define FIFO_init(fifo)         { fifo._read = 0; fifo._write = 0; }
#define FIFO_available(fifo)    ( fifo._read != fifo._write)
#define FIFO_read(fifo)			( (FIFO_available(fifo)) ? fifo._buffer[fifo._read = (fifo._read + 1) & (SDCard_Buffer_Size-1)] : 0 )
#define FIFO_write(fifo, data)	{ fifo._buffer[fifo._write] = data;									\
								  fifo._write = ( fifo._write + 1 ) & (SDCard_Buffer_Size-1);		\
								}

const char TargetFileName[] = { 'R', 'E', 'C', 'O', 'R', 'D', '0' };

class SDCard : public Task
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual RUN_RESULT Run(uint32_t timeStamp);
	virtual RUN_RESULT Setup(uint32_t timeStamp);

	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:
		TargetFile tFile;
		uint32_t RelativeCardAddress = 0x00;
		
		MBR MasterBootRecord;
		VolumeID Volume;
		
		FIFO1k_t RecordBuffer;
		uint16_t RecordBufferDataCnt = 0;
		
		uint32_t CurentLBA;
	
		uint32_t ReadPartition();
		uint32_t ReadFilePosition();
		uint32_t ReadRootDirectoryEntries();
		
		uint32_t WriteRecords();
	
	public:
		SDCardState State = SDCardState::CardDetect;
		SDCard();
	
		inline uint32_t AddRecord(uint8_t* data) 
		{			
			for (uint16_t idx = 0; idx < 1024; idx++)
			{
				FIFO_write(this->RecordBuffer, data[idx]);
				if (data[idx] == 0xAA)
					if (data[idx-1] == 0x55)
					{
						this->RecordBufferDataCnt += (idx + 1);
						break;
					}
			}
		}
}; //SDCard

extern SDCard Disk;

#endif //__SDCARD_H__
