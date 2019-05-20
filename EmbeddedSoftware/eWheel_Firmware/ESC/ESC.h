/* 
* ESC.h
*
* Created: 14.04.2019 15:02:13
* Author: Dominik Hellhake
*/
#ifndef __ESC_H__
#define __ESC_H__

#include "samc21.h"
#include "..\Executable.h"

#define STATUS_None_RECEIVED	0
#define STATUS_1_RECEIVED		1
#define STATUS_2_RECEIVED		2
#define STATUS_3_RECEIVED		4
#define STATUS_4_RECEIVED		8
#define STATUS_COMPLETE			0xF


#define CAN_RECEIVE_BUFFER_SIZE 4

#define VESC_PACKAGE_SIZE 8

enum class VESCPackageType
{
	CAN_PACKET_SET_DUTY = 0,
	CAN_PACKET_SET_CURRENT,
	CAN_PACKET_SET_CURRENT_BRAKE,
	CAN_PACKET_SET_RPM,
	CAN_PACKET_SET_POS,
	CAN_PACKET_FILL_RX_BUFFER,
	CAN_PACKET_FILL_RX_BUFFER_LONG,
	CAN_PACKET_PROCESS_RX_BUFFER,
	CAN_PACKET_PROCESS_SHORT_BUFFER,
	CAN_PACKET_STATUS,
	CAN_PACKET_SET_CURRENT_REL,
	CAN_PACKET_SET_CURRENT_BRAKE_REL,
	CAN_PACKET_SET_CURRENT_HANDBRAKE,
	CAN_PACKET_SET_CURRENT_HANDBRAKE_REL,
	CAN_PACKET_STATUS_2,
	CAN_PACKET_STATUS_3,
	CAN_PACKET_STATUS_4,
	CAN_PACKET_PING,
	CAN_PACKET_PONG,
	CAN_PACKET_DETECT_APPLY_ALL_FOC,
	CAN_PACKET_DETECT_APPLY_ALL_FOC_RES,
	CAN_PACKET_CONF_CURRENT_LIMITS,
	CAN_PACKET_CONF_STORE_CURRENT_LIMITS,
	CAN_PACKET_CONF_CURRENT_LIMITS_IN,
	CAN_PACKET_CONF_STORE_CURRENT_LIMITS_IN,
	CAN_PACKET_CONF_FOC_ERPMS,
	CAN_PACKET_CONF_STORE_FOC_ERPMS
};


typedef struct {
	VESCPackageType _type;
	uint8_t _data[VESC_PACKAGE_SIZE];
} VESCPackage;


class ESC : public Executable
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/	
	virtual bool IsReady(uint32_t timeStamp)
	{
		if (this->CANRcvBufferIndex > 0)
			return true;
		else if (timeStamp - this->LastTarValueUpdate >= 100)
			return true;
			
		return false;
	}	
	virtual RUN_RESULT Run(uint32_t timeStamp);
	virtual void ResetTask() 
	{
		this->VESTStatusReceived = STATUS_None_RECEIVED; 
		this->TaskStatus = TASK_STATUS::WAIT; 
	}

	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	private:		
		volatile uint8_t CANRcvBufferIndex = 0;
		VESCPackage CANRcvBuffer[CAN_RECEIVE_BUFFER_SIZE];
		
		static inline void buffer_set_int32(uint8_t *buffer, int32_t value)
		{
			uint8_t *v = (uint8_t*)&value;
			for (uint8_t i = 0; i < 4; i++)
				buffer[i] = v[3-i];						
		}
		
		static inline int32_t buffer_get_int32(const uint8_t *buffer, uint8_t index)
		{
			return 	(buffer[index] << 24)		|
					(buffer[index + 1]) << 16	|
					(buffer[index + 2]) << 8	|
					(buffer[index + 3]);
		}
		static inline int16_t buffer_get_int16(const uint8_t *buffer, uint8_t index)
		{
			return	(buffer[index]) << 8 |
					(buffer[index + 1]);
		}
	public:
		// CAN_PACKET_STATUS
		float Avl_RPM;
		float Avl_Current;
		float Avl_Duty;
		// CAN_PACKET_STATUS_2
		float Avl_Ah;
		float Avl_AhCharged;
		// CAN_PACKET_STATUS_3
		float Avl_Wh;
		float Avl_WhCharged;
		// CAN_PACKET_STATUS_4
		float Avl_TempFET;
		float Avl_TempMotor;
		float Avl_CurrentIn;
		float Avl_PIDPosNow;
		
		// Duty Cycle [0.0 to 1.0]
		float Tar_Duty;
		// HandBrake in Ampere
		float Tar_HandBrake;
		// Brake in Ampere
		float Tar_Brake;
		
		ESC();
		
		void ResetTarValues();
		
		inline void SetHandBrake(float value)
		{
			this->ResetTarValues();
			
			if (value < 0.0f)
				this->Tar_HandBrake = 0.0f;
			else if (value > 10.0f)
				this->Tar_HandBrake = 10.0f;
			else
				this->Tar_HandBrake = value;			
		}
		
		
		inline void SetBrake(float value)
		{
			this->ResetTarValues();
			
			if (value < 0.0f)
				this->Tar_Brake = 0.0f;
			else if (value > 10.0f)
				this->Tar_Brake = 10.0f;
			else
				this->Tar_Brake = value;
		}
		
		inline void SetTar_Duty(float value)
		{
			this->ResetTarValues();
			
			if (value > 0.20f)
				this->Tar_Duty = 0.20f;
			else if (value < 0.0f)
				this->Tar_Duty = 0.0f;
			else
				this->Tar_Duty = value;
		}
		
		void ReceiveVESCPackage(uint8_t id, uint8_t *data);
	private:
		uint8_t VESTStatusReceived;
	
		uint32_t LastTarValueUpdate;
	
		void ProcessVESCPackages();
		void SendTarValues(uint32_t timeStamp);
}; //ESC
extern ESC VESC;


#endif //__ESC_H__
