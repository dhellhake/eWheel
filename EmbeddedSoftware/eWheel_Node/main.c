/*
 * eWheel_Node.c
 *
 * Created: 05.06.2019 17:51:49
 * Author : Dominik hellhake
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile float adc_volt_0;
volatile float adc_volt_1;


void init_uart(uint32_t baudrate){

	uint16_t UBRR_val = (F_CPU/16)/(baudrate-1);

	UBRR0H = UBRR_val >> 8;
	UBRR0L = UBRR_val;

	UCSR0B |= (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0); // UART TX (Transmit - senden) einschalten
	UCSR0C |= (1<<USBS0) | (3<<UCSZ00); //Modus Asynchron 8N1 (8 Datenbits, No Parity, 1 Stopbit)
}

void uart_putc(uint8_t data){

	while(!(UCSR0A & (1<<UDRE0))); // wait until sending is possible
	UDR0 = data;
}

void init_adc()
{
	ADMUX =		(1 << REFS0);
	ADCSRA =	(1 << ADEN) |
				(1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) |
				(1 << ADIE);
}

void start_adc()
{
	sei();
	ADCSRA |= (1<<ADSC);
}


int main(void)
{
	init_uart(115200);
	init_adc();
	start_adc();
	
	uint8_t *bytes_0 = &adc_volt_0;
	uint8_t *bytes_1 = &adc_volt_1;
    /* Replace with your application code */
    while (1) 
    {
	    uart_putc(bytes_1[3]);
	    uart_putc(bytes_1[2]);
	    uart_putc(bytes_1[1]);
	    uart_putc(bytes_1[0]);
	    uart_putc(bytes_0[3]);
	    uart_putc(bytes_0[2]);
	    uart_putc(bytes_0[1]);
		uart_putc(bytes_0[0]);
		uart_putc(0xFF);
		uart_putc(0xFF);
		_delay_ms(5);
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