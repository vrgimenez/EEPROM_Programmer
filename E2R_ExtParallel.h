/*
 * File:   E2R_ExtParallel.h
 * Author: vrgimenez
 *
 * Created on 25 de junio de 2022, 21:02
 */

#ifndef _E2R_EXTPARALLEL_H
#define	_E2R_EXTPARALLEL_H

/*Type and constant definitions-----------------------------------------------*/

//16-bit Address
typedef union {
	uint16_t value;
	struct {
		uint8_t LB;
		uint8_t HB;
	};
} T_ADDR;

// External Paged Parallel EEPROM Sizes
#define E2REXT_SIZE				(32768)
#define E2REXT_PAGE_SIZE		(64)

// External Paged Parallel EEPROM Function Macros
// Chip Select (Chip Enable)
#define CS_ENA() do { CTRL_CS_LAT= 0; } while (0)
#define CS_DIS() do { CTRL_CS_LAT= 1; } while (0)
// Read (Output Enable)
#define RD_ENA() do { CTRL_RD_LAT= 0; } while (0)
#define RD_DIS() do { CTRL_RD_LAT= 1; } while (0)
// Write (Write Enable)
#define WR_ENA() do { CTRL_WR_LAT= 0; } while (0)
#define WR_DIS() do { CTRL_WR_LAT= 1; } while (0)

/*External object declarations------------------------------------------------*/
extern T_ADDR	tAddress;

/*External function declarations----------------------------------------------*/
void	E2RExt_Init(void);
uint8_t	E2RExt_ReadByte(T_ADDR tAddr);
void	E2RExt_WriteByte(T_ADDR tAddr, uint8_t byte);
void	E2RExt_ReadBlock  (uint16_t u16Address, uint8_t *u8pData, uint8_t u8Size);
void	E2RExt_WriteBlock (uint16_t u16Address, uint8_t *u8pData, uint8_t u8Size);
void	E2RExt_ReadString (uint16_t u16Address, uint8_t *str);
void	E2RExt_WriteString(uint16_t u16Address, uint8_t *str);

#endif	/* _E2R_EXTPARALLEL_H */
