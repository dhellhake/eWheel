/* 
* I2C.cpp
*
* Created: 30.05.2018 18:19:17
* Author: Dominik Hellhake
*/
#include "I2C.h"

uint16_t I2C::tx_index = 0;
uint16_t I2C::tx_count = 0;
uint8_t *I2C::buffer_ref = 0x00;

void I2C::StartDMACTransfer(DmacDescriptor *descriptor)
{
	StartDMATransfer(descriptor, 0);
}
