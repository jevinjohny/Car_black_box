#include <xc.h>
#include "timer0.h"

void init_timer0(void)
{
	// Use instruction cycle clock (Fosc/4) as timer source
	// Configure 8-bit timer mode
	T08BIT = 1;
	// Select internal clock source (not external pin)
	T0CS = 0;

	// Enable timer0
	TMR0ON = 1;

	// Disable prescaler (1:1 ratio)
	PSA = 1;

	// Load initial timer value
	TMR0 = 6;

	// Clear timer0 overflow interrupt flag
	TMR0IF = 0;

	// Enable timer0 overflow interrupt
	TMR0IE = 1;
}