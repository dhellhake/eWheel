/*
 * DMA.c
 *
 * Created: 15.03.2019 14:28:16
 *  Author: Dominik Hellhake
 */
#include <string.h>
#include "DMA.h"
 
volatile uint8_t DMAC_TX_Complete[1] = { 1 };

COMPILER_ALIGNED(16)
DmacDescriptor descriptor_section[1] SECTION_DMAC_DESCRIPTOR;

/** Initial write back memory section. */
COMPILER_ALIGNED(16)
static DmacDescriptor _write_back_section[1] SECTION_DMAC_DESCRIPTOR;


void InitDMAC()
{
	MCLK->AHBMASK.reg |= MCLK_AHBMASK_DMAC;
 
	/* Perform a software reset before enable DMA controller */
	DMAC->CTRL.reg &= ~DMAC_CTRL_DMAENABLE;
	DMAC->CTRL.reg = DMAC_CTRL_SWRST;
 
	/* Setup descriptor base address and write back section base address */
	DMAC->BASEADDR.reg = (uint32_t)descriptor_section;
	DMAC->WRBADDR.reg = (uint32_t)_write_back_section;
 
	/* Enable all priority level at the same time */
	DMAC->CTRL.reg = DMAC_CTRL_DMAENABLE | DMAC_CTRL_LVLEN(0xf);
}
 
void SetDMAChannel0()
{
	DMAC->CHID.reg = DMAC_CHID_ID(0);
	DMAC->CHCTRLA.reg &= ~DMAC_CHCTRLA_ENABLE;
	DMAC->CHCTRLA.reg = DMAC_CHCTRLA_SWRST;
	DMAC->CHCTRLA.reg |= DMAC_CHCTRLA_RUNSTDBY;
	 
	DMAC->SWTRIGCTRL.reg &= (uint32_t)(~(1 << 0));
	 
	DMAC->CHCTRLB.reg =	DMAC_CHCTRLB_LVL(0) | 
						DMAC_CHCTRLB_TRIGSRC(SERCOM4_DMAC_ID_TX) |
						DMAC_CHCTRLB_TRIGACT(DMAC_CHCTRLB_TRIGACT_BEAT_Val);
	 
	/* Enable DMAC interrupt */
	NVIC->ISER[0] = (uint32_t)(1 << ((uint32_t)DMAC_IRQn & 0x0000001f));
	DMAC->CHINTENSET.reg = DMAC_CHINTENSET_TERR | DMAC_CHINTENSET_TCMPL | DMAC_CHINTENSET_SUSP;
}

void StartDMATransfer(DmacDescriptor *descriptor, uint8_t dmaSlot)
{
	DMAC_TX_Complete[dmaSlot] = 0;
	/* Set dma-descriptor to the target slot base address */
	memcpy(&descriptor_section[dmaSlot], descriptor, sizeof(DmacDescriptor));
	/* Enable the transfer channel */
	DMAC->CHCTRLA.reg |= DMAC_CHCTRLA_ENABLE;
}

void DMAC_Handler(void)
{
	DMAC_TX_Complete[DMAC->INTPEND.bit.ID] = 1;
 
	DMAC->CHINTFLAG.bit.TCMPL = 1;
}

