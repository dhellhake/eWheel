/* 
* CANCont.h
*
* Created: 12.04.2019 22:25:31
* Author: Dominik Hellhake
*/
#ifndef __CANCONT_H__
#define __CANCONT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "samc21.h"

#define COMPILER_ALIGNED(a) __attribute__((__aligned__(a)))

/*
 * The setting of the nominal bit rate is based on the GCLK_CAN is 48M which you can
 * change in the conf_clock.h. Below is the default configuration. The
 * time quanta is 48MHz / (5+1) =  8MHz. And each bit is (1 + NTSEG1 + 1 + NTSEG2 + 1) = 16 time
 * quanta which means the bit rate is 8MHz/16=500KHz.
 */
/* Nominal bit Baud Rate Prescaler */
#define CONF_CAN_NBTP_NBRP_VALUE    5
/* Nominal bit (Re)Synchronization Jump Width */
#define CONF_CAN_NBTP_NSJW_VALUE    3
/* Nominal bit Time segment before sample point */
#define CONF_CAN_NBTP_NTSEG1_VALUE  10
/* Nominal bit Time segment after sample point */
#define CONF_CAN_NBTP_NTSEG2_VALUE  3
/*
 * The setting of the data bit rate is based on the GCLK_CAN is 48M which you can
 * change in the conf_clock.h. Below is the default configuration. The
 * time quanta is 48MHz / (5+1) =  8MHz. And each bit is (1 + DTSEG1 + 1 + DTSEG2 + 1) = 16 time
 * quanta which means the bit rate is 8MHz/16=500KHz.
 */
/* Data bit Baud Rate Prescaler */
#define CONF_CAN_DBTP_DBRP_VALUE    5
/* Data bit (Re)Synchronization Jump Width */
#define CONF_CAN_DBTP_DSJW_VALUE    3
/* Data bit Time segment before sample point */
#define CONF_CAN_DBTP_DTSEG1_VALUE  10
/* Data bit Time segment after sample point */
#define CONF_CAN_DBTP_DTSEG2_VALUE  3


/* The value should be 8/12/16/20/24/32/48/64. */
#define CONF_CAN_ELEMENT_DATA_SIZE         12
#define CONF_CAN0_RX_FIFO_0_NUM         16            /* Range: 1..64 */ 

/* -------- CAN_RX_ELEMENT_R0 : (CAN RX element: 0x00) (R/W 32) Rx Element R0 Configuration -------- */
typedef union {
	struct {
		uint32_t ID:29;            /*!< bit:  0..28  Identifier */
		uint32_t RTR:1;            /*!< bit:  29     Remote Transmission Request */
		uint32_t XTD:1;            /*!< bit:  30     Extended Identifier */
		uint32_t ESI:1;            /*!< bit:  31     Error State Indicator */
	} bit;                       /*!< Structure used for bit access */
	uint32_t reg;                /*!< Type used for register access */
} CAN_RX_ELEMENT_R0_Type;
/* -------- CAN_RX_ELEMENT_R1 : (CAN RX element: 0x01) (R/W 32) Rx Element R1 Configuration -------- */
typedef union {
	struct {
		uint32_t RXTS:16;          /*!< bit: 0..15   Rx Timestamp */
		uint32_t DLC:4;            /*!< bit: 16..19  Data Length Code */
		uint32_t BRS:1;            /*!< bit: 20      Bit Rate Switch */
		uint32_t FDF:1;            /*!< bit: 21      FD Format */
		uint32_t :2;               /*!< bit: 22..23  Reserved */
		uint32_t FIDX:7;           /*!< bit: 24..30  Filter Index */
		uint32_t ANMF:1;           /*!< bit: 31      Accepted Non-matching Frame */
	} bit;                       /*!< Structure used for bit  access */
	uint32_t reg;                /*!< Type used for register access */
} CAN_RX_ELEMENT_R1_Type;


/**
 * \brief CAN receive element structure for FIFO 0.
 */
struct can_rx_element_fifo_0 {
	__IO CAN_RX_ELEMENT_R0_Type R0;
	__IO CAN_RX_ELEMENT_R1_Type R1;
	uint8_t data[CONF_CAN_ELEMENT_DATA_SIZE];
};

static volatile uint32_t receive_index = 0;



void InitCAN0();

#ifdef __cplusplus
}
#endif


#endif //__CANCONT_H__
