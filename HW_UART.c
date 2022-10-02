/****************************************************************************************
 *										VRX Design										*
 ****************************************************************************************
 *
 *	Description: VRX Hardware UART Driver.
 *				 Implements Software IO FIFO 2^N Buffer.
 *
 *	Author: Ing. Vektor Raúl Ximen © 2011	[vrgimenez@gmail.com]
 *
 ****************************************************************************************/

/*Includes of system headers------------------------------------------------------------*/
#include <htc.h>
#include <stdio.h>
#include <stdint.h>

/*Includes of local headers-------------------------------------------------------------*/
#include "PIC-EEPROM_PRG.h"
#include "HW_UART.h"

/*Type and constant definitions---------------------------------------------------------*/

/*Global variables----------------------------------------------------------------------*/

//DBG UART FIFO
UART_FIFO	DBG;
uint8_t		DBGInQueue[QUEUE_SIZE];
uint8_t		DBGOutQueue[QUEUE_SIZE];
uint8_t		UART_RX_Enabled;

/*Functions-----------------------------------------------------------------------------*/
void Console_Init(void)
{
		#ifdef ENABLE_DBG_CONSOLE
	//Initializes EUSART
	UART_Init();
	Queues_Init();

	//Show Product Version
	printf("\r\n" PRODUCT_FULL_VERSION_STR);
	printf("\r\n" LEGAL_TRADEMARKS_STR);
	printf("\r\nBuild: " __DATE__ ", " __TIME__);
	printf("\r\nHI-TECH PICC-18 v%u.%u %u\r\n",_HTC_VER_MAJOR_,_HTC_VER_MINOR_,_HTC_VER_PATCH_);
	printf("READY\r\n");

	//Wait text output
//	while(DBG.Out.Head != DBG.Out.Tail);
		#endif
}

void UART_Init(void)
{
	SET_UART_PORTS();

	TXSTAbits.TXEN= 1;				//Transmit Enabled (TXIF=1)
	TXSTAbits.BRGH= 1;				//High Baud Rate
	RCSTAbits.SPEN= 1;				//Serial Port Enable
	RCSTAbits.CREN= 1;				//Enables Receiver
	BAUDCONbits.BRG16= 1;			//16-bit Baud Rate Generator
	SPBRGH= (uint8_t)(_spbrg>>8);	//Set BRG
	SPBRG=  (uint8_t)(_spbrg);
	PIE1bits.RCIE= 1;				//Interrupt Enable
}

void Queues_Init(void)
{
	// Initializes DBG In/Out Queues
	DBG.In.Queue=  DBGInQueue;
	DBG.Out.Queue= DBGOutQueue;
}

uint8_t getch(void)
{
	uint8_t byte;

	if(RCSTAbits.OERR)
	{
		RCSTAbits.CREN= 0;
		RCSTAbits.CREN= 1;
	}

	if (DBG.In.Head == DBG.In.Tail)
		return -1;

	GIE= 0;
	byte= DBG.In.Queue[DBG.In.Tail++];
	DBG.In.Tail&= QUEUE_MASK;
	GIE= 1;

	return byte;
}

void putch(uint8_t byte)
{
	if( ((DBG.Out.Head+1) & QUEUE_MASK) != DBG.Out.Tail )
	{
		GIE= 0;
		DBG.Out.Queue[DBG.Out.Head++]= byte;
		DBG.Out.Head&= QUEUE_MASK;
		TXIE= 1;
		GIE= 1;
	}
}

void putstr(const uint8_t *str)
{
	while(str && *str)
	{
		putch(*str++);
	}
}
