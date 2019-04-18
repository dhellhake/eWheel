/* 
* CANCont.cpp
*
* Created: 12.04.2019 22:25:31
* Author: Dominik Hellhake
*/
#include "CANCont.h"
#include "..\..\..\System\System.h"

COMPILER_ALIGNED(4)
struct can_rx_element_fifo_0 can0_rx_fifo_0[CONF_CAN0_RX_FIFO_0_NUM];


void InitCAN0()
{
	//Set bits in the clock mask for an APBx bus.
	MCLK->AHBMASK.bit.CAN0_ = 1;
	
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[CAN0_GCLK_ID].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[CAN0_GCLK_ID].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[CAN0_GCLK_ID].reg = GCLK_PCHCTRL_GEN(0);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[CAN0_GCLK_ID].reg |= GCLK_PCHCTRL_CHEN;
	
	
	/* Configuration Change Enable. */
	CAN0->CCCR.reg |= CAN_CCCR_CCE;
	
	CAN0->NBTP.reg =	CAN_NBTP_NBRP(CONF_CAN_NBTP_NBRP_VALUE)			|
	CAN_NBTP_NSJW(CONF_CAN_NBTP_NSJW_VALUE)			|
	CAN_NBTP_NTSEG1(CONF_CAN_NBTP_NTSEG1_VALUE)		|
	CAN_NBTP_NTSEG2(CONF_CAN_NBTP_NTSEG2_VALUE);
	CAN0->DBTP.reg =	CAN_DBTP_DBRP(CONF_CAN_DBTP_DBRP_VALUE) |
	CAN_DBTP_DSJW(CONF_CAN_DBTP_DSJW_VALUE) |
	CAN_DBTP_DTSEG1(CONF_CAN_DBTP_DTSEG1_VALUE) |
	CAN_DBTP_DTSEG2(CONF_CAN_DBTP_DTSEG2_VALUE);
	
	/* --- Acceptance Filtering --- */
	/* Global Filter Configuration */
	CAN0->GFC.reg |= (CAN_GFC_RRFE | CAN_GFC_RRFS) |			//Reject Can Remote Frames	(CAN_GFC_ANFS_RXF0 | CAN_GFC_ANFE_RXF0);	// Accept Non-matching Frames Standard&Extended
	/* Standard ID Filter Configuration */
	//No filter for Standard ID
	CAN0->SIDFC.reg = 0;
	/* Extended ID Filter Configuration */
	// List Size Extended = 0
	CAN0->XIDFC.reg = 0;
	
	/* Rx FIFO 0 Configuration */
	CAN0->RXF0C.reg =	CAN_RXF0C_F0SA((uint32_t)can0_rx_fifo_0)	|	// Set FIFO Start Address
	CAN_RXF0C_F0S(CONF_CAN0_RX_FIFO_0_NUM)		|	// Set FIFO Size
	CAN_RXF0C_F0OM;									// Enable Override Mode
	
	
	/* Enable the interrupt setting which no need change. */
	CAN0->ILE.reg = CAN_ILE_EINT0 | CAN_ILE_EINT1;
	CAN0->TXBTIE.reg = CAN_TXBTIE_MASK;
	CAN0->TXBCIE.reg = CAN_TXBCIE_MASK;
	
	CAN0->IE.reg = (1 << 0);		// ?Rx FIFO 0 New Message Interrupt Enable
	
	
	NVIC_SetPriority(CAN0_IRQn, 1);
	NVIC->ISER[0] = (uint32_t)(1 << ((uint32_t)CAN0_IRQn & 0x0000001f));
	
	
	CAN0->CCCR.reg &= ~CAN_CCCR_INIT;
	/* Wait for the sync. */
	while (CAN0->CCCR.reg & CAN_CCCR_INIT);
}

inline int32_t buffer_get_int32(const uint8_t *buffer, uint8_t index) 
{
	return 	(buffer[index] << 24)		|
			(buffer[index + 1]) << 16	|
			(buffer[index + 2]) << 8	|
			(buffer[index + 3]);
}
inline int16_t buffer_get_int16(const uint8_t *buffer, uint8_t index) 
{
	return	(buffer[index]) << 8 |
			(buffer[index + 1]);
}


volatile uint8_t el = 0;
volatile uint32_t elapsed[100] = { 0 };

void CAN0_Handler(void)
{
	uint32_t ir_status = CAN0->IR.reg;
	
	if (ir_status && CAN_IE_RF0NE)
	{
		//Ack interrupt
		CAN0->IR.reg = CAN_IE_RF0NE;
		
		can_rx_element_fifo_0 *received = &can0_rx_fifo_0[receive_index];
		
		uint32_t id = received->R0.bit.ID;
		
		uint8_t vesc_id = id & 0xFF;
		uint8_t vesc_cmd = id >> 8;
		
		if (vesc_cmd == 9)
		{
			uint8_t tmp[8] = {0};
			int32_t index = 0;
			
			for (uint8_t x = 0; x < 8; x++)
				tmp[x] = received->data[x];
			
			float rpm = (float)buffer_get_int32(&tmp[0], 0);
			float curr = (float)buffer_get_int16(&tmp[0], 4) / 10.0f;
			float duty = (float)buffer_get_int16(&tmp[0], 6) / 1000.0f;
			
			
			elapsed[el++] = GetElapsedMilis();
			
			if (el >= 100)
				el = 0;
			
			
			
			
			
		}
		
		//Ack received Message
		CAN0->RXF0A.reg = CAN_RXF0A_F0AI(receive_index);
		
		receive_index++;
		if (receive_index == CONF_CAN0_RX_FIFO_0_NUM)
			receive_index = 0;
	} 
}