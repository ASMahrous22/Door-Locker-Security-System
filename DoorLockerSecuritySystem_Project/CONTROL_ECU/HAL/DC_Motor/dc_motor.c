/*
 * dc_motor.c
 *
 *  Created on: ٢٥‏/٠٢‏/٢٠٢٤
 *      Author: AS.Mahrous
 */

#include "dc_motor.h"
#include "PWM/pwm_timer0.h"
#include "../../MCAL/GPIO/gpio.h"

void DcMotor_init(void)
{
	/* setting the two motor pins as output */
	GPIO_setupPinDirection(MOTOR_PORT,MOTOR_IN1,PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PORT,MOTOR_IN2,PIN_OUTPUT);

	/* stopping the motor */
	GPIO_writePin(MOTOR_PORT,MOTOR_IN1,LOGIC_LOW);
	GPIO_writePin(MOTOR_PORT,MOTOR_IN2,LOGIC_LOW);
}

void DcMotor_Rotate(DcMotor_State state,uint8 speed)
{
	switch(state)
	{
	case STOP:
		speed = 0;
		PWM_Timer0_Start(speed);

		/* setting direction of the motor to stop */
		GPIO_writePin(MOTOR_PORT,MOTOR_IN1,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT,MOTOR_IN2,LOGIC_LOW);
		break;

	case CW:
		/* setting direction of the motor to rotate clockwise */
		GPIO_writePin(MOTOR_PORT,MOTOR_IN1,LOGIC_HIGH);
		GPIO_writePin(MOTOR_PORT,MOTOR_IN2,LOGIC_LOW);

		/* setting "speed = duty cycle" by which the motor rotates */
		PWM_Timer0_Start(speed);
		break;

	case A_CW:
		/* setting direction of the motor to rotate anti-clockwise */
		GPIO_writePin(MOTOR_PORT,MOTOR_IN1,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT,MOTOR_IN2,LOGIC_HIGH);

		/* setting "speed = duty cycle" by which the motor rotates */
		PWM_Timer0_Start(speed);
		break;
	}
}
