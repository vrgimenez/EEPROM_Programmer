/*
 * File:   NVM_DataMemory.h
 * Author: vrgimenez
 *
 * Created on 6 de septiembre de 2020, 22:58
 */

#ifndef _NVM_DATAMEMORY_H
#define	_NVM_DATAMEMORY_H

	#if defined _18F4620
#include "E2R_DataMemory.h"
	#elif defined _16F1579
#include "HEF_DataMemory.h"
	#endif

/*Type and constant definitions-----------------------------------------------*/

	#if defined _18F4620
#define NVM_Init()						do { E2R_Init();					} while (0)
#define NVM_ReadBlock(a,b,c)			do { E2R_ReadBlock(a,b,c);			} while (0)
#define NVM_WriteBlock(a,b,c)			do { E2R_WriteBlock(a,b,c);			} while (0)
#define NVM_ReadString(a,b)				do { E2R_ReadString(a,b);			} while (0)
#define NVM_WriteString(a,b)			do { E2R_WriteString(a,b);			} while (0)
#define NVM_ReadSerialNumber()			do { E2R_ReadSerialNumber();		} while (0)
#define NVM_WriteSerialNumber()			do { E2R_WriteSerialNumber();		} while (0)
#define NVM_ReadBitSets()				do { E2R_ReadBitSets();				} while (0)
#define NVM_WriteBitSets()				do { E2R_WriteBitSets();			} while (0)
	#elif defined _16F1579
#define NVM_Init()						do { HEF_Init();					} while (0)
#define NVM_ReadSerialNumber()			do { HEF_ReadSerialNumber();		} while (0)
#define NVM_WriteSerialNumber()			do { HEF_BuildAndWriteFlashBlock();	} while (0)
	#endif

/*External object declarations------------------------------------------------*/

/*External function declarations----------------------------------------------*/

#endif	/* _NVM_DATAMEMORY_H */

