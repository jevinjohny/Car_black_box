#include "i2c.h"
#include "externaleeprom.h"
#include <xc.h>
#define _XTAL_FREQ 20000000



void eeprom_write_byte(unsigned char address, unsigned char data)
{
    /* AT24C01A uses 8-bit address (0x00 to 0x7F) */
    i2c_start();
    i2c_write(EEPROM_SLAVE_WRITE);  // Send device address with write bit 
    
    i2c_write(address);              // Send memory address (8-bit)
    
    i2c_write(data);                 // Send data byte

    i2c_stop();

    __delay_ms(5);
}

unsigned char read_external(unsigned char address)
{
	unsigned char data;

	i2c_start();
	i2c_write(EEPROM_SLAVE_WRITE);
	i2c_write(address);
	i2c_rep_start();
	i2c_write(EEPROM_SLAVE_READ);
	data = i2c_read();
	i2c_stop();

	return data;
}

