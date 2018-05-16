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
	
	if ((EIC->INTFLAG.reg & 0b1110) != 0x00)
	{
		/* Hall Sensor caused interrupt */
		MotorControl::ElectronicPhase_Changed();
		
		EIC->INTFLAG.reg = (1 << 1) | (1 << 2) | (1 << 3);
	} else if ((EIC->INTFLAG.reg & (1 << 13)) != 0x00)
	{
		/* LSM6D->INT1 caused interrupt */
		LSM6D::Update();
		
		EIC->INTFLAG.reg = (1 << 13);
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
	MotorDriver::PhaseDuty = 0x0F;
	MotorControl::Init();
	while (1)
	{
	}
}
