/*
 * DSU.cpp
 *
 * Created: 28.03.2020 19:06:19
 *  Author: Dominik Hellhake
 */ 
#include "DSU.h"


uint32_t CalculateCRC32(const uint32_t *addr, const uint32_t len)
{
	uint32_t pcrc32 = 0xFFFFFFFF;


	PAC->WRCTRL.reg = SYSTEM_PERIPHERAL_ID(DSU) | PAC_WRCTRL_KEY(PAC_WRCTRL_KEY_CLR_Val);
	
	DSU->DATA.reg = pcrc32;
	
	uint32_t addrReg = (((uint32_t)addr) & 0xFFFFFFFC);
	DSU->ADDR.reg = addrReg;
	DSU->LENGTH.reg = len;

	
	DSU->CTRL.bit.CRC = 1;
	while ((DSU->STATUSA.reg & DSU_STATUSA_DONE) != 1) { }


	if (DSU->STATUSA.reg & DSU_STATUSA_BERR)
	{
	}
	
	pcrc32 = DSU->DATA.reg;
	pcrc32 = ~pcrc32;
	
	DSU->STATUSA.reg = DSU_STATUSA_DONE;	
	PAC->WRCTRL.reg = SYSTEM_PERIPHERAL_ID(DSU) | PAC_WRCTRL_KEY(PAC_WRCTRL_KEY_CLR_Val);
	
	return pcrc32;
}