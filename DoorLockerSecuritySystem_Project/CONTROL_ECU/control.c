/*
 * Module: CONTROL UNIT
 *
 * File Name: control.c
 *
 * Description: responsible for all the processing and taking decisions in the system like password
 *              checking, opening the door and activating the alarm system
 *
 *  Created on: ١٢‏/٠٣‏/٢٠٢٤
 *  Author: AS.Mahrous
 */

#include <avr/io.h>
#include "util/delay.h"
#include "MCAL/I2C/i2c.h"
#include "MCAL/UART/uart.h"
#include "HAL/BUZZER/buzzer.h"
#include "HAL/EEPROM/eeprom.h"
#include "HAL/DC_Motor/dc_motor.h"
#include "MCAL/TIMER/timer1.h"

/*******************************************************************************
*                              Definitions                                     *
*******************************************************************************/
#define MATCHED 1
#define UNMATCHED 0
#define PASSWORD_SIZE 5

/*******************************************************************************
*                            Variable Definitions                              *
*******************************************************************************/
volatile uint8 g_tickTime = 0;

/*******************************************************************************
*                            Types Definitions                                 *
*******************************************************************************/
/* Setting the I2C configurations.
 * address: device address 10
 * bit-rate: 400000 kbps
 */
TWI_ConfigType I2C_settings={0x0A,I2C_BAUD_RATE_400K};

/* Setting the UART configurations */
UART_configType UART_settings_mc2 = {EIGHT_BIT,EVEN_PARITY,ONE_STOP_BIT,UART_BAUD_RATE};

/* How the Timer Settings has been Chosen:
 * CPU Frequency (F_CPU) = 8 MHz
 * Prescaler = F_CPU/256
 * With a prescaler of 256, the effective clock frequency for Timer1 becomes:
 * Effective Frequency = F_CPU / Prescaler
 *                     = 8 MHz / 256 = 31250 Hz
 * Timer Ticks = Timer Frequency * Time
 *             = 31250 * 1 = 31250
 * Compare value = Timer Ticks - 1
 *               = 31250 - 1 = 31249
 * So, with a compare value of 31249, it means that the Timer1 will overflow exactly once every second. */

 /* Setting the TIMER configurations. */
TIMER1_configType TIMER1_settings_2 = {0,31249,F_CPU_256,CTC_OCR1A_TOP};

/*******************************************************************************
*                           Functions Definitions                              *
*******************************************************************************/
/* Description:
 *  * It is the callback function and it increments the ticks of the timer.
 */
void Timer1_callBack(void)
{
	g_tickTime++;
}

/* Description:
 * It receives the re-Entered Password from HMI_ECU & saves it in the EEPROM.
 * It also checks whether the received Password matches the saved one or not.
 */
void Password_Checker(void)
{
	uint8 idx=0,check_counter=0,received_key=0,save_key=0,saved_1=0,saved_2=0;

	/* This loop is to Receive & Save the re-Entered Password */
	for (idx = 6; idx < PASSWORD_SIZE+6 ; idx++)
	{

		received_key = UART_recieveByte();					       /* Receive the key from HMI_ECU */
		save_key = EEPROM_writeByte((0x01) + idx, received_key);   /* Save the key in the EEPROM */

		_delay_ms(500);								 /* delay to give the EEPROM the time to save successfully */
	}

	/* checking the two received passwords byte by byte */
	for (idx = 0;idx < PASSWORD_SIZE; idx++)
	{
		EEPROM_readByte((0x01) + idx, &saved_1);
		_delay_ms(10);
		EEPROM_readByte((0x07) + idx, &saved_2);
		_delay_ms(10);
		if (saved_1 == saved_2)
		{
			check_counter++;       /* If the two bytes are matched the check counter will be incremented */
		}
	}
	if (check_counter == PASSWORD_SIZE)
	{
		UART_sendByte(MATCHED);   /* Sending 1 to HMI_ECU to let it know that the two passwords are matched */
	}
	else
	{
		UART_sendByte(UNMATCHED); /* Sending 0 to HMI_ECU to let it know that the two passwords are not matched */
	}
	check_counter = 0;            /* reset the check counter for another check */
}

/* Description:
 * It receives the Password for the 1st time or when changing it from HMI_ECU & saves it in the EEPROM.
 * It also checks whether the received Passwords are matched or not.
 */
