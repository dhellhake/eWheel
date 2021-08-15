/*
 * SDCardReg.h
 *
 * Created: 08.05.2021 12:12:38
 *  Author: dominik hellhake
 */ 
#ifndef SDCARDREG_H_
#define SDCARDREG_H_

#define CARD_STATUS_APP_CMD           (1lu << 5)
#define CARD_STATUS_SWITCH_ERROR      (1lu << 7)
#define CARD_STATUS_READY_FOR_DATA    (1lu << 8)
#define CARD_STATUS_STATE_IDLE        (0lu << 9)
#define CARD_STATUS_STATE_READY       (1lu << 9)
#define CARD_STATUS_STATE_IDENT       (2lu << 9)
#define CARD_STATUS_STATE_STBY        (3lu << 9)
#define CARD_STATUS_STATE_TRAN        (4lu << 9)
#define CARD_STATUS_STATE_DATA        (5lu << 9)
#define CARD_STATUS_STATE_RCV         (6lu << 9)
#define CARD_STATUS_STATE_PRG         (7lu << 9)
#define CARD_STATUS_STATE_DIS         (8lu << 9)
#define CARD_STATUS_STATE             (0xFlu << 9)
#define CARD_STATUS_ERASE_RESET       (1lu << 13)
#define CARD_STATUS_WP_ERASE_SKIP     (1lu << 15)
#define CARD_STATUS_CIDCSD_OVERWRITE  (1lu << 16)
#define CARD_STATUS_OVERRUN           (1lu << 17)
#define CARD_STATUS_UNERRUN           (1lu << 18)
#define CARD_STATUS_ERROR             (1lu << 19)
#define CARD_STATUS_CC_ERROR          (1lu << 20)
#define CARD_STATUS_CARD_ECC_FAILED   (1lu << 21)
#define CARD_STATUS_ILLEGAL_COMMAND   (1lu << 22)
#define CARD_STATUS_COM_CRC_ERROR     (1lu << 23)
#define CARD_STATUS_UNLOCK_FAILED     (1lu << 24)
#define CARD_STATUS_CARD_IS_LOCKED    (1lu << 25)
#define CARD_STATUS_WP_VIOLATION      (1lu << 26)
#define CARD_STATUS_ERASE_PARAM       (1lu << 27)
#define CARD_STATUS_ERASE_SEQ_ERROR   (1lu << 28)
#define CARD_STATUS_BLOCK_LEN_ERROR   (1lu << 29)
#define CARD_STATUS_ADDRESS_MISALIGN  (1lu << 30)
#define CARD_STATUS_ADDR_OUT_OF_RANGE (1lu << 31)

#define CARD_STATUS_ERR_RD_WR \
		( CARD_STATUS_ADDR_OUT_OF_RANGE \
		| CARD_STATUS_ADDRESS_MISALIGN \
		| CARD_STATUS_BLOCK_LEN_ERROR \
		| CARD_STATUS_WP_VIOLATION \
		| CARD_STATUS_ILLEGAL_COMMAND \
		| CARD_STATUS_CC_ERROR \
		| CARD_STATUS_ERROR)



#define OCR_REG_BSIZE          (32 / 8)    /**< 32 bits, 4 bytes */
#define OCR_VDD_170_195        (1lu << 7)
#define OCR_VDD_20_21          (1lu << 8)
#define OCR_VDD_21_22          (1lu << 9)
#define OCR_VDD_22_23          (1lu << 10)
#define OCR_VDD_23_24          (1lu << 11)
#define OCR_VDD_24_25          (1lu << 12)
#define OCR_VDD_25_26          (1lu << 13)
#define OCR_VDD_26_27          (1lu << 14)
#define OCR_VDD_27_28          (1lu << 15)
#define OCR_VDD_28_29          (1lu << 16)
#define OCR_VDD_29_30          (1lu << 17)
#define OCR_VDD_30_31          (1lu << 18)
#define OCR_VDD_31_32          (1lu << 19)
#define OCR_VDD_32_33          (1lu << 20)
#define OCR_VDD_33_34          (1lu << 21)
#define OCR_VDD_34_35          (1lu << 22)
#define OCR_VDD_35_36          (1lu << 23)
#define OCR_SDIO_S18R          (1lu << 24) /**< Switching to 1.8V Accepted */
#define OCR_SDIO_MP            (1lu << 27) /**< Memory Present */
#define OCR_SDIO_NF            (7lu << 28) /**< Number of I/O Functions */
#define OCR_ACCESS_MODE_MASK   (3lu << 29) /**< (MMC) Access mode mask */
#define OCR_ACCESS_MODE_BYTE   (0lu << 29) /**< (MMC) Byte access mode */
#define OCR_ACCESS_MODE_SECTOR (2lu << 29) /**< (MMC) Sector access mode */
#define OCR_CCS                (1lu << 30) /**< (SD) Card Capacity Status */
#define OCR_POWER_UP_BUSY      (1lu << 31) /**< Card power up status bit */

#define SD_MMC_VOLTAGE_SUPPORT (OCR_VDD_27_28 | OCR_VDD_28_29 | OCR_VDD_29_30 | OCR_VDD_30_31 | OCR_VDD_31_32 | OCR_VDD_32_33)

#define SD_ACMD41_HCS   (1lu << 30) /* (SD) Host Capacity Support */

#endif /* SDCARDREG_H_ */