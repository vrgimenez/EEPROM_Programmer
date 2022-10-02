/****************************************************************************************
 *										VRX Design										*
 ****************************************************************************************
 *
 *	Description: VRX AT Command Parser.
 *
 *	Author: Ing. Vektor Raúl Ximen © 2008	[vrgimenez@gmail.com]
 *
 ****************************************************************************************/

/*Includes of system headers------------------------------------------------------------*/
#include <htc.h>
#include <stdio.h>
#include <stdint.h>

/*Includes of local headers-------------------------------------------------------------*/
#include "NVM_DataMemory.h"
#include "PIC-EEPROM_PRG.h"
#include "ATCommSet.h"
#include "HW_UART.h"
#include "version.h"
#include "Str.h"

/*Type and constant definitions---------------------------------------------------------*/
#define HDR_SIZE		(4)					//[1ByteLenght]+[2BytesAddress]+[1ByteRecdType]
#define MAX_HEX_ROW		(HDR_SIZE+32+1)		//[Lenght]+[Address]+[RecdType]+[Data]+[Checksum]

#define	N_AT			(7)
const char
		ATSN[]=		"AT+SN",
		ATRST[]=	"AT+RST",
		ATVER[]=	"AT+VER",
		ATDEV[]=	"AT+DEV",
		ATODOMV[]=	"AT+ODOMV",				//Odometer Value
		ATHEX[]=	"AT+HEX",				//Read/Write Intel HEX Data Row
		ATEXIT[]=	"AT+EXIT";				//Exit AT Command Mode

enum{
		_ATSN_,
		_ATRST_,
		_ATVER_,
		_ATDEV_,
		_ATODOMV_,
		_ATHEX_,
		_ATEXIT_
	};

/*Global variables----------------------------------------------------------------------*/
uint8_t c, c_prv='\r';
uint8_t cHEXRow[MAX_HEX_ROW], cHEXcnt, cHEXchk;