void createPass_changePass(void)
{
	uint8 idx=0,check_counter=0,received_key=0,save_key=0,saved_1=0,saved_2=0;

	/* This loop is to Receive & Save the 1st Entered Password */
	for (idx = 0; idx < PASSWORD_SIZE ; idx++)
	{

		received_key = UART_recieveByte();					       /* Receive the key from HMI_ECU */
		save_key = EEPROM_writeByte((0x01) + idx, received_key);   /* Save the key in the EEPROM */

		_delay_ms(500);								 /* delay to give the EEPROM the time to save successfully */
	}

	/* This loop is to Receive & Save the re-Entered Password */
	for (idx = 6; idx < PASSWORD_SIZE+6 ; idx++)
	{

		received_key = UART_recieveByte();					       /* Receive the key from HMI_ECU */
		save_key = EEPROM_writeByte((0x01) + idx, received_key);   /* Save the key in another place in EEPROM */

		_delay_ms(500);								 /* delay to give the EEPROM the time to save successfully */
	}

	/* checking the two received passwords byte by byte */
	for (idx = 0;idx < PASSWORD_SIZE; idx++)
	{
		EEPROM_readByte((0x01) + idx, &saved_1);
		_delay_ms(10);
		EEPROM_readByte((0x07) + idx, &saved_2);
		_delay_ms(10);
		if (saved_1 == saved_2)
		{
			check_counter++;       /* If the two bytes are matched the check counter will be incremented */
		}
	}
	if (check_counter == PASSWORD_SIZE)
	{
		UART_sendByte(MATCHED);   /* Sending 1 to HMI_ECU to let it know that the two passwords are matched */
	}
	else
	{
		UART_sendByte(UNMATCHED); /* Sending 0 to HMI_ECU to let it know that the two passwords are not matched */
	}
	check_counter = 0;            /* reset the check counter for another check */
}

void timer1_Delay(uint8 sec)
{
    /* Timer1 initialization */
    TIMER1_init(&TIMER1_settings_2);

	/* Setting the TIMER1_callBack to be the callback function */
	TIMER1_setCallBack(Timer1_callBack);

	/* reset the timer counter to start counting from 0 */
	g_tickTime = 0;

	/* wait until the input seconds */
	while (g_tickTime <= sec);

	TIMER1_deInit();

	/* reset the timer counter again to get the needed seconds */
	g_tickTime = 0;
}
/*******************************************************************************
*                              Main Code                                       *
*******************************************************************************/
int main(void)
{
	/* Variable to receive the sent data from HMI_ECU */
	uint8 hmi_received_data = 0;

    /* enabling interrupt bit "I-bit" */
    SREG |= (1<<7);

	/* I2C initialization */
	TWI_init(&I2C_settings);

	/* UART initialization */
	UART_init(&UART_settings_mc2);

	 /* TIMER initialization */
	TIMER1_init(&TIMER1_settings_2);

	/* Setting the TIMER1_callBack to be the callback function */
	TIMER1_setCallBack(Timer1_callBack);

	/* Initializing the DC-MOTOR */
	DcMotor_init();

	/* Initializing the BUZZER */
	Buzzer_init();

	for(;;)
	{
		hmi_received_data = UART_recieveByte();

		switch(hmi_received_data)
		{
		case '*': /* The user will change the password or enter the password for the 1st time */

			/* call createPass_changePass function to do this action */
			createPass_changePass();

			break;

		case '#': /* The user will re-Enter the password to either open the door or change password */

			/* call PASS_CHECK function to check whether the password entered matches the user's password or not */
			Password_Checker();

			break;

		case '&': /* The user wants to open the door */

			/* OPEN the door for 15 seconds */
			DcMotor_Rotate(CW, MAX_SPEED);
			/* Delay with using timer1 */
			timer1_Delay(15);

			/* HOLD the door for 3 seconds */
			DcMotor_Rotate(STOP, 0);
			/* Delay with using timer1 */
			timer1_Delay(3);

			/* CLOSE the door for 15 seconds */
			DcMotor_Rotate(A_CW, MAX_SPEED);
			/* Delay with using timer1 */
			timer1_Delay(15);

			g_tickTime = 0;                         /* reset the timer counter */
			DcMotor_Rotate(STOP, 0);                /* Stopping the motor */
			break;

		case '$': /* The user entered the wrong password 3-times ,so the Alarm must be ON */

			g_tickTime = 0;
			/* Activate the BUZZER for 1 minute */
			Buzzer_on();
			/* Delay with using timer1 */
			timer1_Delay(60);

			g_tickTime = 0;
			/* Turn off the BUZZER after 1 minute */
			Buzzer_off();

			break;
		}
	}
}
