/*
 * BLE.h
 *
 * Created: 02.07.2020 17:57:24
 *  Author: dominik hellhake
 */ 
#ifndef BLE_H_
#define BLE_H_

enum class BLE_CMD : uint8_t
{	
	REQUEST_IMU_ValSet		= 0x01,
	RESPONSE_IMU_ValSet		= 0x02,
};


struct BLE_IMU_ValSet_1
{	
	uint8_t ValSetIndex = 1;
	//BNO
	float Avl_Yaw;
	float Avl_Pitch;
	float Avl_Roll;
	float Avl_AccX;
	float Avl_AccY;
	float Avl_AccZ;
	float Avl_GyroX;
	float Avl_GyroY;
	float Avl_GyroZ;
};

#endif /* BLE_H_ */