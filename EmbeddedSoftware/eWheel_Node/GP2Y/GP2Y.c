/*
 * GP2Y.c
 *
 * Created: 10.06.2019 19:10:19
 *  Author: Dominik Hellhake
 */ 
#include "GP2Y.h"

/************************************************************************/
/* output distance characteristics										*/
/* Contains voltage * 100 values										*/
/* Index at which a voltage is stored represents the					*/
/* distance in cm + 3cm offset.											*/
/************************************************************************/
uint16_t keys[28] =
{
	290,
	270,
	231,
	195,
	171,
	147,
	126,
	121,
	113,
	100,
	93,
	85,
	80,
	70,
	67,
	63,
	57,
	53,
	49,
	47,
	45,
	43,
	40,
	38,
	36,
	34,
	32,
	31
};


float GetDistance(float voltage)
{
	uint16_t key = (uint16_t)(voltage * 100.0f);
	
	uint8_t least_lower_Pos = 0;
	
	for (uint8_t index = 0; index < 28; index++)
	{
		if (keys[index] <= key)
		{
			least_lower_Pos = index;
			break;
		}
	}
	
	float lt = keys[least_lower_Pos];
	
	if (least_lower_Pos == 0)
		return ((float)least_lower_Pos) + 3.0f;
	
	float gt = keys[least_lower_Pos - 1];
		
	float interpol = (key - lt) / ((gt - lt) / 100.0f);
	
	return ((float)least_lower_Pos) + 4.0f - (interpol / 100.0f);
}
