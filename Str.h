#ifndef _STR_H
#define _STR_H

/*Type and constant definitions-----------------------------------------------*/

/*External object declarations------------------------------------------------*/

/*External function declarations----------------------------------------------*/
bit		IsDigit(uint8_t ch);
bit		IsAlpha(uint8_t ch);
uint8_t	Dig2Num(uint8_t c);
uint8_t	Alph2Num(uint8_t c);
uint8_t	ToNumber(uint8_t c);
uint8_t	Checksum(uint8_t *str);

#endif
