/*
 * SERCOM.h
 *
 * Created: 15.03.2019 15:10:07
 *  Author: Dominik Hellhake
 */
#ifndef SERCOM_H_
#define SERCOM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "samc21.h"
#include "..\..\..\Utilities.h"

void InitSERCOM0();
uint8_t SERCOM0_TransmitByte(uint8_t byte);

void InitSERCOM1();
void SERCOM1_SendByte(uint8_t byte);

void InitSERCOM2();
uint8_t SERCOM2_TransmitByte(uint8_t byte);

void InitSERCOM3();
void SERCOM3_SendByte(uint8_t byte);

void InitSERCOM5();
void SERCOM5_SendByte(uint8_t byte);

inline uint16_t calculate_baud_value(const uint32_t baudrate, const uint32_t peripheral_clock, uint8_t sample_num)
{
	/* Temporary variables */
	uint64_t ratio = 0;
	uint64_t scale = 0;
	uint64_t baud_calculated = 0;
	uint64_t temp1;
	/* Calculate the BAUD value */
	temp1 = ((sample_num * (uint64_t)baudrate) << 32);
	ratio = long_division(temp1, peripheral_clock);
	scale = ((uint64_t)1 << 32) - ratio;
	baud_calculated = (65536 * scale) >> 32;
	return baud_calculated;
}

#ifdef __cplusplus
}
#endif

#endif /* SERCOM_H_ */