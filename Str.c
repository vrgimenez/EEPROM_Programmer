/****************************************************************************************
 *										VRX Design										*
 ****************************************************************************************
 *
 *	Description: VRX Low Level Serial Input Tools.
 *
 *	Author: Ing. Vektor Raúl Ximen © 2011	[vrgimenez@gmail.com]
 *
 ****************************************************************************************/

/*Includes of system headers------------------------------------------------------------*/
#include <htc.h>
#include <stdio.h>
#include <stdint.h>

/*Includes of local headers-------------------------------------------------------------*/
#include "Str.h"

/*Type and constant definitions---------------------------------------------------------*/

/*Global variables----------------------------------------------------------------------*/

/*Functions-----------------------------------------------------------------------------*/
bit IsDigit(uint8_t ch)
{
	return ch >='0' && ch <= '9';
}

bit IsAlpha(uint8_t ch)
{
	return ch >='A' && ch <= 'F';
}

uint8_t Dig2Num(uint8_t c)
{
	return c - '0';
}

uint8_t Alph2Num(uint8_t c)
{
	return c - 'A' + 0xA;
}

uint8_t ToNumber(uint8_t c)
{
	if(IsDigit(c))
		c -= '0';
	else
	{
		if(IsAlpha(c))
			c = c - 'A' + 0xA;
		else
			return -1;
	}
	return c;
}

uint8_t Checksum(uint8_t *str)
{
	uint8_t cChecksum= 0;
	while(*str)
	{
		cChecksum ^= *str++;
	}
	return cChecksum;
}
