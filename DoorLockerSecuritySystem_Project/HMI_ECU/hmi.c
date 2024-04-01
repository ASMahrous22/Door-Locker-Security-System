/*
 *  Module: HUMAN MACHINE INTERFACE
 *
 *  File Name: hmi.c
 *
 *  Description: Interaction with the user through keypad & displaying on the LCD
 *
 *  Created on: ١٢‏/٠٣‏/٢٠٢٤
 *  Author: AS.Mahrous
 */

#include <avr/io.h>
#include "util/delay.h"
#include "HAL/LCD/lcd.h"
#include "MCAL/UART/uart.h"
#include "HAL/KEYPAD/keypad.h"
#include "MCAL/TIMER/timer1.h"

/*******************************************************************************
*                              Definitions                                     *
*******************************************************************************/
#define PASSWORD_SIZE 5
#define PASSWORD_MATCH 1
#define PASSWORD_UNMATCH 0

/*******************************************************************************
*                             Global Variables                                 *
*******************************************************************************/
/* This variable represents the seconds */
volatile uint8 g_tickTime = 0;

/* it will receive the ACK with 1 or 0 from CONTROL_ECU */
uint8 control_received_data = 0;

/*******************************************************************************
*                              Types Definitions                               *
*******************************************************************************/
/* Setting the UART configurations */
UART_configType UART_settings_mc1 = {EIGHT_BIT,EVEN_PARITY,ONE_STOP_BIT,UART_BAUD_RATE};

/* How the Timer Settings has been Chosen:
 * CPU Frequency (F_CPU) = 8 MHz
 * Prescaler = F_CPU/1024
 * With a prescaler of 1024, the effective clock frequency for Timer1 becomes:
 * Effective Frequency = F_CPU / Prescaler
 *                     = 8 MHz / 1024 = 7812.5 Hz
 * Timer Ticks = Timer Frequency * Time
 *             = 7812.5 * 1 = 7812.5
 * Compare value = Timer Ticks - 1
 *               = 7812.5 - 1 = 7811.5 ~= 7812
 * So, with a compare value of 7811, it means that the Timer1 will overflow exactly once every second. */

 /* TIMER initialization & setting the TIMER configurations. */
TIMER1_configType TIMER1_settings_1 = {0,7812,F_CPU_1024,CTC_OCR1A_TOP};

/*******************************************************************************
*                           Functions Definitions                              *
*******************************************************************************/
/* Description:
 * It is the callback function and it increments the ticks of the timer.
 */
void Timer1_callBack(void)
{
	g_tickTime++;
}

/* Description:
 * Getting the Password from the user & sending it to CONTROL_ECU via UART
 */
void input_Password(void)
{
	uint8 key,idx;

	for (idx = 0; idx < PASSWORD_SIZE; idx++)
	{
		key = KEYPAD_getPressedKey();
		if ((key <= 9) && (key >= 0))
		{
			LCD_displayCharacter('*');
			/* send the pressed key to the CONTROL_ECU */
			UART_sendByte(key);
		}
		else
		{
			/* to make the UART send five numbers only not including any characters */
			idx--;
		}
		_delay_ms(500); /* Press time */
	}

	key = KEYPAD_getPressedKey();
	if (key == KEYPAD_ENTER_KEY)
	{
		/* Receive a byte 1 or 0 to check that the two passwords are matched or not */
		control_received_data = UART_recieveByte();
		/* to give the UART time to receive the byte */
		_delay_ms(500);
	}
}

/* Description:
 * Responsible for changing the password first time and for any time the user wants to change it
 */
void change_Password(void)
{
	uint8 key, idx;
	for (idx = 0; idx < PASSWORD_SIZE ; idx++)
	{
		/* Get the pressed key number,
		 * if any switch pressed for more than 500 ms it will considered more than one press */
		key = KEYPAD_getPressedKey();

		if ((key <= 9) && (key >= 0))
		{
			LCD_displayCharacter('*');
			/* send the pressed key to the CONTROL_ECU */
			UART_sendByte(key);
		}
		else
		{
			/* to make the UART send five numbers only not including any characters */
			idx--;
		}
		_delay_ms(500); /* Press time */
	}

	key = KEYPAD_getPressedKey();
	if (key == KEYPAD_ENTER_KEY)
	{
		/* Asking the user to enter the same password */
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Plz re-enter the");
		LCD_displayStringRowColumn(1, 0, "Same Pass:");
	}
	LCD_moveCursor(1, 11);

	for (idx = 0; idx < PASSWORD_SIZE; idx++)
	{
		key = KEYPAD_getPressedKey();
		if ((key <= 9) && (key >= 0))
		{
			LCD_displayCharacter('*');
			/* send the pressed key to the CONTROL_ECU */
			UART_sendByte(key);
		}
		else
		{
			/* to make the UART send five numbers only not including any characters */
			idx--;
		}
		_delay_ms(500); /* Press time */
	}

	key = KEYPAD_getPressedKey();
	if (key == KEYPAD_ENTER_KEY)
	{
		/* Receive a byte 1 or 0 to check that the two passwords are matched or not */
		control_received_data = UART_recieveByte();
		/* to give the UART time to receive the byte */
		_delay_ms(500);
	}
}

