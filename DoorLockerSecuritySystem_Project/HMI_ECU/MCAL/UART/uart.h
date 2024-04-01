/*
 * uart.h
 *
 *  Created on: ١٢‏/٠٣‏/٢٠٢٤
 *  Author: AS.Mahrous
 */

#ifndef MCAL_UART_UART_H_
#define MCAL_UART_UART_H_

#include "../std_types.h"

/*******************************************************************************
 *                             Definitions                                     *
 *******************************************************************************/
#define UART_BAUD_RATE 9600

/*******************************************************************************
 *                          Type Declarations                                  *
 *******************************************************************************/
typedef uint32 UART_BaudRate;

typedef enum
{
	FIVE_BIT,SIX_BIT,SEVEN_BIT,EIGHT_BIT,NINE_BIT = 7
}UART_BitData;

typedef enum
{
	DISABLED,EVEN_PARITY = 2,ODD_PARITY
}UART_Parity;

typedef enum
{
	ONE_STOP_BIT,TWO_STOP_BITS
}UART_StopBits;

typedef struct
{
	UART_BitData bit_data;
	UART_Parity parity;
	UART_StopBits stop_bits;
	UART_BaudRate baud_rate;
}UART_configType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_configType *Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* MCAL_UART_UART_H_ */
