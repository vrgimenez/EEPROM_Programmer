# Programador EEPROM basado en un PIC18F4620

### Caracter�sticas

- Es una herramienta espec�fica para grabar Memorias AT28C256 [256K (32K x 8) Paged Parallel EEPROM];
- En principio el c�digo es adaptable a cualquier otro MCU con 27 GPIOs libres (8 de Datos, 16 de Direcciones, y 3 de Control);
- A su vez, tambi�n se debe poder adaptar para grabar Memorias de menor o mayor tama�o (hasta 64K por supuesto); e incluso memorias de otros fabricantes siempre y cuando adaptemos el c�digo a los tiempos especificados por el fabricante.
- No dispone de Software, se debe embeber la EEPROM a grabar en un Array...si, una porquer�a, pero funciona;
- Graba y/o Verifica el Grabado de todo el rango de la EEPROM; si bien el FW tiene funciones de grabado y lectura de un Byte o de una P�gina, no tiene implementado el grabado de un rango de memoria.
- El Array se puede generar con [Bin2C](https://www.segger.com/free-utilities/bin2c/) que es una herramienta gratuita de [SEGGER Microcontroller GmbH](https://www.segger.com/);
- Bienvenidos sean los que le quieran hacer un Software, pero primero hay que hacer que �ste FW pueda recibir los datos por UART...;
- Tiene implementado un int�rprete de Comandos AT expandible, el cual se detalla a continuaci�n:

## Int�rpretes de Comandos AT
### Habilitaci�n del Int�rprete de Comandos AT
Se debe ingresar por �nica vez cada vez la secuencia de escape "bac77ERI#" sin comillas.
Para salir del int�rprete de Comandos AT, adem�s del comando _Reset_, se puede utilizar tambi�n el comando _Exit_.
### Sintaxis
Se debe poner "AT" o "at" al principio de cada l�nea de Comando. Para terminar una l�nea de Comando hay que enviar &lt;CR&gt;&lt;LF&gt;.
### Tipos de Comandos y Respuestas AT
Comando | Sintaxis | Descripci�n
-|-|-
Testeo | AT+&lt;x&gt;=? | El equipo devuelve la lista de par�metros y los rango de valores con el correspondiente Comando de Escritura.
Lectura | AT+&lt;x&gt;? | �ste comando devuelve el valor actual del par�metro o par�metros.
Escritura | AT+&lt;x&gt;=... | �ste comando setea los valores del par�metro definido por el usuario.
### Lista de Comandos AT

- AT+SN: Lee y Graba el N�mero de Serie del FW embebido.
- AT+RST: Resetea el FW.
- AT+VER: Presenta la versi�n del FW en formato YY.MMDDa.
- AT+DEV: Presenta los datos del Desarrollador del FW (yo, o sea)
- AT+E2RWR: Escribe y Verifica todo el rango de la EEPROM.
- AT+E2RRD:  Lee todo el rango de la EEPROM. (no implementado)
- AT+E2RCHK: Verifica todo el rango de la EEPROM.
- AT+HEX: Lee y Graba la EEPROM interna del MCU en formato Intel HEX de hasta 32 bytes por l�nea. **Se podr�a adaptar para que reciba desde una App externa y grabe en la EEPROM Paralelo externa.**
- AT+EXIT: Sale del modo Comando AT.

## Arrays
Actualmente el FW tiene seteables en Tiempo de Compilaci�n dos Set de Arrays de 32K.
Uno que se corresponde a la ROM del Adaptador IDE de MSX, el Beer IDE v2.02 v1.9RC1. La misma es de 16K, as� que est� la misma ROM concatenada una destr�s de la otra.
El otro contiene, concatenados tambien, las ROMs de dos Juegos de 16K.
### C�mo agregar un nuevo Array
Por prolijidad, los nuevos Arrays se deben agregar en la Carpeta ROMS/, y el formato de nombre que se eligi� es **ROM-[NAME1\_NAME2\_...]\_[SIZE].c/h**, donde size puede ser 8K, 16K, 32K � 64K. Como nuestro grabador de momento solo graba EEPROMs de 32K, 32K es lo �nico que veremos de momento.
Para no modificar el resto del c�digo cada vez que cambiamos la ROM a grabar, todos los Array se deber�n declarar/definir como:
```C
const uint8_t ROM_DATA_32K[32768UL + 1] = {
    //...
};
```
Luego agregamos nuestra nueva ROM en **ROM-Set.h** de la siguiente manera:
```C
#ifndef _ROMSET_H
	#define _ROMSET_H

	#define BEER19_32K				1
	#define GAME1_GAME2_32K		2
	#define MY_ADDED_ROM_32K		2		// Nueva Agregada

	#define ROM_SET					MY_ADDED_ROM_32K		// Selecciono el nuevo ROM Set

	#if (ROM_SET == BEER19_32K)
		#include "ROMS\ROM-BEER19_32K.h"
		#define ROM_SET_SIZE	ROM_BEER19_32K_SIZE

	#elif (ROM_SET == GAME1_GAME2_32K)
		#include "ROMS\ROM-GAME1_GAME2_32K.h"
		#define ROM_SET_SIZE	ROM_GAME1_GAME2_32K_SIZE

	#elif (ROM_SET == MY_ADDED_ROM_32K)		// Agrego �stas tres l�neas para que incluya el Array[] y defina el Tama�o
		#include "ROMS\ROM-MY_ADDED_ROM_32K.h"
		#define ROM_SET_SIZE	ROM_MY_ADDED_ROM_32K_SIZE

	#else
		#error ROM SET NOT SUPPORTED
	#endif
#endif
```
Y listo! a Compilar y a grabar el FW, Luego conectar todo, escribir bac77ERI# y luego AT+E2RWR=1&lt;CR&gt;&lt;LF&gt; y a esperar que todo salga bien!

## Compilaci�n y Grabado en MCU
�ste FW se realiz� en horas ociosas, en las que no dispon�a de un [ICD3](https://www.microchip.com/ICD3) o un [PICKIT3](https://www.microchip.com/pickit3). Lo que ten�a era un viejo [ICD2 LE](https://www.microchip.com/forums/m288679.aspx) que solo es compatible con el viejo [MPLAB IDE](https://www.microchip.com/en-us/tools-resources/archives/mplab-ecosystem). De los compiladores que soporta, el mejor, en mi humilde opini�n, es el [Hi-Tech PICC18](https://www.microchip.com/en-us/development-tool/SW500007).
Seguramente el c�digo es f�cilmente adaptable al nuevo [MPLAB X IDE](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide) y al [Compilador XC8](https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers) que uso cuando desarrollo para terceros.
Bueno, en fin, �sto esta desarrollado y probado con el [MPLAB IDE v8.92](https://ww1.microchip.com/downloads/en/DeviceDoc/MPLAB_IDE_8_92.zip) (el �ltimo desarrollador por Microchip) y el Compilador HI-TECH C PRO for the PIC18 MCU Family v9.66.
### Toolchain
Como les dec�a, el programador/debugger es el [ICD2 LE](https://www.microchip.com/forums/m288679.aspx) que se puede reemplazar f�cilmente con un [ICD2](https://www.microchip.com/en-us/development-tool/MPLAB-ICD-2), un [ICD3](https://www.microchip.com/ICD3) o un [PICKIT3](https://www.microchip.com/pickit3), o cualquier otro soportado por el MPLAB IDE v8.92.

**Tabla de Contenidos**

[TOCM]

[TOC]

**Readme.md** creado con [MEditor.md](https://pandao.github.io/editor.md/en.html)
