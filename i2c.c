#include <xc.h>
#include "i2c.h"

void init_i2c(void)
{
	// Configure SCL and SDA as inputs (open-drain)
	TRISC3 = 1;
	TRISC4 = 1;
	// Set I2C master mode with clock frequency
	SSPCON1 = 0x28;
        
	// Set baud rate (SSPADD = 0x31 for 400 kHz @ 20MHz)
	SSPADD = 0x31;
	// Use standard I2C voltage levels
	CKE = 0;
	// Enable slew rate control for standard mode
	SMP = 1;
	// Clear I2C interrupt flag
	SSPIF = 0;
	// Clear bus collision flag
	BCLIF = 0;
}

void i2c_idle(void)
{
	// Wait for I2C interrupt flag to be set
	while (!SSPIF);
	// Clear interrupt flag
	SSPIF = 0;
}

void i2c_start(void)
{
	// Initiate I2C start condition
	SEN = 1;
	i2c_idle();
}

void i2c_stop(void)
{
	// Initiate I2C stop condition
	PEN = 1;
	i2c_idle();
}

void i2c_rep_start(void)
{
	// Initiate I2C repeated start condition
	RSEN = 1;
	i2c_idle();
}

void i2c_write(unsigned char data)
{
	// Load data to I2C transmit buffer
	SSPBUF = data;
	i2c_idle();
}

void i2c_rx_mode(void)
{
	// Enable I2C receive mode
	RCEN = 1;
	i2c_idle();
}

void i2c_no_ack(void)
{
	ACKDT = 1;
	ACKEN = 1;
}

unsigned char i2c_read(void)
{
	i2c_rx_mode();
	i2c_no_ack();

	return SSPBUF;
}