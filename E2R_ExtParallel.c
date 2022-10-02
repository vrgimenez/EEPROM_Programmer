/*Includes of system headers------------------------------------------------------------*/
#include <htc.h>
#include <stdio.h>
#include <stdint.h>

/*Includes of local headers-------------------------------------------------------------*/
#include "E2R_ExtParallel.h"
#include "GPIO.h"
#include "PIC-EEPROM_PRG.h"

/*Type and constant definitions---------------------------------------------------------*/

/*Global variables----------------------------------------------------------------------*/
uint8_t	i,u8E2RExtPage[E2REXT_PAGE_SIZE];
T_ADDR	tAddress;

/*Functions-----------------------------------------------------------------------------*/
void E2RExt_Init(void)
{
	CS_DIS();
	RD_DIS();
	WR_DIS();
}

uint8_t E2RExt_ReadByte(T_ADDR tAddr)
{
	uint8_t byte;

	//Set Address
	PORTA= tAddr.LB;
	PORTB= tAddr.HB;

	//Set Control Lines
	WR_DIS();	//125ns
	CS_ENA();	//125ns
	RD_ENA();	//125ns

	//tACC (worst case) 350ns
	//tCE  (worst case) 350ns
	//tOE  (worst case) 100ns
	//Tcy (@32MHz) 125ns

	//Set Data Bus Direction
	DATA_BUS_IO= 0xFF;	//125ns

	//Read Data
	byte= PORTD;

	//Reset Control Lines
	RD_DIS();
	CS_DIS();

	return byte;
}

void E2RExt_WriteByte(T_ADDR tAddr, uint8_t byte)
{
	//Set Address
	PORTA= tAddr.LB;
	PORTB= tAddr.HB;

	//Set Data Bus Direction
	DATA_BUS_IO= 0x00;	//125ns

	//Set Data
	PORTD= byte;

	//Set Control Lines
	RD_DIS();	//125ns
	CS_ENA();	//125ns
	WR_ENA();	//125ns

	//tAH (min) 50ns
	//tWP (min) 100ns
	//tDS (min) 50ns
	//Tcy (@32MHz) 125ns
	NOP();	//125ns

	//Reset Control Lines
	WR_DIS();	//125ns
	CS_DIS();
}

void E2RExt_ReadPage(uint16_t u16Address, uint8_t *u8pData, uint8_t u8Size)
{
	for(i = 0; i < u8Size; i++)
	{
		u8pData[i]= eeprom_read(u16Address + i);
		CLRWDT();
	}
}

void E2RExt_WritePage(uint16_t u16Address, uint8_t *u8pData, uint8_t u8Size)
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

void E2RExt_ReadString(uint16_t u16Address, uint8_t *str)
{
	do
	{
		str[i]= eeprom_read(u16Address + i);
		CLRWDT();
	}
	while (str[i++] && i < E2REXT_PAGE_SIZE);
}

void E2RExt_WriteString(uint16_t u16Address, uint8_t *str)
{
	do
	{
		eeprom_write(u16Address + i, str[i]);
		CLRWDT();
	}
	while (str[i++] && i < E2REXT_PAGE_SIZE);
}
