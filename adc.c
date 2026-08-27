#include <xc.h>
#include "adc.h"

void init_adc(void)
{
	// Right justify ADC result registers for 10-bit resolution
	ADFM = 1;

	// Set acquisition time to 4 Tad cycles
	ACQT2 = 0;
	ACQT1 = 1;
	ACQT0 = 0;

	// Set conversion clock to Fosc/32 (1.6 µsecs per Tad) for 20 MHz device
	ADCS0 = 0;
	ADCS1 = 1;
	ADCS2 = 0;

	// Initialize: stop any ongoing conversion
	GODONE = 0;

	

	// Set voltage references: VSS and VDD
	VCFG1 = 0;
	VCFG0 = 0;
        
    // Configure AN4 as analog input, others as digital
	ADCON1 = 0x0B;

	// Clear ADC result registers
	ADRESH = 0;
	ADRESL = 0;

	// Enable ADC module
	ADON = 1;
}

unsigned short read_adc(unsigned char channel)
{
	unsigned short reg_val;

	// Select the ADC channel
	ADCON0 = (ADCON0 & 0xC3) | (channel << 2);

	// Start conversion and wait for completion
	GO = 1;
	while (GO);
	// Combine high and low bytes for 10-bit result
	reg_val = (ADRESH << 8) | ADRESL; 

	return reg_val;
}