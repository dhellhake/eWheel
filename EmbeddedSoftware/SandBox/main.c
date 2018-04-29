/*
 * SandBox.c
 *
 * Created: 28.04.2018 10:08:12
 * Author : Dominik Hellhake
 */ 


#include "sam.h"

uint8_t spiSend(uint8_t data)
{	
	while(SERCOM0->SPI.INTFLAG.bit.DRE == 0);
	SERCOM0->SPI.DATA.reg = data;
	while(SERCOM0->SPI.INTFLAG.bit.RXC == 0);
	return SERCOM0->SPI.DATA.reg;
}

/*
* internal Calculate 64 bit division, ref can be found in
* http://en.wikipedia.org/wiki/Division_algorithm#Long_division
*/
static uint64_t long_division(uint64_t n, uint64_t d)
{
	int32_t i;
	uint64_t q = 0, r = 0, bit_shift;
	
	for (i = 63; i >= 0; i--)
	{
		bit_shift = (uint64_t)1 << i;
		r = r << 1;
		if (n & bit_shift)
		{
			r |= 0x01;
		}
		
		if (r >= d)
		{
			r = r - d;
			q |= bit_shift;
		}
	}
	return q;
}

uint16_t calculate_baud_value(const uint32_t baudrate, const uint32_t peripheral_clock, uint8_t sample_num)
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

static inline void pin_set_peripheral_function(uint32_t pinmux)
{
	uint8_t port = (uint8_t)((pinmux >> 16)/32);
	
	PORT->Group[port].PINCFG[((pinmux >> 16) - (port*32))].bit.PMUXEN = 1;
	
	PORT->Group[port].PMUX[((pinmux >> 16) - (port*32))/2].reg &= ~(0xF << (4 * ((pinmux >> 16) & 0x01u)));
	PORT->Group[port].PMUX[((pinmux >> 16) - (port*32))/2].reg |= (uint8_t)((pinmux & 0x0000FFFF) << (4 * ((pinmux >> 16) & 0x01u)));
}

void SERCOM5_Handler()
{
	
	
}

