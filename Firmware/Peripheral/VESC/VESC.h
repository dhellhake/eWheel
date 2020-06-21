/* 
* VESC.h
*
* Created: 21.05.2020 15:10:08
* Author: dominik hellhake
*/
#ifndef __VESC_H__
#define __VESC_H__

#include "sam.h"
#include "..\..\Task.h"

#define VESC_USART			USART0
#define VESC_RCV_BUF_SIZE	128

enum class VESCFaultCode {
	FAULT_CODE_NONE = 0,
	FAULT_CODE_OVER_VOLTAGE,
	FAULT_CODE_UNDER_VOLTAGE,
	FAULT_CODE_DRV,
	FAULT_CODE_ABS_OVER_CURRENT,
	FAULT_CODE_OVER_TEMP_FET,
	FAULT_CODE_OVER_TEMP_MOTOR,
	FAULT_CODE_GATE_DRIVER_OVER_VOLTAGE,
	FAULT_CODE_GATE_DRIVER_UNDER_VOLTAGE,
	FAULT_CODE_MCU_UNDER_VOLTAGE,
	FAULT_CODE_BOOTING_FROM_WATCHDOG_RESET,
	FAULT_CODE_ENCODER_SPI,
	FAULT_CODE_ENCODER_SINCOS_BELOW_MIN_AMPLITUDE,
	FAULT_CODE_ENCODER_SINCOS_ABOVE_MAX_AMPLITUDE,
	FAULT_CODE_FLASH_CORRUPTION,
	FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_1,
	FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_2,
	FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_3,
	FAULT_CODE_UNBALANCED_CURRENTS,
	FAULT_CODE_RESOLVER_LOT,
	FAULT_CODE_RESOLVER_DOS,
	FAULT_CODE_RESOLVER_LOS
};

