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

int main(void)
{
	
	DDRD &= ~(1 << PD0);
	DDRD |= (1 << PD1);

	
    /* Replace with your application code */
    while (1) 
    {
	    PORTD |= (1 << PD1);  //PB1 High
				
		_delay_ms(50);
		
	    PORTD &= ~(1 << PD1); //PB1 Low
		
	    
	    _delay_ms(50);
    }
}

