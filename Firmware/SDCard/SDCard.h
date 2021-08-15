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

typedef enum SDCardState
{
	NoCard =			0x00,
	CardDetect =		0x01,
	CardInitialized =	0x02,
	PartitionValid =	0x03,
	FileFound =			0x04,
	Ready =				0x05,
	
	FileNotFound =		0xFE,
	CardError =			0xFF
} SDCardState;

typedef struct TargetFile
{
	uint16_t ClusterList[64] = {0};
	uint8_t ClusterListIndex = 0x00;
		
} TargetFile;

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
	private:
		SDCardState State = SDCardState::CardDetect;
		TargetFile tFile;
		uint32_t RelativeCardAddress = 0x00;
		
		MBR MasterBootRecord;
		VolumeID Volume;
		
		uint32_t CurentLBA;
	
		uint32_t ReadPartition();
		uint32_t ReadFilePosition();
		uint32_t ReadRootDirectoryEntries();
	
}; //SDCard

extern SDCard Disk;

#endif //__SDCARD_H__
