#include "ds1307.h"
#include "i2c.h"
#include <xc.h>

// DS1307 I2C Slave Address: 0xD0 (write) or 0xD1 (read)

void init_ds1307(void)
{
	unsigned char dummy;

	// Stop the RTC clock by setting CH bit in seconds register
	dummy = read_ds1307(SEC_ADDR);
	write_ds1307(SEC_ADDR, dummy | 0x80); 

	// Set 12-hour time format
	dummy = read_ds1307(HOUR_ADDR);
	write_ds1307(HOUR_ADDR, dummy | 0x40); 

	// Configure control register: SQW output at 32.768 kHz (RS1=1, RS0=1)
	write_ds1307(CNTL_ADDR, 0x93); 

	// Start the RTC clock by clearing CH bit in seconds register
	dummy = read_ds1307(SEC_ADDR);
	write_ds1307(SEC_ADDR, dummy & 0x7F); 

}

void write_ds1307(unsigned char address, unsigned char data)
{
	// Send I2C START, slave address (write mode), register address, and data
	i2c_start();
	i2c_write(SLAVE_WRITE);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();
}

unsigned char read_ds1307(unsigned char address)
{
	unsigned char data;

	// Send START, write slave address and register address
	i2c_start();
	i2c_write(SLAVE_WRITE);
	i2c_write(address);
	// Send repeated START for read operation
	i2c_rep_start();
	i2c_write(SLAVE_READ);
	// Read data from register and send STOP condition
	data = i2c_read();
	i2c_stop();

	return data;
}