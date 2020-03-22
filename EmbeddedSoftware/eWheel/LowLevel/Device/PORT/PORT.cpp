/*
 * PORT.cpp
 *
 * Created: 15.03.2019 16:38:48
 *  Author: Dominik Hellhake
 */
#include "PORT.h"

void InitPORT()
{	
	/* Init PORT interface to LS9M */
	//Configure CS-Pin as Output
	PORT->Group[0].DIRSET.reg = PORT_PA12;
	PORT->Group[0].OUTSET.reg = PORT_PA12;
	//Configure INT1-Pin as Input
	PORT->Group[0].DIRCLR.reg = PORT_PA13;
	PORT->Group[0].OUTCLR.reg = PORT_PA13;
	PORT->Group[0].PINCFG[13].reg = PORT_PINCFG_INEN;
	SetPinPeripheralFunction(PINMUX_PA09D_SERCOM2_PAD1);
	SetPinPeripheralFunction(PINMUX_PA10D_SERCOM2_PAD2);
	SetPinPeripheralFunction(PINMUX_PA11D_SERCOM2_PAD3);
	
	
	/* Init PORT interface to AT45DB */
	//Configure CS-Pin as Output
	PORT->Group[0].DIRSET.reg = PORT_PA08;
	PORT->Group[0].OUTSET.reg = PORT_PA08;
	SetPinPeripheralFunction(PINMUX_PA05D_SERCOM0_PAD1);
	SetPinPeripheralFunction(PINMUX_PA06D_SERCOM0_PAD2);
	SetPinPeripheralFunction(PINMUX_PA07D_SERCOM0_PAD3);
		
	/* Init PORT interface to CC41-A */
	SetPinPeripheralFunction(PINMUX_PA17C_SERCOM1_PAD1);
	SetPinPeripheralFunction(PINMUX_PA16C_SERCOM1_PAD0);
	
	/* Init PORT interface to VESC */
	SetPinPeripheralFunction(PINMUX_PA22C_SERCOM3_PAD0);
	SetPinPeripheralFunction(PINMUX_PA23C_SERCOM3_PAD1);
	
	/* Init PORT interface to CP2102 */
	SetPinPeripheralFunction(PINMUX_PB02D_SERCOM5_PAD0);
	SetPinPeripheralFunction(PINMUX_PB03D_SERCOM5_PAD1);
	
	/* Init PORT interface to Board-LED */	
	PORT->Group[0].DIRSET.reg = PORT_PA28;
	PORT->Group[0].OUTCLR.reg = PORT_PA28;
}


void SetPinPeripheralFunction(uint32_t pinmux)
{
	uint8_t port = (uint8_t)((pinmux >> 16)/32);
	
	PORT->Group[port].PINCFG[((pinmux >> 16) - (port*32))].bit.PMUXEN = 1;
	
	PORT->Group[port].PMUX[((pinmux >> 16) - (port*32))/2].reg &= ~(0xF << (4 * ((pinmux >> 16) & 0x01u)));
	PORT->Group[port].PMUX[((pinmux >> 16) - (port*32))/2].reg |= (uint8_t)((pinmux & 0x0000FFFF) << (4 * ((pinmux >> 16) & 0x01u)));
}