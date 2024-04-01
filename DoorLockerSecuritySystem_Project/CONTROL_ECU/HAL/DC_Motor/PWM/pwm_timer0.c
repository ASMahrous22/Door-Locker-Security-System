/*
 * pwm_timer0.c
 *
 *  Created on: ٢٥‏/٠٢‏/٢٠٢٤
 *  Author: AS.Mahrous
 */

#include <avr/io.h>
#include "pwm_timer0.h"
#include "../../../MCAL/GPIO/gpio.h"


void PWM_Timer0_Start(uint8 duty_cycle)
{
	/* Set Timer Initial Value to 0 */
	TCNT0 = 0;

   /*  configure the timer :
	* 1. Fast PWM mode FOC0=0
	* 2. Fast PWM Mode WGM01=1 & WGM00=1
	* 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	* 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
    */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);

	/* setting the compare value based on duty cycle */
	OCR0 = duty_cycle ;

	/* setting PB3/OC0 as output pin, this pin where the PWM signal is generated from MC. */
	GPIO_setupPinDirection(PORTB_ID,PIN3_ID,PIN_OUTPUT);
}
