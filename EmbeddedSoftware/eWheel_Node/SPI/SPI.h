/*
 * SPI.h
 *
 * Created: 09.06.2019 14:30:04
 *  Author: Dominik Hellhake
 */
#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>

void init_spi(void);

uint8_t spi_putc (uint8_t data);

#endif /* SPI_H_ */