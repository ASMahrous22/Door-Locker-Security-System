/*
 * buzzer.c
 *
 *  Created on: ٢٧‏/٠٣‏/٢٠٢٤
 *  Author: AS.Mahrous
 */

#include "buzzer.h"
#include "../../MCAL/GPIO/gpio.h"

/*******************************************************************************
*                       Functions Definitions                                  *
*******************************************************************************/
void Buzzer_init(void)
{
	/* setting the buzzer pin as output pin */
	GPIO_setupPinDirection(BUZZER_PORT,BUZZER_PIN,PIN_OUTPUT);

	/* turning the buzzer off at first */
	GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_LOW);
}

void Buzzer_off(void)
{
	/* disabling the buzzer */
	GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_LOW);
}

void Buzzer_on(void)
{
	/* enabling the buzzer */
	GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_HIGH);
}
