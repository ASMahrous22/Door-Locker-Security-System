 /******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: eeprom.h
 *
 * Description: Header file for the External EEPROM Memory
 *
 * Author: AS.Mahrous
 *
 *******************************************************************************/


#ifndef EEPROM_H_
#define EEPROM_H_

#include "../../MCAL/std_types.h"

/*******************************************************************************
 *                              Definitions                                    *
 *******************************************************************************/
#define ERROR 0
#define SUCCESS 1

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

uint8 EEPROM_writeByte(uint16 u16addr,uint8 u8data);
uint8 EEPROM_readByte(uint16 u16addr,uint8 *u8data);
 
#endif /* EXTERNAL_EEPROM_H_ */
