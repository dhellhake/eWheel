/*
 * Utilities.h
 *
 * Created: 15.03.2019 15:34:48
 *  Author: Dominik Hellhake
 */ 
#ifndef UTILITIES_H_
#define UTILITIES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "samc21.h"

enum TraceType
{
	BoardTrace = 1,
	VESCTrace = 2,
	DriveTrace = 3
};

/*
* internal Calculate 64 bit division, ref can be found in
* http://en.wikipedia.org/wiki/Division_algorithm#Long_division
*/
inline uint64_t long_division(uint64_t n, uint64_t d)
{
	int32_t i;
	uint64_t q = 0, r = 0, bit_shift;
		
	for (i = 63; i >= 0; i--)
	{
		bit_shift = (uint64_t)1 << i;
		r = r << 1;
		if (n & bit_shift)
		{
			r |= 0x01;
		}
			
		if (r >= d)
		{
			r = r - d;
			q |= bit_shift;
		}
	}
	return q;
}

#ifdef __cplusplus
}
#endif

#endif /* UTILITIES_H_ */