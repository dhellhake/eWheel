/* 
* DebugPort.cpp
*
* Created: 01.05.2018 17:53:43
* Author: dominik hellhake
*/


#include "DebugPort.h"

// default constructor
DebugPort::DebugPort()
	: USART()
{
} //DebugPort

void DebugPort::Init()
{
	USART::InitSERCOM();
	
}

// default destructor
DebugPort::~DebugPort()
{
} //~DebugPort
