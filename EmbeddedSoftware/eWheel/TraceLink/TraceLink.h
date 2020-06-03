/* 
* TraceLink.h
*
* Created: 03.06.2020 20:11:20
* Author: dominik helhake
*/
#ifndef __TRACELINK_H__
#define __TRACELINK_H__

#include "sam.h"
#include "..\Task.h"

#define TRACE_RCV_BUF_SIZE	128

enum class TRACE_CMD : uint8_t
{
	ENTER_TRACEMODE = 0x01,
	ERASE_MEMORY	= 0x02,
	REQUEST_MODE	= 0x03,
	RESPONSE_MODE	= 0x04,
	SW_RESET		= 0xFF
};

class TraceLink : public Task
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual RUN_RESULT Run(uint32_t timeStamp);

	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:	
		uint8_t ReceiveBufferIndex = 0;
		uint8_t ReceiveBuffer[TRACE_RCV_BUF_SIZE];
}; //TraceLink

extern TraceLink Trace;

#endif //__TRACELINK_H__
