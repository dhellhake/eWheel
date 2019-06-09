/*
 * eWheel_Node.c
 *
 * Created: 05.06.2019 17:51:49
 * Author : Dominik hellhake
 */
#include "System.h"
#include "ADC/ADC.h"
#include "USART/USART.h"
#include "MCP2515/MCP2515.h"


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile float adc_volt_0;
volatile float adc_volt_1;

int main(void)
{
	init_uart(115200);
	init_adc();

	mcp2515_init();
	
	sei();
	start_adc();
	
	
	uint8_t *bytes_0 = &adc_volt_0;
	uint8_t *bytes_1 = &adc_volt_1;
    /* Replace with your application code */
    while (1) 
    {
		uint8_t status = mcp2515_read_register(0x0E);
		
		uart_putc(status);
		
		_delay_ms(1000);
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