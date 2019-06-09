/*
 * ADC.c
 *
 * Created: 09.06.2019 13:40:29
 *  Author: Dominik Hellhake
 */
#include "ADC.h"
#include <avr/io.h>

void init_adc()
{
	ADMUX =		(1 << REFS0);
	ADCSRA =	(1 << ADEN) |
	(1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) |
	(1 << ADIE);
}

void start_adc()
{
	ADCSRA |= (1<<ADSC);
}
