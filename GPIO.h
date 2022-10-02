/*
 * File:   GPIO.h
 * Author: vrgimenez
 *
 * Created on 07 de septiembre de 2018, 10:55
 */

#ifndef _GPIO_H
#define _GPIO_H

/*Type and constant definitions---------------------------------------------------------*/

// Debug LEDs
#define LED1_DBG_IO			TRISC0
#define LED1_DBG_OUT		RC0
#define LED1_DBG_LAT		LATC0

#define LED1_ON()			LED1_DBG_OUT= 1
#define LED1_OFF()			LED1_DBG_OUT= 0
#define LED1_TOGGLE()		LED1_DBG_LAT= !LED1_DBG_LAT

#define LED2_DBG_IO			TRISC1
#define LED2_DBG_OUT		RC1
#define LED2_DBG_LAT		LATC1

#define LED2_ON()			LED2_DBG_OUT= 1
#define LED2_OFF()			LED2_DBG_OUT= 0
#define LED2_TOGGLE()		LED2_DBG_LAT= !LED2_DBG_LAT

// External Paged Parallel EEPROM Data Bus
#define DATA_BUS_IO			TRISD
#define DATA_BUS_OUT		PORTD
#define DATA_BUS_LAT		LATD

// External Paged Parallel EEPROM Address Bus
// Low Byte
#define ADDR_BUS_LO_IO		TRISA
#define ADDR_BUS_LO_OUT		PORTA
#define ADDR_BUS_LO_LAT		LATA
// High Byte
#define ADDR_BUS_HI_IO		TRISB
#define ADDR_BUS_HI_OUT		PORTB
#define ADDR_BUS_HI_LAT		LATB

// External Paged Parallel EEPROM Control Lines
// Chip Select (Chip Enable)
#define CTRL_CS_IO			TRISC2
#define CTRL_CS_OUT			RC2
#define CTRL_CS_LAT			LATC2
// Read (Output Enable)
#define CTRL_RD_IO			TRISC4
#define CTRL_RD_OUT			RC4
#define CTRL_RD_LAT			LATC4
// Write (Write Enable)
#define CTRL_WR_IO			TRISC5
#define CTRL_WR_OUT			RC5
#define CTRL_WR_LAT			LATC5

// Additional IO Macros
#define CLEAR_ANALOG_INPUTS() {\
	ADCON1bits.PCFG= 0b1111;	/*A/D Port Configuration Control bits (all digital)*/\
}
#define RESET_PORTS() {\
	PORTA= 0;\
	PORTB= 0;\
	PORTC= 0;\
	PORTD= 0;\
	PORTE= 0;\
}
#define SET_OUTPUTS() { /*Unused pins set as Outputs*/\
	/*PORTA IO Pins*/\
	ADDR_BUS_LO_IO= 0x00; /*TRISA= 0x00;*/\
	/*PORTB IO Pins*/\
	ADDR_BUS_HI_IO= 0x00; /*TRISB= 0x00;*/\
	/*PORTC IO Pins*/\
	LED1_DBG_IO= 0; /*TRISC0= 0;*/\
	LED2_DBG_IO= 0; /*TRISC1= 0;*/\
	CTRL_CS_IO= 0;  /*TRISC2= 0;*/\
	TRISC3= 0;\
	CTRL_RD_IO= 0;  /*TRISC4= 0;*/\
	CTRL_WR_IO= 0;  /*TRISC5= 0;*/\
/*	TRISC6= 0; TX_DBG*/\
/*	TRISC7= 0; RX_DBG*/\
	/*PORTD IO Pins*/\
	DATA_BUS_IO= 0x00; /*TRISD= 0x00;*/\
	/*PORTE IO Pins*/\
	TRISE= 0;\
}
#define SET_INPUTS() {\
}

/*External object declarations----------------------------------------------------------*/

/*External function declarations--------------------------------------------------------*/

#endif	//#ifndef _GPIO_H
