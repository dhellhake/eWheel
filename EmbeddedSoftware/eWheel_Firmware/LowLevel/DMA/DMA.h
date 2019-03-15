/*
 * DMA.h
 *
 * Created: 15.03.2019 14:16:32
 *  Author: Dominik Hellhake
 */ 
#ifndef DMA_H_
#define DMA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "samc21.h"

#define COMPILER_ALIGNED(a) __attribute__((__aligned__(a)))

// DMAC transmit complete flag
extern volatile uint8_t DMAC_TX_Complete[];

void InitDMAC();
void SetDMAChannel0();

void StartDMATransfer(DmacDescriptor *descriptor, uint8_t dmaSlot);


#ifdef __cplusplus
}
#endif

#endif /* DMA_H_ */