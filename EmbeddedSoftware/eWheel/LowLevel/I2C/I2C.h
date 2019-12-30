/* 
* I2C.h
*
* Created: 30.05.2018 18:19:17
* Author: Dominik Hellhake
*/
#ifndef __I2C_H__
#define __I2C_H__

#include "samc21.h"
#include "..\Device/DMA/DMA.h"

class I2C
{
	private:
		static uint8_t *buffer_ref;
		static uint16_t tx_index;
		static uint16_t tx_count;

	public:
		static void StartDMACTransfer(DmacDescriptor *descriptor);		
		static inline void Wait()
		{
			while(DMAC_TX_Complete[0] == 0);
		}
}; //I2C

#endif //__I2C_H__
