/*
 * System.h
 *
 * Created: 09.06.2019 13:47:05
 *  Author: Dominik Hellhake
 */
#ifndef SYSTEM_H_
#define SYSTEM_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>

uint8_t IsFrontNode;

#endif /* SYSTEM_H_ */