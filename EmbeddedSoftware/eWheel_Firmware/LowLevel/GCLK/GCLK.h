/*
 * GCLK.h
 *
 * Created: 15.03.2019 15:22:53
 *  Author: Dominik Hellhake
 */

#ifndef GCLK_H_
#define GCLK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "samc21.h"

void InitGCLK();
uint32_t GetGCLK_Hz(uint32_t gclk_id);

#ifdef __cplusplus
}
#endif

#endif /* GCLK_H_ */