/*Includes of system headers--------------------------------------------------------------*/
#include <htc.h>
#include <stdio.h>
#include <stdint.h>

/*Includes of local headers---------------------------------------------------------------*/
#include "PIC-EEPROM_PRG.h"
#include "HW_UART.h"
#include "NVM_Int.h"
#include "ATCommSet.h"
#include "GPIO.h"
#include "E2R_ExtParallel.h"
#include "ROM-BEER19_32K.h"
#include "version.h"

/*Type and constant definitions-----------------------------------------------------------*/

/* Configuration Bits */
__CONFIG(1, IESODIS & FCMDIS & OSC_INTIO67);
__CONFIG(2, BOREN & BORV43 & PWRTEN & WDTPS64 & WDTEN);
__CONFIG(3, MCLRDIS & LPT1DIS & PBDIGITAL & CCP2RC1);
__CONFIG(4, DEBUGDIS & XINSTDIS & LVPDIS & STVREN);
__CONFIG(5, CPA & CPB);
__CONFIG(6, WPB);
__CONFIG(7, TRPB);

/*Global variables------------------------------------------------------------------------*/
T_SERIAL_N2	tSerialN;
T_BIT_SET	tBitSet;
T_BIT_FLAGS	tBitFlags;
T_RTC		tRTC;
uint8_t		c2ms, c10ms, c100ms, c1000ms;
uint8_t		tmp;

/*Functions-------------------------------------------------------------------------------*/
void main (void)
{
	//Set Osc, PWM, Ports, Comparator, Tic and enable ISRs
	APP_Init();

	//Initializes Serial Debug Console
	Console_Init();

	//Initializes External Paged Parallel EEPROM Memory
	E2RExt_Init();

	tAddress.value= 0;
	E2RPage= 0;
	tmp= 0;

	//Infinite Loop
	while(1)
	{
		if(tBitFlags.E2RTrigger)
		{
			tBitFlags.E2RTrigger= 0;

			if(tBitFlags.E2RWrite)
			{
				switch(tmp)
				{
					case 0:
						printf("Write Page %u with ",E2RPage);
						printf("%u errors/bytes left\r\n",E2RExt_WritePage(tAddress,&ROM_BEER19_32K[tAddress.value],E2REXT_PAGE_SIZE));
						tmp++;
						break;
					case 1:
						printf("Check Page %u with ",E2RPage);
						printf("%u errors/bytes left\r\n",E2RExt_CheckPage(tAddress,&ROM_BEER19_32K[tAddress.value],E2REXT_PAGE_SIZE));
	
						if((tAddress.value+= E2REXT_PAGE_SIZE) >= E2REXT_SIZE)
							tAddress.value= 0;
	
						if(++E2RPage >= 512)
						{
							printf("EEPROM Write Finished\r\n");
							tBitFlags.E2RWrite= 0;
							E2RPage= 0;
						}
						tmp--;
						break;
				}
			}

			if(tBitFlags.E2RCheck)
			{
				printf("Check Page 0x%03X of 0x1FF\r\n",E2RPage);
				if(tmp = E2RExt_CheckPage(tAddress,&ROM_BEER19_32K[tAddress.value],E2REXT_PAGE_SIZE)) {
					printf("Error: 0x%02X\r\nEEPROM Write Verify FAIL!\r\n",tmp);
					tBitFlags.E2RCheck= 0;
					E2RPage= 0;
				}

				if((tAddress.value+= E2REXT_PAGE_SIZE) >= E2REXT_SIZE)
					tAddress.value= 0;

				if(++E2RPage >= 512)
				{
					printf("EEPROM Write Verify OK!\r\n");
					tBitFlags.E2RCheck= 0;
					E2RPage= 0;
				}
			}
		}

		if(tBitFlags.RTCTrigger)
		{
			tBitFlags.RTCTrigger= 0;

		/*	printf("%04X=R%02X\r\n",tAddress.value,E2RExt_ReadByte(tAddress));

			if(++tAddress.value >= E2REXT_SIZE)
				tAddress.value= 0;*/

		/*	//Data Bus Test
			DATA_BUS_IO= 0x00;	//Data Bus (Output)
			PORTD= 0xFF;		//d0:d7=1

			//Address Bus Test
			PORTA= 0xFF;	//a0:a7=1
			PORTB= 0xFF;	//a8:a14=1

			//Control Lines Test
			WR_DIS();
			CS_DIS();
			RD_DIS();*/

			if(++tRTC.Seconds == 60)
			{
				tRTC.Seconds = 0;
				if(++tRTC.Minutes == 60)
				{
					tRTC.Minutes = 0;
					if(++tRTC.Hours == 24)
					{
						tRTC.Hours = 0;
					}
				}
			}
		//	printf("\r\n%02u:%02u:%02u", tRTC.Hours, tRTC.Minutes, tRTC.Seconds);
		//	RTC();
		}

			#ifdef ENABLE_DBG_CONSOLE
		ATCommandParser();
			#endif

		//Clear the watchdog timer
		CLRWDT();
	}
}

