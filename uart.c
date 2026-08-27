#include <xc.h>
#include "uart.h"

void init_uart(void)
{
	// Configure serial pins
	RX_PIN = 1;
	TX_PIN = 0;

	// Configure TXSTA register: Transmitter Status and Control
	TX9 = 0;    // 8-bit transmission
	TXEN = 1;   // Enable UART transmission
	SYNC = 0;   // Asynchronous mode
	SENDB = 0;  // Don't send break character
	BRGH = 1;   // High baud rate mode

	// Configure RCSTA register: Receiver Status and Control
	SPEN = 1;   // Enable serial port
	RX9 = 0;    // 8-bit reception
	CREN = 1;   // Enable continuous reception

	// Configure baud rate (8-bit mode)
	BRG16 = 0;   // 8-bit baud rate generator
	// Set to 10 for 115200, 64 for 19200, 129 for 9600
	SPBRG = 129; // 9600 baud rate

	// Clear interrupt flags
	TXIF = 0;    // Clear transmit flag
	RCIF = 0;    // Clear receive flag
}

void putch(unsigned char byte) 
{
	/* Output one byte */
	/* Set when register is empty */
	while(!TXIF)
	{
		continue;
	}
	TXIF = 0;
	TXREG = byte;
} 

int puts(const char *s)
{
	while(*s)		
	{
		putch(*s++);	
	}
	return 0;
}

unsigned char getch(void)
{
	/* Retrieve one byte */
	/* Set when register is not empty */
	while(!RCIF)
	{
		continue;
	}
	RCIF = 0;
	return RCREG;
}

unsigned char getche(void)
{
	unsigned char c;

	putch(c = getch());

	return (c);
}
