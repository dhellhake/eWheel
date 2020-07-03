/* 
* ADS1118.h
*
* Created: 13.06.2020 18:30:36
* Author: dominik hellhake
*/
#ifndef __ADS1118_H__
#define __ADS1118_H__

#include "sam.h"
#include "..\..\Task.h"

enum class ADS_QU_Voltage : uint8_t
{
	INVALID = 0,
	VALID = 1	
};

enum class ADS_IR_Pos : uint8_t
{
	FRONT = 0,
	BACK = 1	
};

class ADS1118 : public Task
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual RUN_RESULT Run(uint32_t timeStamp);
	virtual RUN_RESULT Setup(uint32_t timeStamp);

	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:
		ADS1118();
	
		void Update();
	
		float Avl_Vfront;
		ADS_QU_Voltage Qu_Vfront;
		
		float Avl_Vback;
		ADS_QU_Voltage Qu_Vback;
	
	private:
		ADS_IR_Pos Current_Sampling;
	
		uint16_t Config_Front;
		uint16_t Config_Back;
	
		uint16_t ReadRaw(uint16_t config);
		
		ADS_QU_Voltage SetVoltage(uint16_t rawValue, float *ref);

}; //ADS1118

extern ADS1118 ADS;

#endif //__ADS1118_H__
