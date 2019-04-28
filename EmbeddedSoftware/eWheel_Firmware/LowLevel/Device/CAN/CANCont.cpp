/* 
* CANCont.cpp
*
* Created: 12.04.2019 22:25:31
* Author: Dominik Hellhake
*/
#include "CANCont.h"
#include "..\SysTick\SysTick.h"
#include "..\..\..\System\System.h"

COMPILER_ALIGNED(4)
struct can_rx_element_fifo_0 can0_rx_fifo_0[CONF_CAN0_RX_FIFO_0_NUM];
COMPILER_ALIGNED(4)
static struct can_tx_element can0_tx_buffer[CONF_CAN0_TX_BUFFER_NUM];


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
	CAN0->TXBC.reg =	CAN_TXBC_TBSA((uint32_t)can0_tx_buffer) |
						CAN_TXBC_NDTB(CONF_CAN0_TX_BUFFER_NUM);
	
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

void CAN_SendExtMessage(uint32_t id, uint8_t *data, uint32_t data_length, uint8_t tx_buffer_index)
{
	can0_tx_buffer[tx_buffer_index].T0.bit.ID = id;
	can0_tx_buffer[tx_buffer_index].T0.bit.XTD = 1;
	can0_tx_buffer[tx_buffer_index].T1.bit.DLC = data_length;
	
	for (uint8_t i = 0; i < CONF_CAN_ELEMENT_DATA_SIZE; i++)
		can0_tx_buffer[tx_buffer_index].data[i] = data[i];
		
	while (CAN0->CCCR.reg & CAN_CCCR_CCE) {	}		
	CAN0->TXBAR.reg = (1 << tx_buffer_index);
}

void CAN0_Handler(void)
{
	uint32_t ir_status = CAN0->IR.reg;
	
	if (ir_status && CAN_IE_RF0NE)
	{
		//Ack interrupt
		CAN0->IR.reg = CAN_IE_RF0NE;
		
		can_rx_element_fifo_0 *received = &can0_rx_fifo_0[receive_index];
				
		// VESC Package
		if ((received->R0.bit.ID & 0xFF) == 124)
		{			
			eWheel.vESC.ReceiveVESCPackage(received->R0.bit.ID >> 8, received->data);
		}
		
		//Ack received Message
		CAN0->RXF0A.reg = CAN_RXF0A_F0AI(receive_index);
		
		receive_index++;
		if (receive_index == CONF_CAN0_RX_FIFO_0_NUM)
			receive_index = 0;
	} 
}