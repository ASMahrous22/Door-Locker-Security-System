 /******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: i2c.h
 *
 * Description: Header file for the TWI(I2C) AVR driver
 *
 * Author: AS.Mahrous
 *
 *******************************************************************************/ 

#ifndef I2C_H_
#define I2C_H_

#include "../std_types.h"

/*******************************************************************************
*                           Type Declarations                                  *
*******************************************************************************/
typedef uint8 TWI_Address;

typedef uint16 TWI_BaudRate;

typedef struct
{
 TWI_Address address;
 TWI_BaudRate bit_rate;
}TWI_ConfigType;

/*******************************************************************************
*                                Definitions                                   *
*******************************************************************************/
/* I2C bit rate can be calculated using the formula:
 * 400K Bits/Sec = 400,000 bits/sec.
 * Bit Rate = F_CPU / (16 + (2 * TWBR * Prescaler))
 * Prescaler is 1 for standard mode, where the fastest possible bit rate is achieved.
 * TWBR = (F_CPU / (4 * Bit Rate)) - 16
 * For an 8MHz clock, the TWBR value for 400K Bits/Sec:
 * TWBR = (8,000,000 / (4 * 400,000)) - 16 = 49
 * */
#define I2C_BAUD_RATE_400K 0x02
#define I2C_BAUD_RATE_100K 0x10

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void TWI_init(const TWI_ConfigType *Config_Ptr);
void TWI_start(void);
void TWI_stop(void);
void TWI_writeByte(uint8 data);
uint8 TWI_readByteWithACK(void);
uint8 TWI_readByteWithNACK(void);
uint8 TWI_getStatus(void);


#endif /* TWI_H_ */