/* Description:
 * displaying the status while opening the door.
 */
void open_Door(void)
{
	/* Sending the & to let the CONTROL_ECU know that the user wants to use open the door */
	UART_sendByte('&');

	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Door is");
	LCD_displayStringRowColumn(1, 0, "Unlocking");
	timer1_Delay(15);

	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "   WARNING!!! ");
	_delay_ms(1500);

	LCD_clearScreen();
	/* warning to warn the user that the door will close in 3 seconds */
	LCD_displayStringRowColumn(0, 0, "Door is");
	LCD_displayStringRowColumn(1, 0, "Closing");
	timer1_Delay(3);

	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Door is");
	LCD_displayStringRowColumn(1, 0, "Locking");
	timer1_Delay(15);
}

/* Description:
 * it triggers the alarm when the password does not match the user's password for 3-consecutive times
 */
void buzzer_Alarm(void)
{
	/* Sending the $ to let the CONTROL_ECU know that the user wants to use open the door */
	UART_sendByte('$');
	LCD_clearScreen();
	LCD_displayString("xxxx ERROR xxxx");
	/* wait until the 1 minute */
	timer1_Delay(60);
}

void system_Options(void)
{
	uint8 getKey;
	/* Display the main options on the screen */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "+ : Open Door");
	LCD_displayStringRowColumn(1, 0, "- : Change Pass");

	getKey = KEYPAD_getPressedKey();
	switch(getKey)
	{
	case '+':
		/* Enter the password to be able to open the door */
		LCD_clearScreen();
		LCD_displayString("Plz Enter Pass:");
		LCD_moveCursor(1,0);
		/* sending # to CONTROL_ECU to be ready for reading the password and check it with the one saved before */
		UART_sendByte('#');
		/* take the password from the user */
		input_Password();

		if (control_received_data == PASSWORD_MATCH)
		{
			/* Open the door */
			open_Door();
		}
		/* If the two passwords don't match then repeat step 1 for the first time */
		else
		{
			LCD_clearScreen();
			LCD_displayString("Incorrect Pass");
			LCD_displayStringRowColumn(1,0,"Pls Try Again");
			_delay_ms(2000);
			LCD_clearScreen();
			LCD_displayString("Plz Enter Pass:");
			LCD_moveCursor(1,0);
			/* Sending the # to let the CONTROL_ECU be ready for reading the password and check on it */
			UART_sendByte('#');
			/* take the password from the user */
			input_Password();

			if (control_received_data == PASSWORD_MATCH)
			{
				/* Open the door */
				open_Door();
			}
			/* If the two passwords don't match then repeat step 1 for the second time */
			else
			{
				LCD_clearScreen();
				LCD_displayString("Incorrect Pass");
				LCD_displayStringRowColumn(1,0,"Pls Try Again");
				_delay_ms(2000);
				LCD_clearScreen();
				LCD_displayString("Plz Enter Pass:");
				LCD_moveCursor(1,0);
				/* Sending the # to let the CONTROL_ECU be ready for reading the password and check on it */
				UART_sendByte('#');
				/* take the password from the user */
				input_Password();

				if (control_received_data == PASSWORD_MATCH)
				{
					/* Open the door */
					open_Door();
				}
				/* If the two passwords don't match for the last time then turn on the buzzer */
				else
				{
					/* Turn on the BUZZER */
					buzzer_Alarm();
				}
			}
		}
		break;

	case '-':
		/* Enter the password to be able to change the password */
		LCD_clearScreen();
		LCD_displayString("Plz Enter Pass:");
		LCD_moveCursor(1, 0);
		/* Sending the # to let the CONTROL_ECU be ready for reading the password and check on it */
		UART_sendByte('#');
		/* take the password from the user */
		input_Password();
		if (control_received_data == PASSWORD_MATCH) /* Receive 1 for matched passwords */
		{
			LCD_clearScreen();
			LCD_displayString("Plz enter pass:");
			LCD_moveCursor(1, 0);
			/* Sending * to make the CONTROL_ECU ready for reading the password and save it */
			UART_sendByte('*');
			/* calling the function change_Password to give the password to the system and save it */
			change_Password();
		}
		/* If the two passwords don't match then repeat step 1 for the first time */
		else
		{
			LCD_clearScreen();
			LCD_displayString("Incorrect Pass");
			LCD_displayStringRowColumn(1,0,"Pls Try Again");
			_delay_ms(2000);
			/* Enter the password to be able to change the password */
			LCD_clearScreen();
			LCD_displayString("Plz Enter Pass:");
			LCD_moveCursor(1, 0);
			/* Sending the # to let the CONTROL_ECU be ready for reading the password and check on it */
			UART_sendByte('#');
			/* take the password from the user */
			input_Password();
			if (control_received_data == PASSWORD_MATCH) /* Receive 1 for matched passwords */
			{
				LCD_clearScreen();
				LCD_displayString("Plz enter pass:");
				LCD_moveCursor(1, 0);
				/* Sending * to make the CONTROL_ECU ready for reading the password and save it */
				UART_sendByte('*');
				/* calling the function change_Password to give the password to the system and save it */
				change_Password();
			}
			/* If the two passwords don't match then repeat step 1 for the second time */
			else
			{
				LCD_clearScreen();
				LCD_displayString("Incorrect Pass");
				LCD_displayStringRowColumn(1,0,"Pls Try Again");
				_delay_ms(2000);
				/* Enter the password to be able to change the password */
				LCD_clearScreen();
				LCD_displayString("Plz Enter Pass:");
				LCD_moveCursor(1, 0);
				/* Sending the # to let the CONTROL_ECU be ready for reading the password and check on it */
				UART_sendByte('#');
				/* take the password from the user */
				input_Password();
				/* Sending the letter R to notify CONTROL_ECU that the user wants to change the password */
				UART_sendByte('R');
				if (control_received_data == PASSWORD_MATCH) /* Receive 1 for matched passwords */
				{
					LCD_clearScreen();
					LCD_displayString("Plz enter pass:");
					LCD_moveCursor(1, 0);
					/* Sending * to make the CONTROL_ECU ready for reading the password and save it */
					UART_sendByte('*');
					/* calling the function change_Password to give the password to the system and save it */
					change_Password();
				}
				/* If the two passwords don't match for the last time then turn on the buzzer */
				else
				{
					/* Turn on the BUZZER */
					buzzer_Alarm();
				}
			}
		}
		break;
	}
}

