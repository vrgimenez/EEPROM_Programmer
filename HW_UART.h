#ifndef _HW_UART_H
#define _HW_UART_H

/*Type and constant definitions---------------------------------------------------------*/
//Note: FCY must be defined in KHz. Example: MCU running@10MHz -> _Fcy= 10000
#define BAUDRATE_DBG	115200				//Baudrate when using Debug mode

#define _spbrg			(FCY*1000/BAUDRATE_DBG-1)

#define QUEUE_SIZE		(1<<7)				//only power of 2 are valid (256,128,64,32...)
#define QUEUE_MASK		(QUEUE_SIZE-1)

typedef struct {
	uint8_t *Queue;
	volatile uint8_t Head;
	volatile uint8_t Tail;
} QUEUE;

typedef struct {
	QUEUE In;
	QUEUE Out;
} UART_FIFO;

#define DBG_PORT_ISR() {\
	uint8_t tmp;\
	if(TXIE && TXIF) {\
		TXREG= DBG.Out.Queue[DBG.Out.Tail++];\
		DBG.Out.Tail &= QUEUE_MASK;\
		if(DBG.Out.Tail == DBG.Out.Head)\
			TXIE = 0;\
	}\
	if(RCIE && RCIF) {\
		if(UART_RX_Enabled) {\
			DBG.In.Queue[DBG.In.Head]= RCREG;\
			tmp = (DBG.In.Head+1) & QUEUE_MASK;\
			if(tmp != DBG.In.Tail)\
				DBG.In.Head = tmp;\
		} else {\
			ATEscapeSequence();\
		}\
	}\
}

#define SET_UART_PORTS() {\
	TRISC6= 1;			/*TX_DBG*/\
	TRISC7= 1;			/*RX_DBG*/\
}

/*External object declarations----------------------------------------------------------*/
extern UART_FIFO DBG;
extern uint8_t UART_RX_Enabled;

/*External function declarations--------------------------------------------------------*/
void	Console_Init(void);
void	UART_Init(void);
void	Queues_Init(void);
uint8_t	getch(void);
void	putch(uint8_t byte);
void	putstr(const uint8_t *str);

#endif	//#ifndef _HW_UART_H