void APP_Init(void)
{
	//Configure Oscilator
		#if defined _18F4620
	OSCCONbits.IRCF= 0b111;		//8 MHz (INTOSC drives clock directly)
	OSCCONbits.SCS= 0b00;		//Primary Oscillator
	OSCTUNEbits.PLLEN= 1;		//PLL enabled for INTOSC (4 MHz and 8 MHz only)
		#elif defined _16F18346
	OSCCON1bits.NDIV= 0b0000;	//Clock Divider x1
	OSCCON1bits.NOSC= 0b110;	//HFINTOSC (1 MHz)
	OSCFRQbits.HFFRQ= 0b0111;	//HFFRQ 32_MHz (NOSC = 110)
		#elif defined _16F1579
	OSCCONbits.SCS= 0b10;		//Internal oscillator block
	OSCCONbits.IRCF= 0b1110;	//8MHz or 32 MHz HF
	OSCCONbits.SPLLEN= 0;		//4x PLL is disabled
		#endif

	//Configure Timer 0
		#if defined _18F4620
	T0CONbits.TMR0ON= 0;		//TMR0 Stops
	T0CONbits.T0PS= 0b011;		//TMR0 1:16 Prescale value
	T0CONbits.PSA= 0;			//TMR0 Prescaler is Assigned
	T0CONbits.T0CS= 0;			//TMR0 Clock Source Internal instruction cycle clock (CLKO)
	T0CONbits.T08BIT= 1;		//TMR0 Configured as an 8-bit timer/counter
	T0CONbits.TMR0ON= 1;		//TMR0 Enables
	TMR0L= TMR0_PERIOD;			//TMR0 Period Register (When T016BIT=0)
		#elif defined _16F18346
	T0CON1bits.T0CKPS= 0b0100;	//TMR0 Prescaler Rate 1:16
	T0CON1bits.T0CS= 0b010;		//TMR0 Clock Source FOSC/4
	T0CON0bits.T0OUTPS= 0b0000;	//TMR0 Output Postscaler 1:1
	T0CON0bits.T016BIT= 0;		//TMR0 Operating as 8-bit timer
	T0CON0bits.T0EN= 1;			//TMR0 Enabled
	TMR0H= TMR0_PERIOD;			//TMR0 Period Register (When T016BIT=0)
	TMR0L= 0;					//TMR0 Count Register
		#elif defined _16F1579
	OPTION_REGbits.T0CS= 0;		//Internal instruction cycle clock (CLKO)
	OPTION_REGbits.PSA= 0;		//Prescaler is assigned to the Timer0 module
	OPTION_REGbits.PS= 0b001;	//Prescaler Rate 1:4
	TMR0= TMR0_PERIOD;			//System Tic (500us; 1000 instructions/tic @ 8MHz[2 MIPS])
		#endif

	//Configure I/O Ports (I/O pins set as input on POR/BOR)
	CLEAR_ANALOG_INPUTS();
//	SET_ANALOG_INPUTS();
	RESET_PORTS();
	SET_OUTPUTS();
//	SET_INPUTS();

	//Enable Interrupts
	TMR0IE= 1;
	PEIE= 1;
	GIE= 1;
}

void interrupt ISRs(void)
{
	if(TMR0IE && TMR0IF)			//each 500us
	{
			#if defined _18F4620
		TMR0IF= 0;
		TMR0= TMR0_PERIOD;
			#elif defined _16F1579
		TMR0= TMR0_PERIOD;
			#endif

		if(++c2ms >= (2*FTICK))		//each 2ms
		{
			c2ms= 0;

			if(++c10ms >= (10/2))	//each 10ms
			{
				c10ms= 0;

				if(++c100ms >= (100/10))	//each 100ms
				{
					c100ms= 0;

					tBitFlags.E2RTrigger= 1;
					LED1_TOGGLE();

					if(++c1000ms >= (1000/100))		//each 1000ms
					{
						c1000ms= 0;

						LED2_TOGGLE();

						tBitFlags.RTCTrigger= 1;
					}
				}
			}
		}

		TMR0IF= 0;
	}

		#ifdef ENABLE_DBG_CONSOLE
	DBG_PORT_ISR();
		#endif
}