/* Description:
 * it makes a delay in seconds by the use of timer1
 */
void timer1_Delay(uint8 sec)
{
    /* Timer1 initialization */
    TIMER1_init(&TIMER1_settings_1);

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
    /* enabling interrupt bit "I-bit" */
    SREG |= (1<<7);

    /* UART initialization */
    UART_init(&UART_settings_mc1);

    /* Timer1 initialization */
    TIMER1_init(&TIMER1_settings_1);

	/* Setting the TIMER1_callBack to be the callback function */
	TIMER1_setCallBack(Timer1_callBack);

	/* LCD initialization */
	LCD_init();

	LCD_displayString("Plz Enter Pass:");
	LCD_moveCursor(1, 0);
	/* Sending * to make the CONTROL_ECU ready for reading the password and save it */
	UART_sendByte('*');
	/* Step.1 : calling the function CHANGE_PASSWORD to give the password to the system and save it */
	change_Password();
	/* if the two passwords are matched  we will receive 1 */
	if (control_received_data == PASSWORD_MATCH)
	{
		/* Go to step 2 */
	}
	/* if the two passwords don't match repeat step 1 for 2nd time  */
	else
	{
		LCD_clearScreen();
		LCD_displayString("Incorrect Pass");
		LCD_displayStringRowColumn(1,0,"Pls Try Again");
		timer1_Delay(2);
		LCD_clearScreen();
		LCD_displayString("Plz Enter Pass:");
		LCD_moveCursor(1, 0);
		UART_sendByte('*');
		change_Password();
		if (control_received_data == PASSWORD_MATCH)
		{
			/* Go to step 2 */
		}
		/* if the two passwords don't match repeat step 1 for last time  */
		else
		{
			LCD_clearScreen();
			LCD_displayString("Incorrect Pass");
			LCD_displayStringRowColumn(1,0,"Plz Try Again");
			timer1_Delay(2);
			LCD_clearScreen();
			LCD_displayString("Plz Enter Pass:");
			LCD_moveCursor(1, 0);
			UART_sendByte('*');
			change_Password();
			if (control_received_data == PASSWORD_MATCH)
			{
				/* Go to step 2 */
			}
			/* if the two passwords don't match again, DISPLAY an ERROR and close the system */
			else
			{
				LCD_clearScreen();
				LCD_displayString("  SYSTEM ERROR ");
				timer1_Delay(2);
				return 0;
			}
		}
	}

	for(;;)
	{
		system_Options();
	}
}
