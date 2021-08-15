/*
 * Disk.h
 *
 * Created: 14.05.2021 22:42:37
 *  Author: dominik hellhake
 */ 
#ifndef DISK_H_
#define DISK_H_

typedef struct VolumeID
{
	uint16_t BPB_BytesPerSec;
	uint8_t BPB_SecPerClus;
	uint16_t BPB_RsvdSecCnt;
	uint8_t BPB_NumFATs;
	uint32_t BPB_FATSz32;
	uint32_t BPB_RootClus;
} VolumeID;

typedef struct MBR
{
	uint8_t TypeCode;
	uint32_t LBA_begin;
} MBR;

typedef struct DirectoryEntry
{
	uint8_t Filename[11];
	uint8_t Attributes;
	uint8_t reserved;
	uint8_t Created[5];
	uint8_t Accessed[2];
	uint16_t LBA_High;
	uint8_t Changed[4];
	uint16_t LBA_Low;
	uint8_t Size[4];
} DirectoryEntry;

#endif /* DISK_H_ */