/* 
* ADS.h
*
* Created: 09.07.2018 21:35:28
* Author: Dominik Hellhake
*/
#ifndef __ADS_H__
#define __ADS_H__

#include "..\Executable.h"

#include "..\LowLevel/Device/SysTick/SysTick.h"
#include "..\SSD1306\SSD1306.h"
#include "..\GP2Y\GP2Y.h"

#define MotorCurrent_Config_Val		0b01000010
#define Distance_Config_Val			0b01010010
#define Battery_Config_Val			0b01100010

enum class ADS_STATE
{
	MotorCurent,
	Distance,
	Battery
};

class ADS : public Executable
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	public:
		GP2Y *IR = NULL;
		SSD1306 *OLED = NULL;
	private:
		uint64_t LastRun = 0;
	public:
		virtual bool CanExecute() { return (this->LastRun + 33) <= GetElapsedMilis(); };
		virtual RUN_RESULT Run(uint32_t timeStamp);
		virtual void Propagate();

	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:
		float Battery = 0;
		float MotorCurrent_RAW = 0;
		float Distance_RAW = 0;
				
		ADS();
	private:
		ADS_STATE State = ADS_STATE::MotorCurent;
		
		int16_t WriteRegister(uint8_t *data);
}; //ADS

#endif //__ADS_H__
