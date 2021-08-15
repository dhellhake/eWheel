/* 
* SDCard.cpp
*
* Created: 14.05.2021 17:33:49
* Author: dominik hellhake
*/
#include "SDCard.h"
#include "..\ATSAME70\HSMCI\HSMCIlib.h"

SDCard Disk;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT SDCard::Run(uint32_t timeStamp)
{
	RUN_RESULT result = RUN_RESULT::IDLE;
		
	switch (this->State)
	{
		case SDCardState::NoCard:
		break;
		case SDCardState::CardDetect:
			if ((this->RelativeCardAddress = InitSDCard()) != 0x00)
				this->State = SDCardState::CardInitialized;
			else
				this->State = SDCardState::CardError;
		break;
		case SDCardState::CardInitialized:
			if (this->ReadPartition() != 0x00)
				this->State = SDCardState::PartitionValid;
			else
				this->State = SDCardState::CardError;				
		break;
		case SDCardState::PartitionValid:
			if ((this->tFile.ClusterList[0] = this->ReadRootDirectoryEntries()) != 0x00)
			{
				this->CurentLBA = this->MasterBootRecord.LBA_begin + (uint32_t)this->Volume.BPB_RsvdSecCnt;
				this->CurentLBA += (this->tFile.ClusterList[0] - (this->tFile.ClusterList[0] % 128)) / 128;
				this->State = SDCardState::FileFound;
			}
			else
				this->State = SDCardState::FileNotFound;	
		break;
		case SDCardState::FileFound:
		{
			uint32_t result = this->ReadFilePosition();
			switch (result)
			{
				case 0x00:	// EOF found.				
					this->State = SDCardState::Ready;
				break;
				case 0x01: // continue with next block of FAT
					this->State = SDCardState::FileFound;
				break;
				default: // error
					this->State = SDCardState::FileNotFound;
				break;
			}		
		}
		break;
		case SDCardState::Ready:
		
		break;
		case SDCardState::FileNotFound:		
		case SDCardState::CardError:
		break;	
	}
		
		
	return result;
}


uint32_t SDCard::ReadPartition()
{
	uint8_t rxd[512] = { 0 };
	
	//	Read MBR
	ReadSector(this->RelativeCardAddress, 0, (uint32_t*)rxd);
	this->MasterBootRecord.TypeCode =	*((uint8_t  *)(rxd+446+4));
	this->MasterBootRecord.LBA_begin =	*((uint32_t  *)(rxd+446+8));
	
	if (this->MasterBootRecord.TypeCode != 0x0C)
		return 0;
	
	// Read Volume ID
	ReadSector(this->RelativeCardAddress, this->MasterBootRecord.LBA_begin, (uint32_t*)rxd);
	this->Volume.BPB_BytesPerSec =	*((uint16_t *)(rxd+0x0B));
	this->Volume.BPB_SecPerClus =	*((uint8_t  *)(rxd+0x0D));
	this->Volume.BPB_RsvdSecCnt =	*((uint16_t *)(rxd+0x0E));
	this->Volume.BPB_NumFATs =		*((uint8_t  *)(rxd+0x10));
	this->Volume.BPB_FATSz32 =		*((uint32_t *)(rxd+0x24));
	this->Volume.BPB_RootClus =		*((uint32_t *)(rxd+0x2C));
	
	if (this->Volume.BPB_BytesPerSec != 512)
		return 0;
		
	uint32_t fat_begin_lba = this->MasterBootRecord.LBA_begin + (uint32_t)this->Volume.BPB_RsvdSecCnt;
	uint32_t cluster_begin_lba = fat_begin_lba + (this->Volume.BPB_NumFATs * this->Volume.BPB_FATSz32);
	this->CurentLBA = cluster_begin_lba + (this->Volume.BPB_RootClus - 2) * this->Volume.BPB_SecPerClus;
		
	return 1;
}


/************************************************************************/
/* Read File-List from Directory Table									*/
/* Assumption:															*/
/* Target-File is located in Root directory which contains				*/
/* less than 128 file. File is therefore located in first block of      */
/* Directory-Table.														*/
/************************************************************************/
uint32_t SDCard::ReadRootDirectoryEntries()
{
	uint8_t rxd[512] = { 0 };	
	
	//  Read Root Directory
	ReadSector(this->RelativeCardAddress, this->CurentLBA, (uint32_t*)rxd);
	
	uint32_t filePosition = 0x00;
	for (uint8_t entryId = 0; entryId < (512 / 32); entryId++)
	{		
		DirectoryEntry *entry = (DirectoryEntry*)(rxd + (entryId * 32));
		
		if (entry->Filename[0] == 0x00)
			continue;
		
		filePosition = entryId;
		for (uint8_t charIdx = 0; charIdx < sizeof(TargetFileName); charIdx++)
		{
			if (entry->Filename[charIdx] != TargetFileName[charIdx])
			{
				filePosition = 0x00;
				break;
			}			
		}
		
		if (filePosition != 0x00)
		{
			filePosition = (entry->LBA_High << 16) | entry->LBA_Low;
			break;
		}
	}
		
	return filePosition;
}

uint32_t SDCard::ReadFilePosition()
{
	uint8_t rxd[512] = { 0 };
	
	//  Read File Allocation Table
	ReadSector(this->RelativeCardAddress, this->CurentLBA, (uint32_t*)rxd);
	
	uint16_t slotStart = 0x00;
	if (this->tFile.ClusterListIndex == 0x00)
	{
		slotStart = this->tFile.ClusterList[0];
		this->tFile.ClusterList[1] = this->tFile.ClusterList[0]; 
		this->tFile.ClusterListIndex = 1;
	}
	
	for (; slotStart < 128; slotStart++)
	{
		uint32_t slot = *((uint32_t*)(rxd + (slotStart * 4)));
		
		if (slot == 0x0FFFFFFF)
			return 0; // EOF
		
		if ((uint32_t)(this->tFile.ClusterList[this->tFile.ClusterListIndex] + 0x0001) != slot) // Jump in non-continuous cluster-chain
			this->tFile.ClusterListIndex++;
		
		this->tFile.ClusterList[this->tFile.ClusterListIndex] = slot;	
	}
	this->CurentLBA++;
		
	return 1;
}


RUN_RESULT SDCard::Setup(uint32_t timeStamp)
{
	
	return RUN_RESULT::SUCCESS;
}