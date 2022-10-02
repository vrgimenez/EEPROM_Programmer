/*Includes of system headers------------------------------------------------------------*/
#include <stdint.h>

/*Includes of local headers-------------------------------------------------------------*/
#include "ROM-Set.h"

#if (ROM_SET == BEER19_32K)
	#include "ROMS\ROM-BEER19_32K.c"

#elif (ROM_SET == KVALLEY_PIPPOLS_32K)
	#include "ROMS\ROM-KVALLEY_PIPPOLS_32K.c"

#else
	#error ROM SET NOT SUPPORTED
#endif

/*Type and constant definitions---------------------------------------------------------*/

/*Global variables----------------------------------------------------------------------*/

/*Functions-----------------------------------------------------------------------------*/
