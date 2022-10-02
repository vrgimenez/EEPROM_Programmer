#ifndef _DEVELOPMENT_DEFINES_H
#define _DEVELOPMENT_DEFINES_H

#pragma warning disable 373,520,759,1090

#define ENABLE_DBG_CONSOLE

//for Debug purposes
#if defined ENABLE_DBG_CONSOLE
//	#define PANIC_BTN_TO_DBG_CONSOLE
//	#define TAXI_FLAG_TO_DBG_CONSOLE
//	#define SERV_MODE_TO_DBG_CONSOLE
//	#define PR_SENSOR_TO_DBG_CONSOLE
//	#define EXT_SENSOR_TO_DBG_CONSOLE
//	#define ST_PRINT_TO_DBG_CONSOLE
//	#define ST_PACKET_TO_DBG_CONSOLE
//	#define IO_ODOM_TO_DBG_CONSOLE
//	#define VDD_TO_DBG_CONSOLE
#endif //#ifdef ENABLE_DBG_CONSOLE

#ifndef _VERSION_H
	#include "version.h"
#endif

#endif //#ifndef _DEVELOPMENT_DEFINES_H
