/*Includes of system headers------------------------------------------------------------*/
#include <htc.h>
#include <stdio.h>
#include <stdint.h>

/*Includes of local headers-------------------------------------------------------------*/
#include "E2R_DataMemory.h"
#include "PIC-EEPROM_PRG.h"

/*Type and constant definitions---------------------------------------------------------*/

//EEPROM Initialization
__EEPROM_DATA(0x56,0x34,0x12,0xBC,0x9A,0x78,0x01,0x00);

/*Global variables----------------------------------------------------------------------*/
uint8_t i,u8E2Rtmp[BLOCK_SIZE];

/*Functions-----------------------------------------------------------------------------*/
void E2R_Init(void)
{
	E2R_ReadSerialNumber();
	E2R_ReadBitSets();
}

void E2R_ReadBlock(uint16_t u16Address, uint8_t *u8pData, uint8_t u8Size)
{
	for(i = 0; i < u8Size; i++)
	{
		u8pData[i]= eeprom_read(u16Address + i);
		CLRWDT();
	}
}

//Rutina optimizada, si el valor a guardar ya existe no se graba y gana tiempo
void E2R_WriteBlock(uint16_t u16Address, uint8_t *u8pData, uint8_t u8Size)
{
	uint8_t cDummy;

	for(i = 0; i < u8Size; i++)
	{
		do
		{
			if(u8pData[i] != eeprom_read(u16Address + i)) //Si el dato a guardar es = al guardado no graba en eeprom (salva tiempo)
			{
				eeprom_write(u16Address + i, u8pData[i]);
				cDummy++;
				CLRWDT();
			}
		}
		//Write verify, keep trying 3 times
		while( (u8pData[i] != eeprom_read(u16Address + i)) &&
			   (cDummy <= 3) );
		cDummy= 0;
	}
}

void E2R_ReadString(uint16_t u16Address, uint8_t *str)
{
	do
	{
		str[i]= eeprom_read(u16Address + i);
		CLRWDT();
	}
	while (str[i++] && i < STRING_SIZE);
}

void E2R_WriteString(uint16_t u16Address, uint8_t *str)
{
	do
	{
		eeprom_write(u16Address + i, str[i]);
		CLRWDT();
	}
	while (str[i++] && i < STRING_SIZE);
}

void E2R_ReadSerialNumber(void)
{
	E2R_ReadBlock(SERIAL_NUMBER_ADDRESS, u8E2Rtmp, SERIAL_NUMBER_SIZE);
	tSerialN.tSNA.LB = u8E2Rtmp[0];
	tSerialN.tSNA.MB = u8E2Rtmp[1];
	tSerialN.tSNA.HB = u8E2Rtmp[2];
	tSerialN.tSNB.LB = u8E2Rtmp[3];
	tSerialN.tSNB.MB = u8E2Rtmp[4];
	tSerialN.tSNB.HB = u8E2Rtmp[5];
}

void E2R_WriteSerialNumber(void)
{
	u8E2Rtmp[0]= tSerialN.tSNA.LB;
	u8E2Rtmp[1]= tSerialN.tSNA.MB;
	u8E2Rtmp[2]= tSerialN.tSNA.HB;
	u8E2Rtmp[3]= tSerialN.tSNB.LB;
	u8E2Rtmp[4]= tSerialN.tSNB.MB;
	u8E2Rtmp[5]= tSerialN.tSNB.HB;
	E2R_WriteBlock(SERIAL_NUMBER_ADDRESS, u8E2Rtmp, SERIAL_NUMBER_SIZE);
}

void E2R_ReadBitSets(void)
{
	E2R_ReadBlock (BIT_SET_ADDRESS, (uint8_t*) &tBitSet.data, BIT_SET_SIZE);
}

void E2R_WriteBitSets(void)
{
	E2R_WriteBlock(BIT_SET_ADDRESS, (uint8_t*) &tBitSet.data, BIT_SET_SIZE);
}
