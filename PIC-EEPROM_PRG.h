#ifndef _PIC_EEPROM_PRG_H
#define _PIC_EEPROM_PRG_H

/*Includes of local headers-------------------------------------------------------------*/
#include "DevelopmentDefines.h"

/*Type and constant definitions---------------------------------------------------------*/
typedef unsigned short long uint24_t;

//Oscilator Defines
#define MHZ					*1000UL
#define KHZ					*1
#define FOSC				32MHZ
#define FCY					FOSC/4
#define FTICK				2KHZ							//500us system tic
#define PRES				16
	#if defined _18F4620
#define TMR0_PERIOD			~(((FCY/PRES)/FTICK) - 1)
	#elif defined _16F18346
#define TMR0_PERIOD			(((FOSC>>6)/FTICK) - 1)
	#elif defined _16F1579
#define TMR0_PERIOD			~(((FOSC>>4)/FTICK) - 6)		//(*)
	#endif

/*
 	(*) se pierden 4 Tcyc en determinar el origen de la excepción:
		46:                	if(TMR0IE&&TMR0IF)				//each 500us
		   00F    1A8B     BTFSC 0xb, 0x5
		   010    1D0B     BTFSS 0xb, 0x2
		   011    28AA     GOTO 0xaa

		y además se pierden 2 Tcyc cada vez que se recarga el TMR0:
		"If the TMR0 register is written, the increment is inhibited for the following
		 two instruction cycles."
*/

//Real Time Clock
typedef struct {
	uint8_t		Seconds;
	uint8_t		Minutes;
	uint8_t		Hours;
} T_RTC;

//Serial Number
typedef union {
	uint24_t value;
	struct {
		uint8_t LB;
		uint8_t MB;
		uint8_t HB;
	};
} T_SERIAL_N;

typedef union {
	struct {
		uint16_t LW;
		uint16_t MW;
		uint16_t HW;
	};
	struct {
		T_SERIAL_N tSNB;
		T_SERIAL_N tSNA;
	};
} T_SERIAL_N2;

//Bit Settings
typedef union {
	uint16_t data;
	struct {
		unsigned RemisMode		: 1;	//0:TaxiMode, 1:RemisMode
		unsigned				: 7;
		unsigned				: 8;
	};
} T_BIT_SET;

//Bit Flags
typedef union {
	uint8_t data;
	struct {
		unsigned RTCTrigger		: 1;	//RTC Engine Trigger
		unsigned E2RTrigger		: 1;	//EEPROM Engine Trigger
		unsigned E2RWrite		: 1;	//EEPROM Write
		unsigned E2RCheck		: 1;	//EEPROM Write Verify
	};
} T_BIT_FLAGS;

/*External object declarations----------------------------------------------------------*/
extern T_SERIAL_N2	tSerialN;
extern T_BIT_SET	tBitSet;
extern T_BIT_FLAGS	tBitFlags;
extern T_RTC		tRTC;
extern uint8_t		c2ms, c10ms, c100ms, c1000ms;

/*External function declarations--------------------------------------------------------*/
void APP_Init(void);

#endif	//#ifndef _PIC_EEPROM_PRG_H
