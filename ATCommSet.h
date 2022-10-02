#ifndef _AT_COMM_SET_H
#define _AT_COMM_SET_H

/*Type and constant definitions-----------------------------------------------*/

/*External object declarations------------------------------------------------*/

/*External function declarations----------------------------------------------*/
void	ATCommandParser(void);
uint8_t	ATRecdStr(const uint8_t *str, uint8_t *ptr);
void	ATEscapeSequence(void);

#endif	//#ifndef _AT_COMM_SET_H
