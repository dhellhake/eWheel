/* 
* I2C.h
*
* Created: 30.05.2018 18:19:17
* Author: domin
*/


#ifndef __I2C_H__
#define __I2C_H__

#include "sam.h"
#include "..\System\System.h"

class I2C
{
//variables
public:
protected:
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
protected:
	I2C();
	~I2C();
private:
	I2C( const I2C &c );
	I2C& operator=( const I2C &c );
}; //I2C

#endif //__I2C_H__
