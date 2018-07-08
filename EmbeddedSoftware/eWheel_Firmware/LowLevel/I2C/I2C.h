/* 
* I2C.h
*
* Created: 30.05.2018 18:19:17
* Author: dominik hellhake
*/

#ifndef __I2C_H__
#define __I2C_H__

#include "sam.h"
#include "..\System\System.h"

class I2C
{
//variables
private:
	static uint8_t *buffer_ref;
	static uint16_t tx_index;
	static uint16_t tx_count;


//functions
public:
	static void InitSERCOM();	
	static void InitDMAC();	
	static void StartDMACTransfer(DmacDescriptor *descriptor);		
	static inline void Wait()
	{
		while(!System::DMAC_TX_Complete[0]);
	}
}; //I2C

#endif //__I2C_H__
