/*
 * dc_motor.h
 *
 *  Created on: ٢٥‏/٠٢‏/٢٠٢٤
 *  Author: AS.Mahrous
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "../../MCAL/std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define MOTOR_IN1 PIN0_ID
#define MOTOR_IN2 PIN1_ID

#define MOTOR_PORT PORTB_ID

#define MAX_SPEED 255
/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum
{
	STOP,CW,A_CW
}DcMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/* Description:
 ➢ The Function responsible for setup the direction for the two motor pins through the GPIO driver.
 ➢ Stop at the DC-Motor at the beginning through the GPIO driver.
*/
void DcMotor_init(void);

/* Description:
 ➢ The function responsible for rotate the DC Motor CW , A-CW or stop the motor based on the input state value.
 ➢ Send the required duty cycle to the PWM driver based on the required speed value.
*/
void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif /* DC_MOTOR_H_ */
