/* 
* GP2Y.h
*
* Created: 15.07.2018 12:49:11
* Author: dominik hellhake
*/

#include "..\Executable.h"
#include "..\SSD1306\SSD1306.h"

#ifndef __GP2Y_H__
#define __GP2Y_H__

#define RAW_BUFFER_SIZE		5
#define CHARACTERISTIC_SIZE 32

class GP2Y : public Executable
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	private:
		bool UpdateReceived = false;
	public:
		SSD1306 *OLED = NULL;
	public:
		virtual bool CanExecute() { return this->UpdateReceived; };
		virtual RUN_RESULT Run();
		virtual void Propagate();

	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:
		uint8_t Distance;
		static uint16_t Characteristics[CHARACTERISTIC_SIZE];
	public:
		GP2Y();
		inline void SetDistanceRaw(float distRaw)
		{
			this->Distance_RAW[this->RawIndex] = distRaw;
			
			this->RawIndex++;
			if (this->RawIndex > RAW_BUFFER_SIZE - 1)
				this->RawIndex = 0;
			
			this->UpdateReceived = true;
		}
	private:
		uint8_t RawIndex = 0;
		float Distance_RAW[RAW_BUFFER_SIZE];

}; //GP2Y

#endif //__GP2Y_H__
