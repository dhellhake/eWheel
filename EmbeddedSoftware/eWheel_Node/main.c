/*
 * eWheel_Node.c
 *
 * Created: 05.06.2019 17:51:49
 * Author : Dominik hellhake
 */
#include "System.h"
 
#include <util/delay.h>
#include <avr/interrupt.h>
#include "ADC/ADC.h"
#include "USART/USART.h"
#include "MCP2515/MCP2515.h"
#include "GP2Y/GP2Y.h"

volatile float adc_volt_0;
volatile float adc_volt_1;

int main(void)
{
	init_uart(115200);
	init_adc();
	mcp2515_init();
		
	// Input pin to identify node-type
	DDRC &= ~(1 << 5);
	PORTC |= (1 << PORTC5);
	_delay_ms(1);
	if(PINC & (1 << 5))
		IsFrontNode = 0x01;
	else
		IsFrontNode = 0x00;
	
	sei();
	start_adc();
		
	
	CANMessage message;
	message.rtr = 0;
	message.length = 8;
	
	if (IsFrontNode == 0)
		message.id = 0x0123;
	else
		message.id = 0x0122;
	
	uart_putc(IsFrontNode);
	
    while (1) 
    {		
		float* fPtr = (float*)message.data;
		*fPtr = adc_volt_0;
		
		fPtr = (float*)(message.data + 4);
		*fPtr = adc_volt_1;
		
		uint8_t sreg = SREG;
		cli();
		mcp2515_send_message(&message);

		SREG = sreg;
		sei();
		
		_delay_ms(10);
    }
}



ISR (INT0_vect)
{
	uint8_t status = mcp2515_read_rx_status();
	
	if ((status & 0x40) > 0x00)
	{
		uint8_t data[13] = { 0 };
		
		mcp2515_read_register_cont(0b01100001, data, 13);
		
		//Clear interrupt
		mcp2515_bit_modify(CANINTF, (1 << RX0IF), 0);
	}
}


// Interrupt subroutine for ADC conversion complete
ISR(ADC_vect)
{	
	if ((ADMUX & 0x0F) == 0x00)
	{
		adc_volt_0 = adc_volt_0 - (0.001f * (adc_volt_0 - (((float)ADC) * 0.004883f)));
		
		ADMUX = (ADMUX & 0xF0) | (0x1 & 0x0F);
		ADCSRA |= (1<<ADSC);
	} else {
		adc_volt_1 = adc_volt_1 - (0.001f * (adc_volt_1 - (((float)ADC) * 0.004883f)));
		
		ADMUX = (ADMUX & 0xF0) | (0x0 & 0x0F);
		ADCSRA |= (1<<ADSC);
	}
}