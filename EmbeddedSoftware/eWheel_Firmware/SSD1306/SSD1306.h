/* 
* SSD1306.h
*
* Created: 30.05.2018 19:34:55
* Author: Dominik Hellhake
*/
#ifndef __SSD1306_H__
#define __SSD1306_H__

#include <math.h>
#include "..\LowLevel\DMA\DMA.h"
#include "..\Executable.h"

#define SLAVE_ADDR 0b0111100

class SSD1306 : public Executable
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	public:
		virtual bool CanExecute() { return this->RowHasChanged[0] || this->RowHasChanged[1]; };
		virtual RUN_RESULT Run();
		
		inline void SetRow(uint32_t number, uint8_t row)
		{
			this->RowValues[row] = number;
			this->RowHasChanged[row] = true;
		}	
	private:
		bool RowHasChanged[2] = { false, false};
		uint32_t RowValues[2] = { 0, 0};
	
	
	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	private:
		COMPILER_ALIGNED(16)
		DmacDescriptor DMAC_Descriptors[9];
	
		static uint8_t Blank[32];
		static uint8_t ASCII[320];
	
		uint8_t CMDBuffer[27] = {};
			
		void SetAddress(uint8_t row);
		void InitDMADescriptors();
	public:
		SSD1306();
		void DeviceInitialization();
		void Clear();
		void WriteInt(uint32_t number, uint8_t row);
}; //SSD1306

#endif //__SSD1306_H__
