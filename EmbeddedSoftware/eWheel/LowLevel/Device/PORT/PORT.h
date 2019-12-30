/*
 * PORT.h
 *
 * Created: 15.03.2019 16:37:16
 *  Author: Dominik Hellhake
 */
#ifndef PORT_H_
#define PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "samc21.h"

void InitPORT();

void SetPinPeripheralFunction(uint32_t pinmux);

#ifdef __cplusplus
}
#endif

#endif /* PORT_H_ */