/*Functions-----------------------------------------------------------------------------*/
void ATCommandParser(void)
{
#if defined ENABLE_DBG_CONSOLE || defined ENABLE_485_CONSOLE

	static uint8_t
		step,
		cmd,
		cRecycle,	//Flag para indicar que al recibir \r o \n debe volver al ciclo
		ctmp1,		//va almacenando los caracteres entrantes, o marca un comando de escritura aceptado
		ctmp2,		//va contando los caracteres (parámetros) entrantes, o los marca como inválidos
		ctmp3,		//almacena el subcomando para el comando "AT+HEX"
		idx[N_AT];
	static uint16_t
		itmp;
	static uint32_t
		ltmp,
		ltmp1;
	enum {
		PARSE_CMD,
		PARSE_TYPE,
		READ_COMMAND,
		TESTWRITE_COMMAND,
		WRITE_COMMAND,
		TEST_COMMAND,
		ERROR
	};

	if((c= getch()) != 255)
	{
		putch(c);	//Local Echo

		if( (c >= 'a') && (c <= 'z') )	//Converts to upper case
		{
			c -= ('a' - 'A');
		}

		switch(step)
		{
			case PARSE_CMD:
				if(cmd != 0xFF) cmd= 0xFF;

				if(ATRecdStr(ATSN,&idx[_ATSN_]) == 1)
				{
					cmd= _ATSN_;
					step++;
				}
				if(ATRecdStr(ATRST,&idx[_ATRST_]) == 1)
				{
					cmd= _ATRST_;
					step++;
				}
				if(ATRecdStr(ATVER,&idx[_ATVER_]) == 1)
				{
					cmd= _ATVER_;
					step++;
				}
				if(ATRecdStr(ATDEV,&idx[_ATDEV_]) == 1)
				{
					cmd= _ATDEV_;
					step++;
				}
				if(ATRecdStr(ATODOMV,&idx[_ATODOMV_]) == 1)
				{
					cmd= _ATODOMV_;
					step++;
				}
				if(ATRecdStr(ATHEX,&idx[_ATHEX_]) == 1)
				{
					cmd= _ATHEX_;
					step++;
				}
				if(ATRecdStr(ATEXIT,&idx[_ATEXIT_]) == 1)
				{
					cmd= _ATEXIT_;
					step++;
				}
				break;
			case PARSE_TYPE:
				if(c == '=')
					step= TESTWRITE_COMMAND;
				else
				if(c == '?')
					step= READ_COMMAND;
				else
				{
					cmd = 0xFF;
					step= ERROR;
				}
				break;
			case READ_COMMAND:
				if(c == '\r' || c == '\n')
				{
					switch(cmd)
					{
						case _ATSN_:
							printf("\r\n+SN: ");
							printf("%04X",tSerialN.HW);
							printf("%04X",tSerialN.MW);
							printf("%04X",tSerialN.LW);
							printf("\r\n");
							break;
						case _ATVER_:
							printf("\r\n+VER: " PRODUCT_FULL_VERSION_STR " | " COMPANY_NAME_STR "\r\n");
							break;
						case _ATDEV_:
							printf("\r\n+DEV: " LEGAL_COPYRIGHT_STR "\r\n");
							break;
						case _ATODOMV_:
						//	printf("\r\n+ODOMV: %lu\r\n",OdometerValue);
							break;
						case _ATHEX_:
							printf("\r\nUsage:\r\n AT+HEX=['R'][Lenght][Address]\r\n");
							break;
						default:
							printf("\r\nNOT IMPLEMENTED\r\n");
							break;
					}
					printf("\r\nOK\r\n");
				}
				break;
			case TESTWRITE_COMMAND:
				if(c == '?')
				{
					step= TEST_COMMAND;
					break;
				}
				else
				{
					step= WRITE_COMMAND;
				}
			case WRITE_COMMAND:
				if(c != '\r' && c != '\n')
				{
					switch(cmd)
					{
						case _ATSN_:							//Carga Hexadecimales de hasta 24 bits
							if((ctmp1= ToNumber(c)) != 255)
							{
								if(ctmp2 >= 6)
								{
									ltmp1<<= 4;
									ltmp1|= ((ltmp >> 20) & 0x0F);
								}
								ltmp<<= 4;
								ltmp|= ctmp1;
								if(ctmp2 != 0xff) ctmp2++;
							}
							else
							{
								ltmp= 0;
								ltmp1= 0;
								ctmp2= 0xff;
							}
							break;
						case _ATRST_:							//Carga Decimales de hasta 16 bits
						case _ATEXIT_:
							if((ctmp1= ToNumber(c)) != 255)
							{
								itmp*= 10;
								itmp+= ctmp1;
								if(ctmp2 != 0xff) ctmp2++;
							}
							else
							{
								itmp= 0;
								ctmp2= 0xff;
							}
							break;
						case _ATODOMV_:							//Carga Decimales de hasta 32 bits
							if((ctmp1= ToNumber(c)) != 255)
							{
								ltmp*= 10;
								ltmp+= ctmp1;
								if(ctmp2 != 0xff) ctmp2++;
							}
							else
							{
								ltmp= 0;
								ctmp2= 0xff;
							}
							break;
						case _ATHEX_:
							if((ctmp1= ToNumber(c)) != 255)
							{
								if(ctmp3 == 'R')				// es un comando de lectura?
								{
									if(ctmp2 <= 1)				// del 1ro al 2do nibble?
									{
										cHEXcnt<<= 4;
										cHEXcnt|= ctmp1;		// almaceno la Cantidad
									}

									if(	(ctmp2 >= 2) &&
										(ctmp2 <= 5) )			// del 3ro al 6to nibble?
									{
										itmp<<= 4;
										itmp|= ctmp1;			// almaceno el Address
									}

									if(	(ctmp2 > 5) &&
										(ctmp2 < MAX_HEX_ROW) )	// nibbles de mas?
									{
										ctmp2= 0xff;			// trama inválida
									}
								}
								else
								if( (ctmp3 == ':') ||
									(ctmp3 == '\\'))								// es algún comando de escritura?
								{
									cHEXRow[ctmp2>>1]<<= 4;							// voy almacenando los bytes en el array
									cHEXRow[ctmp2>>1]|= ctmp1;
									if(ctmp2 & 0x01)								// contador de nibbles es impar? es decir, llegó un byte?
									{
										if((ctmp2>>1) == 0)							// primer byte?
										{
											cHEXcnt= cHEXRow[ctmp2>>1];				// almaceno cantidad de bytes a recibir (Byte Count)
										}

										if(	((ctmp2>>1) >= 1) &&
											((ctmp2>>1) <= 2) )						// del 2do al 3er byte?
										{
											itmp<<= 8;
											itmp|= cHEXRow[ctmp2>>1];				// almaceno el Address
										}

										if((ctmp2>>1) == 3)							// cuarto byte?
										{
											if( (cHEXRow[ctmp2>>1] == 0) ||			// verifica Record Type 0 = Data
												(cHEXRow[ctmp2>>1] == 4))			// verifica Record Type 4 = Extended Linear Address
											{
												cRecycle= 1;
											}
											else
											if(cHEXRow[ctmp2>>1] == 1)				// verifica Record Type 1 = End of file
											{
												cRecycle= 0;
											}
											else
												ctmp2= 0xff;						// no verifica, descarto trama
										}

										if(	((ctmp2>>1) >= HDR_SIZE) &&				// ya pasé el Header
											((ctmp2>>1) == (HDR_SIZE+cHEXcnt)) )	// y estoy en la posición del Checksum?
										{
											cHEXchk^= 0xFF;
											cHEXchk+= 0x01;							// calculo el complemento a la base
										}

										if( (ctmp2>>1) < (HDR_SIZE+cHEXcnt) )
										{
											cHEXchk+= cHEXRow[ctmp2>>1];	// voy calculando checksum
										}
									}
								}
								if(ctmp2 != 0xff)
									if(ctmp2++ >= (2*MAX_HEX_ROW))		// si superé el máximo admisible
									{
										ctmp2= 0xff;					// invalido trama
									}
							}
							else
							{
								if(!ctmp2)					// primer char
								{
									if( (c == 'R') ||		// es alguno de los comandos válidos?
										(c == ':') ||
										(c == '\\') )
									{
										ctmp3= c;			// almaceno el comando
									}
									else
										ctmp2= 0xff;
								}
								else						// cualquier otro char debe ser un nibble
								{
									ctmp2= 0xff;
								}
							}
							break;
						default:
							break;
					}
				}
				else
				{
					ctmp1= 0;	//guardará si el comando da OK o no
					switch(cmd)
					{
						case _ATSN_:
							if((ctmp2 > 0) && (ctmp2 < 13))
							{
								//if(SerialN.value == 0)	//solo me deja cambiar el Número de Serie UNA VEZ!
								{
									tSerialN.tSNB.value= ltmp;
									tSerialN.tSNA.value= ltmp1;

									//Save Serial Number in non-Volatile Memory
									NVM_WriteSerialNumber();

									ctmp1= 1;	//OK
									break;
								}
								/*else
								{
									printf("\r\n+SN ALREADY SET!\r\n");
								}*/
							}
							break;
						case _ATRST_:
							if((ctmp2 == 1) && (itmp == 1))
							{
								// Primero almaceno los Pulsos de Odómetro
								GIE= 0;
							//	NVM_WriteOdometerCnt();
								GIE= 1;

								RESET();
							}
							break;
						case _ATODOMV_:
							if((ctmp2 > 0) && (ctmp2 < 11) && (ltmp < UINT32_MAX))
							{
								GIE= 0;
							//	OdometerCnt= ltmp;
							//	NVM_WriteOdometerCnt();
								GIE= 1;
								ctmp1= 1;		//OK
							}
							break;
						case _ATHEX_:
							switch(ctmp3)
							{
								case ':':
								case '\\':												// es una escritura
									if((ctmp2>>1) == (HDR_SIZE+cHEXcnt+1))				// y me llegó la cantidad debida de caracteres
									{
										if(cRecycle)									// si recibío End Of File que no grabe
										{
											if(cHEXchk == cHEXRow[HDR_SIZE+cHEXcnt])	// y verifica checksum
											{
												if(cHEXRow[3] == 0)						// Graba solo si el tipo es dato
													NVM_WriteBlock(itmp,&cHEXRow[HDR_SIZE],cHEXcnt);
												ctmp1= 1;		//OK
											}
										}
										else
											ctmp1= 1;		//OK
									}
									if(ctmp3 != ':')
										cRecycle= 0;
									break;

								case 'R':
									if((ctmp2 == 6) && (ctmp2 != 0xff))
									{
										cHEXcnt = ((cHEXcnt/16) * 10) + (cHEXcnt%16);	// Toma números en decimal de la línea de comandos, acá le hago cambio de base
										if(cHEXcnt <= 32)
										{
											NVM_ReadBlock(itmp,cHEXRow,cHEXcnt);
											printf("\r\n+HEX: ");
											for(ctmp3=0; ctmp3<cHEXcnt; ctmp3++)
												printf("%02X",cHEXRow[ctmp3]);
											printf("\r\n");
											ctmp1= 1;		//OK
										}
										else
										{
											printf("\r\nRead Buffer Overflow\r\n");
										}
									}
									break;
							}
							break;
						case _ATEXIT_:
							if((ctmp2 == 1) && (itmp == 1))
							{
								UART_RX_Enabled= 0;
								ctmp1= 1;		//OK
							}
							break;
						default:
							break;
					}

					if(c_prv != '\r')
					{
						if(ctmp1)
						{
							if(cRecycle)	printf(" OK...");
							else			printf("\r\nOK\r\n");
						}
						else
						{
							if(cRecycle)	printf(" ERROR...");
							else			printf("\r\nERROR\r\n");
						}
					}

					itmp= 0;
					ltmp= 0;
					ltmp1= 0;
					ctmp2= 0;
					ctmp3= 0;
					cHEXchk= 0;
				}
				break;
			case TEST_COMMAND:
				if(c == '\r' || c == '\n')
				{
					switch(cmd)
					{
						case _ATSN_:
							printf("\r\n+SN: (0 - FFFFFFFFFFFF)\r\n");
							break;
						case _ATRST_:
							printf("\r\n+RST: (1)\r\n");
							break;
						case _ATODOMV_:
							printf("\r\n+ODOMV: (0-4294967295)\r\n");
							break;
						case _ATHEX_:
							printf("\r\n+HEX: [':'|'\\'][Lenght][Address][RecdType][Data][Checksum]\r\n");
							break;
						case _ATEXIT_:
							printf("\r\n+EXIT: (1)\r\n");
							break;
						default:
							printf("\r\nNOT IMPLEMENTED\r\n");
							break;
					}
					printf("\r\nOK\r\n");
				}
				break;
			case ERROR:
				break;
		}

		if(c == '\r' || c == '\n')					// Cualquier ENTER me hace retornar a PARSE_CMD
		{
			if( (	(step == TESTWRITE_COMMAND) ||	// pasos sin CR+LF
					(step == PARSE_TYPE)		||
					(step == ERROR)				||
					(cmd == 0xFF) )				&&
				(c_prv != '\r' && c_prv != '\n') )	// ENTER solo
			{
				printf("\r\nERROR\r\n");			// Informo ERROR si corresponde
			}
			if(!cRecycle)
				step= PARSE_CMD;
		}
		c_prv= c;
	}

#endif
}

// Command Parser
uint8_t ATRecdStr(const uint8_t *str, uint8_t *ptr)
{
	if(c == str[*ptr])		//si hay coincidencia de chars
	{
		(*ptr)++;			//si hubo coincidencia en la prox. vuelta chequeo el siguiente char
		if(str[*ptr] == 0)	//si llegué al final del string a comparar
		{
			*ptr= 0;		//reinicio puntero
			return 1;		//aviso coincidencia
		}
	}
	else					//si no hubo coincidencia
	{
		if(*ptr)			//si había habido alguna coincidencia de char
		{
			*ptr= 0;		//reinicio puntero para próxima vuelta
			return 2;		//aviso que llegó algo y no es lo que esperaba
		}
		*ptr= 0;			//reinicio puntero para próxima vuelta
	}
	return 0;
}

void ATEscapeSequence(void)
{
	const char AT_ESC_SEQ[]="bac77ERI#";	//Escape Sequence to Command Mode
	static uint8_t i;

	if(RCREG == AT_ESC_SEQ[i])
	{
		if(AT_ESC_SEQ[++i] == '\0')
		{
			UART_RX_Enabled= 1;				//Enables UART RX
		}
	}
	else
	{
		i= 0;
	}
}
