/*
 * Module: Timer/Counter1
 *
 * File Name: timer1.c
 *
 * Description: Source file for the AVR Timer/Counter1
 *
 *  Created on: ١٢‏/٠٣‏/٢٠٢٤
 *  Author: AS.Mahrous
 */

#include "timer1.h"
#include <avr/io.h>             /* To use the Timer1 Registers */
#include <avr/interrupt.h>
#include "../common_macros.h"   /* To use the macros like SET_BIT */

/*******************************************************************************
*                            Global Variables                                  *
*******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
*                                   ISRs                                       *
*******************************************************************************/
#ifdef COMPARE_MODE

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
		{
			/* call the function using pointer to function g_callBackPtr();*/
			(*g_callBackPtr)();
		}
}

#endif

#ifdef NORMAL_MODE

ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
			{
				/* call the function using pointer to function g_callBackPtr();*/
				(*g_callBackPtr)();
			}
}

#endif

/*******************************************************************************
*                       Functions Definitions                                  *
*******************************************************************************/

void TIMER1_init(const TIMER1_configType * Config_Ptr)
{
	/* setting FOC1A & FOC1B as we are in non-pwm mode */
    TCCR1A |= (1<<FOC1A) | (1<<FOC1B) ;

    /* setting the timer mode
     * WGM10,WGM11 are always set to 0 as we are in non-pwm mode */
   // TCCR1B = (TCCR1B & 0xE7) | (Config_Ptr -> mode);

    /* another method */
    switch(Config_Ptr -> mode)
    {

    case NORMAL:
    	CLEAR_BIT(TCCR1B,WGM12);
    	CLEAR_BIT(TCCR1B,WGM13);
    	break;

#ifdef COMPARE_MODE                    // it will be used in compare mode only.

    case CTC_OCR1A_TOP:
    	SET_BIT(TCCR1B,WGM12);
    	CLEAR_BIT(TCCR1B,WGM13);
    	break;

    case CTC_ICR1_TOP:
    	SET_BIT(TCCR1B,WGM12);
    	SET_BIT(TCCR1B,WGM13);
    	break;

#endif

    }

	/* selecting prescaler */
	TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr -> prescaler);

	/* another method:
	switch(Config_Ptr -> clock)
	{
	case NO_CLOCK:
		CLEAR_BIT(TCCR1B,CS10);
		CLEAR_BIT(TCCR1B,CS11);
		CLEAR_BIT(TCCR1B,CS12);
		break;

	case F_CPU_CLOCK:
		SET_BIT(TCCR1B,CS10);
		CLEAR_BIT(TCCR1B,CS11);
		CLEAR_BIT(TCCR1B,CS12);
		break;

	case F_CPU_8:
		CLEAR_BIT(TCCR1B,CS10);
		SET_BIT(TCCR1B,CS11);
		CLEAR_BIT(TCCR1B,CS12);
		break;

	case F_CPU_64:
		SET_BIT(TCCR1B,CS10);
		SET_BIT(TCCR1B,CS11);
		CLEAR_BIT(TCCR1B,CS12);
		break;

	case F_CPU_256:
		CLEAR_BIT(TCCR1B,CS10);
		CLEAR_BIT(TCCR1B,CS11);
		SET_BIT(TCCR1B,CS12);
		break;

	case F_CPU_1024:
		SET_BIT(TCCR1B,CS10);
		CLEAR_BIT(TCCR1B,CS11);
		SET_BIT(TCCR1B,CS12);
		break;
	}*/

	/* setting initial value to the timer/counter with zero */
	TCNT1 = (Config_Ptr -> initial_value) ;

	/* it will be used in normal mode only. */
#ifdef NORMAL_MODE

	/* enabling TOIE1 overflow match interrupt */
    TIMSK |= (1<<TOIE1);

#endif


	/* it will be used in compare mode only. */
#ifdef COMPARE_MODE
	/* setting a compare match value in OCR1A = any value , when matching this value the timer will count 1 sec */
	OCR1A = (Config_Ptr -> compare_value);

	/* enabling OCR1A compare match interrupt */
    TIMSK |= (1<<OCIE1A);
#endif

    /* Enabling i-bit */
    SREG |= (1<<7);
}

void TIMER1_deInit(void)
{
	/* clearing the timer registers */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	OCR1A = 0;

	/* disable OCR1A compare match interrupt */
	CLEAR_BIT(TIMSK,OCIE1A);

	/* disable overflow match interrupt */
	CLEAR_BIT(TIMSK,TOIE1);

	/* Reset the global pointer value */
	g_callBackPtr = NULL_PTR;
}

void TIMER1_setCallBack(void(*a_ptr)(void))
{
	g_callBackPtr = a_ptr ;
}
