/*
 * SandBox.cpp
 *
 * Created: 30.04.2018 15:10:53
 * Author : dominik hellhake
 */ 


#include "sam.h"
#include "LowLevel/System/System.h"
#include "DebugPort/DebugPort.h"
#include "LSM6D/LSM6D.h"
#include "MotorControl/MotorControl.h"

#include "MotorControl/MotorDriver/MotorDriver.h"

void EIC_Handler()
{
	
	if ((EIC->INTFLAG.reg & ((1 << 7) | (1 << 12) | (1 << 13))) != 0x00)
	{
		/* Hall Sensor caused interrupt */
		MotorControl::ElectronicPhase_Changed();
				
		EIC->INTFLAG.reg = (1 << 7) | (1 << 12) | (1 << 13);
	} else if ((EIC->INTFLAG.reg & (1 << 2)) != 0x00)
	{
		/* LSM6D->INT1 caused interrupt */
		LSM6D::Update();
		
		EIC->INTFLAG.reg = (1 << 2);
	}
}

int main(void)
{
    /* Initialize the SAM system */
    System::Init();
	/* Initialize the debug serial port */
	DebugPort::Init();
	/* Initialize the lsm6d sensor */
	LSM6D::Init();
	/* Initialize BLDC Motor Control */
	MotorDriver::PhaseDuty = 1200;
	MotorControl::Init();
	
	
	uint8_t result = LSM6D::ReadRegister(LSM6DS3_WHO_AM_I_REG);
	
	result *=2;
	
	while (1)
	{
	}
}