enum class VESCPackageType
{
	COMM_FW_VERSION = 0,
	COMM_JUMP_TO_BOOTLOADER,
	COMM_ERASE_NEW_APP,
	COMM_WRITE_NEW_APP_DATA,
	COMM_GET_VALUES,
	COMM_SET_DUTY,
	COMM_SET_CURRENT,
	COMM_SET_CURRENT_BRAKE,
	COMM_SET_RPM,
	COMM_SET_POS,
	COMM_SET_HANDBRAKE,
	COMM_SET_DETECT,
	COMM_SET_SERVO_POS,
	COMM_SET_MCCONF,
	COMM_GET_MCCONF,
	COMM_GET_MCCONF_DEFAULT,
	COMM_SET_APPCONF,
	COMM_GET_APPCONF,
	COMM_GET_APPCONF_DEFAULT,
	COMM_SAMPLE_PRINT,
	COMM_TERMINAL_CMD,
	COMM_PRINT,
	COMM_ROTOR_POSITION,
	COMM_EXPERIMENT_SAMPLE,
	COMM_DETECT_MOTOR_PARAM,
	COMM_DETECT_MOTOR_R_L,
	COMM_DETECT_MOTOR_FLUX_LINKAGE,
	COMM_DETECT_ENCODER,
	COMM_DETECT_HALL_FOC,
	COMM_REBOOT,
	COMM_ALIVE,
	COMM_GET_DECODED_PPM,
	COMM_GET_DECODED_ADC,
	COMM_GET_DECODED_CHUK,
	COMM_FORWARD_CAN,
	COMM_SET_CHUCK_DATA,
	COMM_CUSTOM_APP_DATA,
	COMM_NRF_START_PAIRING,
	COMM_GPD_SET_FSW,
	COMM_GPD_BUFFER_NOTIFY,
	COMM_GPD_BUFFER_SIZE_LEFT,
	COMM_GPD_FILL_BUFFER,
	COMM_GPD_OUTPUT_SAMPLE,
	COMM_GPD_SET_MODE,
	COMM_GPD_FILL_BUFFER_INT8,
	COMM_GPD_FILL_BUFFER_INT16,
	COMM_GPD_SET_BUFFER_INT_SCALE,
	COMM_GET_VALUES_SETUP,
	COMM_SET_MCCONF_TEMP,
	COMM_SET_MCCONF_TEMP_SETUP,
	COMM_GET_VALUES_SELECTIVE,
	COMM_GET_VALUES_SETUP_SELECTIVE,
	COMM_EXT_NRF_PRESENT,
	COMM_EXT_NRF_ESB_SET_CH_ADDR,
	COMM_EXT_NRF_ESB_SEND_DATA,
	COMM_EXT_NRF_ESB_RX_DATA,
	COMM_EXT_NRF_SET_ENABLED,
	COMM_DETECT_MOTOR_FLUX_LINKAGE_OPENLOOP,
	COMM_DETECT_APPLY_ALL_FOC,
	COMM_JUMP_TO_BOOTLOADER_ALL_CAN,
	COMM_ERASE_NEW_APP_ALL_CAN,
	COMM_WRITE_NEW_APP_DATA_ALL_CAN,
	COMM_PING_CAN,
	COMM_APP_DISABLE_OUTPUT,
	COMM_TERMINAL_CMD_SYNC,
	COMM_GET_IMU_DATA,
	COMM_BM_CONNECT,
	COMM_BM_ERASE_FLASH_ALL,
	COMM_BM_WRITE_FLASH,
	COMM_BM_REBOOT,
	COMM_BM_DISCONNECT,
	COMM_BM_MAP_PINS_DEFAULT,
	COMM_BM_MAP_PINS_NRF5X,
	COMM_ERASE_BOOTLOADER,
	COMM_ERASE_BOOTLOADER_ALL_CAN,
	COMM_PLOT_INIT,
	COMM_PLOT_DATA,
	COMM_PLOT_ADD_GRAPH,
	COMM_PLOT_SET_GRAPH,
	COMM_GET_DECODED_BALANCE,
	COMM_BM_MEM_READ,
	COMM_WRITE_NEW_APP_DATA_LZO,
	COMM_WRITE_NEW_APP_DATA_ALL_CAN_LZO,
	COMM_BM_WRITE_FLASH_LZO,
	COMM_SET_CURRENT_REL,
	COMM_CAN_FWD_FRAME
};

enum class VESC_COM_STATE
{
	Waiting,
	Requested,
	Received
};

class VESC : public Task
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual RUN_RESULT Run(uint32_t timeStamp);

	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	private:		
		VESC_COM_STATE ComState = VESC_COM_STATE::Waiting;
		uint8_t ReceiveBufferIndex = 0;
		uint8_t ReceiveBuffer[VESC_RCV_BUF_SIZE];
			
		void SendVESCPacket(VESCPackageType type, uint8_t* payload, uint16_t length);
		void UnpackVESCPacket(uint8_t* payload, uint16_t length);	
	
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
			
		void ReceiveByte(uint8_t data);
		void Update();		
		void SetCurrent(float current_val);
		void SetDuty(float duty_val);
	
	
	
		static inline void buffer_set_int32(uint8_t *buffer, int32_t value)
		{
			uint8_t *v = (uint8_t*)&value;
			for (uint8_t i = 0; i < 4; i++)
				buffer[i] = v[3-i];
		}
		static inline int32_t buffer_get_int32(const uint8_t *buffer)
		{
			return 	buffer[0] << 24		|
					buffer[1] << 16		|
					buffer[2] << 8		|
					buffer[3];
		}
		static inline int16_t buffer_get_int16(const uint8_t *buffer)
		{
			return	buffer[0] << 8 | buffer[1];
		}
		static inline float buffer_get_float16(const uint8_t *buffer, float scale)
		{
			return (float)buffer_get_int16(buffer) / scale;
		}
		static inline float buffer_get_float32(const uint8_t *buffer, float scale)
		{
			return (float)buffer_get_int32(buffer) / scale;
		}
}; //VESC

extern VESC ESC;

#endif //__VESC_H__
