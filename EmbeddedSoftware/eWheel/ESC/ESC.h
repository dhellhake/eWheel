/* 
* ESC.h
*
* Created: 14.04.2019 15:02:13
* Author: Dominik Hellhake
*/
#ifndef __ESC_H__
#define __ESC_H__

#include "samc21.h"
#include "ESCUtilities.h"
#include "..\Executable.h"

#define VESC_RCV_BUF_SIZE 256

class ESC : public Executable
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual RUN_RESULT Run(uint32_t timeStamp);

	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/		
		
	public:
		VESCFaultCode FaultCode;
		float Avl_TempFET;
		float Avl_RPM;
		float Avl_Current;
		float Avl_CurrentIn;
		float Avl_Duty;
		float Avl_Vin;
		float Avl_Ah;
		float Avl_AhCharged;
		float Avl_Wh;
		float Avl_WhCharged;
		int32_t Avl_Tach;
		
		// Duty Cycle [0.0 to 1.0]
		float Tar_Duty;
		
		ESC();
		
		void Stop();
		
		inline void ResetTarValues()
		{
			this->Tar_Duty = 0.00f;
		}		
		
		inline void SetTar_Duty(float value)
		{
			this->ResetTarValues();
			
			if (value > 1.0f)
				this->Tar_Duty = 1.0f;
			else if (value < -1.0f)
				this->Tar_Duty = -1.0f;
			else
				this->Tar_Duty = value;
		}
		
		void ReceiveByte(uint8_t data);
		
	private:		
		uint8_t ReceiveBufferIndex = 0;
		uint8_t ReceiveBuffer[VESC_RCV_BUF_SIZE];
		uint8_t ReceiveBuffer_2[VESC_RCV_BUF_SIZE];
	
		uint32_t VESCValues_Tstmp;
		
		
		void SetCurrent(float current_val);
		void SetDuty(float duty_val);
		
		void SendVESCPacket(VESCPackageType type, uint8_t* payload, uint16_t length);
		void ReceiveVESCPacket(uint8_t* payload, uint16_t length);
		
		void GetVESCValues();
}; //ESC
extern ESC VESC;


#endif //__ESC_H__
