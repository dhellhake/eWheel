/*
 * SandBox.cpp
 *
 * Created: 30.04.2018 15:10:53
 * Author : dominik hellhake
 */ 
#include "sam.h"
#include "LowLevel/System/System.h"
#include "DebugPort/DebugPort.h"
#include "LSM6D/LSM6D.h"
#include "MotorControl/MotorControl.h"

#include "MotorControl/MotorSensor/MotorSensor.h"
#include "SystemMonitor/SSD1306/SSD1306.h"

#include <string.h>

void EIC_Handler()
{
	if ((EIC->INTFLAG.reg & ((1 << 7) | (1 << 12) | (1 << 13))) != 0x00)
	{
		/* Hall Sensor caused interrupt */
		MotorControl::ElectronicPhase_Changed();
				
		EIC->INTFLAG.reg = (1 << 7) | (1 << 12) | (1 << 13);
	} 
	if ((EIC->INTFLAG.reg & (1 << 2)) != 0x00)
	{
		/* LSM6D->INT1 caused interrupt */
		//LSM6D::Update();
		
		EIC->INTFLAG.reg = (1 << 2);
	}
}

#   define COMPILER_ALIGNED(a)        __attribute__((__aligned__(a)))

COMPILER_ALIGNED(16)
DmacDescriptor descriptor_section[1];

COMPILER_ALIGNED(16)
static DmacDescriptor _write_back_section[1];

void DMAC_Handler(void)
{
}

int main(void)
{
    /* Initialize the SAM system */
    System::Init();
	/* Initialize the SSD1306 OLED driver */
	//SSD1306::Init();
	/* Initialize the debug serial port */
	//DebugPort::Init();
	/* Initialize the lsm6d sensor */
	//LSM6D::Init();
	/* Initialize BLDC Motor Control */
	//MotorControl::Init();
	
	SSD1306::Init();
		
	
	//Turn off I2C Interrupts
	SERCOM4->I2CM.INTENCLR.reg = (SERCOM_I2CM_INTENSET_MB | SERCOM_I2CM_INTENSET_SB);
		
	
	MCLK->AHBMASK.reg |= MCLK_AHBMASK_DMAC;
	
	/* Perform a software reset before enable DMA controller */
	DMAC->CTRL.reg &= ~DMAC_CTRL_DMAENABLE;
	DMAC->CTRL.reg = DMAC_CTRL_SWRST;
	
	/* Setup descriptor base address and write back section base address */
	DMAC->BASEADDR.reg = (uint32_t)descriptor_section;
	DMAC->WRBADDR.reg = (uint32_t)_write_back_section;
	
	/* Enable all priority level at the same time */
	DMAC->CTRL.reg = DMAC_CTRL_DMAENABLE | DMAC_CTRL_LVLEN(0xf);
	
	
	DMAC->CHID.reg = DMAC_CHID_ID(0);
	DMAC->CHCTRLA.reg &= ~DMAC_CHCTRLA_ENABLE;
	DMAC->CHCTRLA.reg = DMAC_CHCTRLA_SWRST;
	DMAC->CHCTRLA.reg |= DMAC_CHCTRLA_RUNSTDBY;
	
	DMAC->SWTRIGCTRL.reg &= (uint32_t)(~(1 << 0));
	
	
	DMAC->CHCTRLB.reg =	DMAC_CHCTRLB_LVL(0) | \
						DMAC_CHCTRLB_TRIGSRC(SERCOM4_DMAC_ID_TX) | \
						DMAC_CHCTRLB_TRIGACT(DMAC_CHCTRLB_TRIGACT_BEAT_Val);
	
	
	//Descriptor
	//dst_increment = false
	//beat-size = byte
	//Data-length = amount of bytes to send
	//Source-Address = (uint32_t)buffer + DATA_LENGTH (end of buffer)
	//Destination-Addr = (uint32_t)(&SERCOM4->I2CM.DATA.reg)
	
	uint8_t buffer[2] = {0x00, 0xAE};
	
	
	COMPILER_ALIGNED(16)
	DmacDescriptor example_descriptor;
	
	
	/* Set block transfer control */
	example_descriptor.BTCTRL.bit.VALID = true;
	example_descriptor.BTCTRL.bit.EVOSEL = 0;
	example_descriptor.BTCTRL.bit.BLOCKACT = 0;
	example_descriptor.BTCTRL.bit.BEATSIZE = 0;
	example_descriptor.BTCTRL.bit.SRCINC = true;
	example_descriptor.BTCTRL.bit.DSTINC = false;
	example_descriptor.BTCTRL.bit.STEPSEL = 0;
	example_descriptor.BTCTRL.bit.STEPSIZE = 0;

	/* Set transfer size, source address and destination address */
	example_descriptor.BTCNT.reg = 2;								//Data-length = amount of bytes to send
	example_descriptor.SRCADDR.reg = (uint32_t)buffer + 2;
	example_descriptor.DSTADDR.reg = (uint32_t)(&SERCOM4->I2CM.DATA.reg);

	/* Set next transfer descriptor address */
	example_descriptor.DESCADDR.reg = 0;
	
	
	
	//Enable DMAC interrupt
	NVIC->ISER[0] = (uint32_t)(1 << ((uint32_t)DMAC_IRQn & 0x0000001f));	
	DMAC->CHINTENSET.reg = DMAC_CHINTENSET_TERR | DMAC_CHINTENSET_TCMPL | DMAC_CHINTENSET_SUSP;
	
	/* Set channel x descriptor 0 to the descriptor base address */
	memcpy(&descriptor_section[0], &example_descriptor, sizeof(DmacDescriptor));

	/* Enable the transfer channel */
	DMAC->CHCTRLA.reg |= DMAC_CHCTRLA_ENABLE;
		
	SERCOM4->I2CM.ADDR.reg =
					SERCOM_I2CM_ADDR_ADDR(SLAVE_ADDR<<1) |
					SERCOM_I2CM_ADDR_LENEN |
					SERCOM_I2CM_ADDR_LEN(2) |
					0;
	
	while (1)
	{	
	}
}

