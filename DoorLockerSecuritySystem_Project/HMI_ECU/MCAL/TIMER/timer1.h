/*
 * timer.h
 *
 *  Created on: ١٢‏/٠٣‏/٢٠٢٤
 *  Author: AS.Mahrous
 */

#ifndef MCAL_TIMER_TIMER1_H_
#define MCAL_TIMER_TIMER1_H_

#include "../std_types.h"

/*******************************************************************************
 *                             Definitions                                     *
 *******************************************************************************/
/* to adjust the working timer mode, its either COMPARE_MODE or NORMAL_MODE  */
#define COMPARE_MODE

/*******************************************************************************
*                           Type Declarations                                  *
*******************************************************************************/
typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}TIMER1_Prescaler;

typedef enum
{
	NORMAL,
#ifdef COMPARE_MODE
	CTC_OCR1A_TOP,CTC_ICR1_TOP = 3
#endif
}TIMER1_Mode;

typedef struct
{
	 uint16 initial_value;
#ifdef COMPARE_MODE
	 uint16 compare_value;           /* it will be used in compare mode only. */
#endif
	 TIMER1_Prescaler prescaler;
	 TIMER1_Mode mode;
} TIMER1_configType;

TIMER1_configType TIMER1_settings;
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * Function responsible for the Initializing of Timer1
 */
void TIMER1_init(const TIMER1_configType * Config_Ptr);

/*
 * Description :
 * Function responsible for the de-initializing "disabling" of Timer1
 */
void TIMER1_deInit(void);

/*
 * Description :
 * Function responsible for setting the Address of the Call Back Function
 */
void TIMER1_setCallBack(void(*a_ptr)(void));

#endif /* MCAL_TIMER_TIMER_H_ */
