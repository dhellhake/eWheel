/* 
* DebugPort.h
*
* Created: 01.05.2018 17:53:43
* Author: domin
*/


#ifndef __DEBUGPORT_H__
#define __DEBUGPORT_H__

#include "sam.h"
#include "..\LowLevel\USART\USART.h"

class DebugPort : public USART
{
//variables
public:
protected:
private:

//functions
public:	
	static void Init();
protected:
private:
	DebugPort();
	~DebugPort();
	DebugPort( const DebugPort &c );
	DebugPort& operator=( const DebugPort &c );

}; //DebugPort

#endif //__DEBUGPORT_H__
