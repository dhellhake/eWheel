/* 
* SSD1306.h
*
* Created: 30.05.2018 19:34:55
* Author: dominik hellhake
*/


#ifndef __SSD1306_H__
#define __SSD1306_H__

#include <math.h>
#include "..\LowLevel\I2C\I2C.h"
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
		static uint8_t Blank[32];
		static uint8_t ASCII[320];
	
		uint8_t CMDBuffer[27] = {};
			
		COMPILER_ALIGNED(16)
		DmacDescriptor DMAC_Descriptors[9];
	public:
		SSD1306();	
		void Clear();	
		void WriteInt(uint32_t number, uint8_t row);
	private:
		void SetAddress(uint8_t row);
		void InitDMADescriptors();

}; //SSD1306

#endif //__SSD1306_H__