void InitUSART()
{
	
	//USART:
	//CP2102.RX => PB02 / SERCOM5.PAD0 (MUX_D)
	//CP2102.TX => PB03 / SERCOM5.PAD1 (MUX_D)
	
	uint32_t baud_val = calculate_baud_value(115200, get_gclk_hz(SERCOM5_GCLK_ID_CORE), 16);
	//Enable Clock for SERCOM5
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.SERCOM5_ = 1;

	//Generate Clock al ref for BAUD
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[SERCOM5_GCLK_ID_CORE].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[SERCOM5_GCLK_ID_CORE].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[SERCOM5_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN(0); //Use Generic Clock Generator 0

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[SERCOM5_GCLK_ID_CORE].reg |= GCLK_PCHCTRL_CHEN;

	//Configure SERCOM5 for USART
	const SERCOM_USART_CTRLA_Type ctrla =
	{
		.bit.DORD = 1,			//LSB first
		.bit.CPOL = 0,			//Rising XCK edge
		.bit.CMODE = 0,			//Async communication
		.bit.FORM = 0x0,		//USAT frame with parity
		.bit.SAMPA = 0x0,		//7-8-9 / 3-4-5
		.bit.RXPO = 0x1,		//Receive on PAD1 (PB03)
		.bit.TXPO = 0x0,		//Transmit on PAD0 (PB02)
		.bit.SAMPR = 0x0,		//16x over-sampling on arithmetic baud
		.bit.IBON = 0,			//Interupt on data-stream
		.bit.RUNSTDBY = 0,		//Generic Clock disable on finish
		.bit.MODE = 0x1			//Use Internal Clock
	};
	SERCOM5->USART.CTRLA.reg = ctrla.reg;
	
	
	const SERCOM_USART_CTRLB_Type ctrlb =
	{
		.bit.LINCMD = 0x0,		//0x0 = Normal USART
		.bit.RXEN = 1,			//1 = Receiver enabled
		.bit.TXEN = 1,			//1 = Transmit enable
		.bit.PMODE = 0,			//0 = Even parity
		.bit.ENC = 0,			//0 = no encoding
		.bit.SFDE = 0,			//0 = Start-of-frame detection disabled
		.bit.COLDEN = 0,		//0 = collision detect disabled
		.bit.SBMODE = 0,		//0 = one stop bit
		.bit.CHSIZE = 0x0,		//0x0 = 8 bits
	};
	SERCOM5->USART.CTRLB.reg = ctrlb.reg;
	
	while(SERCOM3->USART.SYNCBUSY.bit.CTRLB);
	
	SERCOM5->USART.BAUD.reg = baud_val;
	
	pin_set_peripheral_function(PINMUX_PB02D_SERCOM5_PAD0);
	pin_set_peripheral_function(PINMUX_PB03D_SERCOM5_PAD1);


	NVIC->ISER[0] = (uint32_t)(1 << ((uint32_t)SERCOM5_IRQn & 0x0000001f));

	SERCOM5->USART.INTENSET.reg = SERCOM_USART_INTFLAG_RXC;
	SERCOM5->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
	/* synchronization busy */
	while(SERCOM5->USART.SYNCBUSY.reg & SERCOM_USART_SYNCBUSY_ENABLE);
}


void InitSpi()
{
	// max 10Mhz
	// MSB first
	// CPOL=0 (~SERCOM_SPI_CTRLA_CPOL)
	// CPHA=0 (~SERCOM_SPI_CTRLA_CPHA)
	// Module SERCOM0
	// PINMUX_PA08C_SERCOM0_PAD0
	// PINMUX_PA09C_SERCOM0_PAD1
	// PINMUX_PA10C_SERCOM0_PAD2
	// PINMUX_PA11C_SERCOM0_PAD3

	//Enable Clock for SERCOM0
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.SERCOM0_ = 1;
	
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN(0);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg |= GCLK_PCHCTRL_CHEN;
	
	
	//Configure CS-Pin as Output
	PORT->Group[0].DIRSET.reg = PORT_PA09;
	PORT->Group[0].OUTSET.reg = PORT_PA09;
	
	pin_set_peripheral_function(PINMUX_PA08C_SERCOM0_PAD0);
	pin_set_peripheral_function(PINMUX_PA10C_SERCOM0_PAD2);
	pin_set_peripheral_function(PINMUX_PA11C_SERCOM0_PAD3);
	

	//Configure SERCOM0	
	const SERCOM_SPI_CTRLA_Type ctrla = {
		.bit.DORD = 0,		//MSB first
		.bit.CPHA = 0,		//CPHA mode 0
		.bit.CPOL = 0,		//CPOL mode 0
		.bit.FORM = 0,		//SPI frame
		.bit.DIPO = 0,		//MISO on PAD0
		.bit.DOPO = 1,		//MOSI on PAD2; SCK on PAD3; SS on PAD 1
		.bit.MODE = 3		//Master mode
	};
	SERCOM0->SPI.CTRLA.reg = ctrla.reg;
	
	const SERCOM_SPI_CTRLB_Type ctrlb = {
		.bit.RXEN = 1,   // Receive is enabled
		.bit.MSSEN = 0,  // Software controlled Slave Select
		.bit.CHSIZE = 0  // 8-Bit character transfer
	};
	SERCOM0->SPI.CTRLB.reg = ctrlb.reg;
	
	SERCOM0->SPI.BAUD.reg = (get_gclk_hz(SERCOM0_GCLK_ID_CORE) / (2 * 10000000)) - 1;
	
	NVIC->ISER[0] = (uint32_t)(1 << ((uint32_t)SERCOM0_IRQn & 0x0000001f));
	
	SERCOM0->SPI.CTRLA.bit.ENABLE = 1;
	while(SERCOM0->SPI.SYNCBUSY.bit.ENABLE);	
		
}

void SERCOM0_Handler()
{
	
}


int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
			
	InitSpi();	
	PORT->Group[0].OUTCLR.reg = PORT_PA09;
	uint8_t result = spiSend(0x8F);
	result = spiSend(0x00);
	
	PORT->Group[0].OUTSET.reg = PORT_PA09;
	
    /* Replace with your application code */
    while (1)
    {
		
    }
}


