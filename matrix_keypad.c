#include <xc.h>
#include "matrix_keypad.h"
#define _XTAL_FREQ 20000000

void init_matrix_keypad(void)
{
	// Configure PORTB as digital I/O (disable analog)
	ADCON1 = 0x0F;

	// Configure rows (RB7-RB5) as outputs, columns (RB4-RB1) as inputs
	TRISB = 0x1E;

	// Enable pull-ups on column inputs
	RBPU = 0;

	// Initialize row pins to high
	MATRIX_KEYPAD_PORT = MATRIX_KEYPAD_PORT | 0xE0;
}

unsigned char scan_key(void)
{
	// Scan Row 1: Set ROW1 low, others high
	ROW1 = LO;
	ROW2 = HI;
	ROW3 = HI;

	if (COL1 == LO)
	{
		return 1;
	}
	else if (COL2 == LO)
	{
		return 4;
	}
	else if (COL3 == LO)
	{
		return 7;
	}
	else if (COL4 == LO)
	{
		return 10;
	}

	ROW1 = HI;
	ROW2 = LO;
	ROW3 = HI;

	if (COL1 == LO)
	{
		return 2;
	}
	else if (COL2 == LO)
	{
		return 5;
	}
	else if (COL3 == LO)
	{
		return 8;
	}
	else if (COL4 == LO)
	{
		return 11;
	}

	ROW1 = HI;
	ROW2 = HI;
	ROW3 = LO;
	/* TODO: Why more than 2 times? */
	ROW3 = LO;

	if (COL1 == LO)
	{
		return 3;
	}
	else if (COL2 == LO)
	{
		return 6;
	}
	else if (COL3 == LO)
	{
		return 9;
	}
	else if (COL4 == LO)
	{
		return 12;
	}

	return 0xFF;
}

unsigned char read_switches(unsigned char detection_type)
{
	static unsigned char once = 1, key;
        __delay_ms(5);
		if (detection_type == STATE_CHANGE)
		{
			key = scan_key();
			if(key != 0xFF && once  )
			{
				once = 0;
			return key;
		}
		else if(key == 0xFF)
		{
			once = 1;
		}
	}
	else if (detection_type == LEVEL_CHANGE)
	{

		return scan_key();
	}

	return 0xFF;
}
