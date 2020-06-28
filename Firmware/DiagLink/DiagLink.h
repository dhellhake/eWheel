/* 
* DiagLink.h
*
* Created: 03.06.2020 20:11:20
* Author: dominik helhake
*/
#ifndef __DIAGLINK_H__
#define __DIAGLINK_H__

#include "sam.h"
#include "Diagnostics.h"
#include "..\Task.h"

#include "..\Peripheral\ADS1118\ADS1118.h"
#include "..\Peripheral\BNO055\BNO055.h"
#include "..\Peripheral\VESC\VESC.h"

#define DIAGNOSTIC_RCV_BUF_SIZE	128

struct TRACE_PAGE
{
	uint16_t Preamble = 0xAAAA;
	uint32_t timeStamp;
	
	//BNO
	float Yaw;
	float Roll;
	float Pitch;	
	float AccX;
	float AccY;
	float AccZ;
	float GyroX;
	float GyroY;
	float GyroZ;
	float LinAX;
	float LinAY;
	float LinAZ;
	float GravX;
	float GravY;
	float GravZ;	
	int8_t Temp;
	uint8_t CalibStatus;
	
	//ADS	
	float Avl_Vfront;
	ADS_QU_Voltage Qu_Vfront;	
	float Avl_Vback;
	ADS_QU_Voltage Qu_Vback;
	
	//VESC		
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
	float Tar_Duty;
};


class DiagLink : public Task
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual RUN_RESULT Run(uint32_t timeStamp);

	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	private:		
		DIAGNOSTIC_MODE Mode;
	
		void SectorErase(uint16_t sector);
		void ResponseMode(DIAGNOSTIC_MODE mode);
		void ResponseFlashPage(uint16_t pageIndex);
		void SampleTracePage(uint32_t timeStamp);
		TRACE_PAGE page;
		
	public:
		bool Recording;
		uint8_t PageCount = 0;
	
		uint8_t ReceiveBufferIndex = 0;
		uint8_t ReceiveBuffer[DIAGNOSTIC_RCV_BUF_SIZE];
		
		DiagLink();
		
		void SetDiagnosticMode();	
}; //DiagLink

extern DiagLink Diagnostic;

#endif //__DIAGLINK_H__
