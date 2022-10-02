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
uint8_t		i;
uint16_t	E2RPage;
T_ADDR		tAddress;

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

	//Set Data Bus Direction
	DATA_BUS_IO= 0xFF;	//125ns

	//Set Control Lines
	WR_DIS();	//125ns
	CS_ENA();	//125ns
	RD_ENA();	//125ns

	//tACC (worst case) 150ns
	//tCE  (worst case) 150ns
	//tOE  (worst case) 70ns
	//Tcy (@32MHz) 125ns
	NOP();		//125ns

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
	NOP();		//125ns

	//Reset Control Lines
	WR_DIS();	//125ns
	CS_DIS();
}

uint8_t E2RExt_WritePage(T_ADDR tAddr, const uint8_t *u8pData, uint8_t u8Size)
{
	//Set Address
	PORTB= tAddr.HB;

	//Set Data Bus Direction
	DATA_BUS_IO= 0x00;	//125ns

	for(uint8_t offset = tAddr.value & 0x003F; (offset < 0x40) && u8Size; offset++)
	{
		//Set Address
		PORTA= (tAddr.LB & 0xC0) + offset;
	//	printf("0x%02X%02X+%02X=%02X\r\n",tAddr.HB,tAddr.LB&0xC0,offset,*u8pData++);

		//Set Data
		PORTD= *u8pData++;

		//Set Control Lines
		RD_DIS();	//125ns
		CS_ENA();	//125ns
		WR_ENA();	//125ns

		//tAH (min) 50ns
		//tWP (min) 100ns
		//tDS (min) 50ns
		//Tcy (@32MHz) 125ns
		u8Size--;	//125ns

		//Reset Control Lines
		WR_DIS();	//125ns
	}
	CS_DIS();

	return u8Size;
}

uint8_t E2RExt_CheckPage(T_ADDR tAddr, const uint8_t *u8pData, uint8_t u8Size)
{
	//Set Address
	PORTB= tAddr.HB;

	//Set Data Bus Direction
	DATA_BUS_IO= 0xFF;	//125ns

	for(uint8_t offset = tAddr.value & 0x003F; (offset < 0x40) && u8Size; offset++)
	{
		//Set Address
		PORTA= (tAddr.LB & 0xC0) + offset;
	//	printf("0x%02X%02X+%02X=%02X\r\n",tAddr.HB,tAddr.LB&0xC0,offset,*u8pData++);

		//Set Control Lines
		WR_DIS();	//125ns
		CS_ENA();	//125ns
		RD_ENA();	//125ns
	
		//tACC (worst case) 150ns
		//tCE  (worst case) 150ns
		//tOE  (worst case) 70ns
		//Tcy (@32MHz) 125ns
		u8Size--;	//125ns
	
		//Check Data
		if(*u8pData++ != PORTD)
		{
			u8Size= 0xFF;
			break;
		}
	}
	//Reset Control Lines
	RD_DIS();
	CS_DIS();

	return u8Size;
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
