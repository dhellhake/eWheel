/*
 * BLDCPattern.h
 *
 * Created: 17.03.2019 13:30:38
 *  Author: Dominik Hellhake
 */ 
#ifndef BLDCPATTERN_H_
#define BLDCPATTERN_H_

#include "samc21.h"

#define BLDC_PATTERN_STEP1 ((	(1 << U_HIGH_PATT_Pos)						| \
								(1 << U_LOW_PATT_Pos)						| \
								(0 << V_HIGH_PATT_Pos)						| \
								(0 << V_LOW_PATT_Pos)						| \
								(1 << W_HIGH_PATT_Pos)						| \
								(1 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) | \
							((	(0 << U_HIGH_PATT_Pos)						| \
								(1 << U_LOW_PATT_Pos)						| \
								(0 << V_HIGH_PATT_Pos)						| \
								(0 << V_LOW_PATT_Pos)						| \
								(0 << W_HIGH_PATT_Pos)						| \
								(0 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos)

#define BLDC_PATTERN_STEP2 ((	(1 << U_HIGH_PATT_Pos)						| \
								(1 << U_LOW_PATT_Pos)						| \
								(0 << V_HIGH_PATT_Pos)						| \
								(0 << V_LOW_PATT_Pos)						| \
								(1 << W_HIGH_PATT_Pos)						| \
								(1 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) | \
							((	(0 << U_HIGH_PATT_Pos)						| \
								(0 << U_LOW_PATT_Pos)						| \
								(0 << V_HIGH_PATT_Pos)						| \
								(0 << V_LOW_PATT_Pos)						| \
								(0 << W_HIGH_PATT_Pos)						| \
								(1 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos)

#define BLDC_PATTERN_STEP3 ((	(0 << U_HIGH_PATT_Pos)						| \
								(0 << U_LOW_PATT_Pos)						| \
								(1 << V_HIGH_PATT_Pos)						| \
								(1 << V_LOW_PATT_Pos)						| \
								(1 << W_HIGH_PATT_Pos)						| \
								(1 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) | \
							((	(0 << U_HIGH_PATT_Pos)						| \
								(0 << U_LOW_PATT_Pos)						| \
								(0 << V_HIGH_PATT_Pos)						| \
								(0 << V_LOW_PATT_Pos)						| \
								(0 << W_HIGH_PATT_Pos)						| \
								(1 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos)
								
#define BLDC_PATTERN_STEP4 ((	(0 << U_HIGH_PATT_Pos)						| \
								(0 << U_LOW_PATT_Pos)						| \
								(1 << V_HIGH_PATT_Pos)						| \
								(1 << V_LOW_PATT_Pos)						| \
								(1 << W_HIGH_PATT_Pos)						| \
								(1 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) | \
							((	(0 << U_HIGH_PATT_Pos)						| \
								(0 << U_LOW_PATT_Pos)						| \
								(0 << V_HIGH_PATT_Pos)						| \
								(1 << V_LOW_PATT_Pos)						| \
								(0 << W_HIGH_PATT_Pos)						| \
								(0 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos)
								
#define BLDC_PATTERN_STEP5 ((	(1 << U_HIGH_PATT_Pos)						| \
								(1 << U_LOW_PATT_Pos)						| \
								(1 << V_HIGH_PATT_Pos)						| \
								(1 << V_LOW_PATT_Pos)						| \
								(0 << W_HIGH_PATT_Pos)						| \
								(0 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) | \
							((	(0 << U_HIGH_PATT_Pos)						| \
								(0 << U_LOW_PATT_Pos)						| \
								(0 << V_HIGH_PATT_Pos)						| \
								(1 << V_LOW_PATT_Pos)						| \
								(0 << W_HIGH_PATT_Pos)						| \
								(0 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos)
								
#define BLDC_PATTERN_STEP6 ((	(1 << U_HIGH_PATT_Pos)						| \
								(1 << U_LOW_PATT_Pos)						| \
								(1 << V_HIGH_PATT_Pos)						| \
								(1 << V_LOW_PATT_Pos)						| \
								(0 << W_HIGH_PATT_Pos)						| \
								(0 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) | \
							((	(0 << U_HIGH_PATT_Pos)						| \
								(1 << U_LOW_PATT_Pos)						| \
								(0 << V_HIGH_PATT_Pos)						| \
								(0 << V_LOW_PATT_Pos)						| \
								(0 << W_HIGH_PATT_Pos)						| \
								(0 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos)
								
#define BLDC_PATTERN_IDLE ((	(1 << U_HIGH_PATT_Pos)						| \
								(1 << U_LOW_PATT_Pos)						| \
								(1 << V_HIGH_PATT_Pos)						| \
								(1 << V_LOW_PATT_Pos)						| \
								(1 << W_HIGH_PATT_Pos)						| \
								(1 << W_LOW_PATT_Pos)) << TCC_PATT_PGE_Pos) | \
							((	(0 << U_HIGH_PATT_Pos)						| \
								(0 << U_LOW_PATT_Pos)						| \
								(0 << V_HIGH_PATT_Pos)						| \
								(0 << V_LOW_PATT_Pos)						| \
								(0 << W_HIGH_PATT_Pos)						| \
								(0 << W_LOW_PATT_Pos)) << TCC_PATT_PGV_Pos)
#endif /* BLDCPATTERN_H_ */