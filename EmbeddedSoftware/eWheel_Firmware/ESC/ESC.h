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
	virtual bool CanExecute()
	{
		if (this->CANRcvBufferIndex == 0)
			return false;
			
		return true;
	};
	
	virtual RUN_RESULT Run(uint32_t timeStamp);
	virtual void Propagate();


	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	private:		
		volatile uint8_t CANRcvBufferIndex = 0;
		VESCPackage CANRcvBuffer[CAN_RECEIVE_BUFFER_SIZE];
		
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
		float RPM;
		float Current;
		float Duty;
		// CAN_PACKET_STATUS_2
		float Ah;
		float AhCharged;
		// CAN_PACKET_STATUS_3
		float Wh;
		float WhCharged;
		// CAN_PACKET_STATUS_4
		float TempFET;
		float TempMotor;
		float CurrentIn;
		float PIDPosNow;
	
		ESC();
		void ReceiveVESCPackage(uint8_t id, uint8_t *data);
}; //ESC

#endif //__ESC_H__
