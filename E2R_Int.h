/*
 * File:   E2R_Int.h
 * Author: vrgimenez
 *
 * Created on 2 de septiembre de 2020, 13:11
 */

#ifndef _E2R_INT_H
#define	_E2R_INT_H

/*Type and constant definitions-----------------------------------------------*/

//EEPROM Value Sizes
#define BLOCK_SIZE				(6)
#define STRING_SIZE				(32)
#define SERIAL_NUMBER_SIZE		(6)
#define BIT_SET_SIZE			(2)

//EEPROM Memory Map
#define BASE_ADDRESS			(0)
#define SERIAL_NUMBER_ADDRESS	(BASE_ADDRESS + 0)								//0x0000
#define BIT_SET_ADDRESS			(SERIAL_NUMBER_ADDRESS + SERIAL_NUMBER_SIZE)	//0x0006

/*External object declarations------------------------------------------------*/

/*External function declarations----------------------------------------------*/
void E2R_Init(void);
void E2R_ReadBlock  (uint16_t u16Address, uint8_t *u8pData, uint8_t u8Size);
void E2R_WriteBlock (uint16_t u16Address, uint8_t *u8pData, uint8_t u8Size);
void E2R_ReadString (uint16_t u16Address, uint8_t *str);
void E2R_WriteString(uint16_t u16Address, uint8_t *str);
void E2R_ReadSerialNumber(void);
void E2R_WriteSerialNumber(void);
void E2R_ReadBitSets(void);
void E2R_WriteBitSets(void);

#endif	/* _E2R_INT_H */
