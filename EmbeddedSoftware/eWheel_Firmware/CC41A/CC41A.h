/* 
* CC41A.h
*
* Created: 09.03.2019 18:06:43
* Author: Dominik Hellhake
*/
#ifndef __CC41A_H__
#define __CC41A_H__

#include "samc21.h"
#include "..\Executable.h"

enum class PackageType
{
	Orientation = 1	
};

enum class DEBUG_CMD
{
	SetLED = 65
};

typedef struct {
	PackageType _type;
	uint32_t _timeStamp;
	uint8_t _length;
	uint8_t* _data;
} DataPackage;

#define RECEIVE_BUFFER_SIZE 64

class CC41A : public Executable
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual bool IsReady(uint32_t timeStamp)
	{
		return this->ReceiveBufferIndex >= 2;
	}
	virtual RUN_RESULT Run(uint32_t timeStamp);

	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	private:	
		uint8_t ReceiveBufferIndex = 0;
		uint8_t ReceiveBuffer[RECEIVE_BUFFER_SIZE];	
	public:
		CC41A();
		void ReceiveByte(uint8_t data);
		void SendDataPackage(DataPackage *pkg);
}; //CC41A

#endif //__CC41A_H__
