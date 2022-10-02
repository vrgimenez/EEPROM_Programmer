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

#define LED1_ON()			LED1_DBG_OUT= 0
#define LED1_OFF()			LED1_DBG_OUT= 1
#define LED1_TOGGLE()		LED1_DBG_LAT= !LED1_DBG_LAT

#define LED2_DBG_IO			TRISC1
#define LED2_DBG_OUT		RC1
#define LED2_DBG_LAT		LATC1

#define LED2_ON()			LED2_DBG_OUT= 0
#define LED2_OFF()			LED2_DBG_OUT= 1
#define LED2_TOGGLE()		LED2_DBG_LAT= !LED2_DBG_LAT

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
	LATA= 0;\
	LATB= 0;\
	LATC= 0;\
	LATD= 0;\
	LATE= 0;\
}
#define SET_OUTPUTS() { /*Unused pins set as Outputs*/\
	/*PORTA IO Pins*/\
	TRISA= 0;\
	/*PORTB IO Pins*/\
	TRISB= 0;\
	/*PORTC IO Pins*/\
	LED1_DBG_IO= 0; /*TRISC0= 0;*/\
	LED2_DBG_IO= 0; /*TRISC1= 0;*/\
	TRISC2= 0;\
	TRISC3= 0;\
	TRISC4= 0;\
	TRISC5= 0;\
/*	TRISC6= 0; TX_DBG*/\
/*	TRISC7= 0; RX_DBG*/\
	/*PORTD IO Pins*/\
	TRISD= 0;\
	/*PORTE IO Pins*/\
	TRISE= 0;\
}
#define SET_INPUTS() {\
	PANIC_BTN_IN_IO= 1;\
	REAR_PAX_IN_IO= 1;\
	FRONT_PAX_IN_IO= 1;\
	ODOMETER_IN_IO= 1;\
}

/*External object declarations----------------------------------------------------------*/

/*External function declarations--------------------------------------------------------*/

#endif	//#ifndef _GPIO_H
