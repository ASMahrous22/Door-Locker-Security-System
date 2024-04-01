/*
 * buzzer.h
 *
 *  Created on: ٢٧‏/٠٣‏/٢٠٢٤
 *  Author: AS.Mahrous
 */

#ifndef HAL_BUZZER_BUZZER_H_
#define HAL_BUZZER_BUZZER_H_

/*******************************************************************************
*                              Definitions                                     *
*******************************************************************************/
#define BUZZER_PIN PIN0_ID
#define BUZZER_PORT PORTA_ID

/*******************************************************************************
*                       Functions Prototypes                                   *
*******************************************************************************/
/* Description:
⮚ Setup the direction for the buzzer pin as output pin through the GPIO driver.
⮚ Turn off the buzzer through the GPIO.
*/
void Buzzer_init(void);

/* Description
⮚ Function to enable the Buzzer through the GPIO.
*/
void Buzzer_on(void);

/* Description
⮚ Function to disable the Buzzer through the GPIO.
*/
void Buzzer_off(void);

#endif /* HAL_BUZZER_BUZZER_H_ */
