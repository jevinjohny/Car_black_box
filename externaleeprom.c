#include "i2c.h"
#include "externaleeprom.h"
#include <xc.h>
#define _XTAL_FREQ 20000000

/* 
 * AT24C01A Slave address
 * A0 - Write Mode
 * A1 - Read Mode
 * 
 * Note: AT24C01A uses single byte addressing (0x00 to 0x7F)
 * Only lower 7 bits of address are valid
 */

void init_eeprom(void)
{
    /* No special initialization needed for AT24C01A */
    /* Just ensure I2C is initialized before using EEPROM */
}

void eeprom_write_byte(unsigned char address, unsigned char data)
{
    /* AT24C01A uses 8-bit address (0x00 to 0x7F) */
    i2c_start();
    i2c_write(EEPROM_SLAVE_WRITE);  // Send device address with write bit
    i2c_ack();                       // Wait for ACK
    
    i2c_write(address);              // Send memory address (8-bit)
    i2c_ack();                       // Wait for ACK
    
    i2c_write(data);                 // Send data byte
    i2c_ack();                       // Wait for ACK
    
    i2c_stop();
    
    /* Wait for internal write cycle (max 5ms for AT24C01A) */
    __delay_ms(5);
}

void eeprom_write_page(unsigned char address, unsigned char *data, unsigned char length)
{
    unsigned char i;
    unsigned char current_addr = address;
    
    /* Check if we exceed page boundary (8 bytes per page) */
    if ((address & (EEPROM_PAGE_SIZE - 1)) + length > EEPROM_PAGE_SIZE)
    {
        /* Split into multiple page writes */
        unsigned char first_part = EEPROM_PAGE_SIZE - (address & (EEPROM_PAGE_SIZE - 1));
        unsigned char remaining = length - first_part;
        
        /* Write first part */
        eeprom_write_page(address, data, first_part);
        
        /* Write remaining part */
        eeprom_write_page(address + first_part, data + first_part, remaining);
        return;
    }
    
    /* Start I2C communication */
    i2c_start();
    i2c_write(EEPROM_SLAVE_WRITE);
    i2c_ack();
    
    i2c_write(address);
    i2c_ack();
    
    /* Write data bytes */
    for (i = 0; i < length; i++)
    {
        i2c_write(data[i]);
        i2c_ack();
    }
    
    i2c_stop();
    
    /* Wait for internal write cycle */
    __delay_ms(5);
}

unsigned char eeprom_read_byte(unsigned char address)
{
    unsigned char data;
    
    /* Write address phase */
    i2c_start();
    i2c_write(EEPROM_SLAVE_WRITE);
    i2c_ack();
    
    i2c_write(address);
    i2c_ack();
    
    /* Read data phase */
    i2c_rep_start();
    i2c_write(EEPROM_SLAVE_READ);
    i2c_ack();
    
    /* Read byte with NACK */
    data = i2c_read();
    
    i2c_stop();
    
    return data;
}

void eeprom_read_sequence(unsigned char address, unsigned char *buffer, unsigned char length)
{
    unsigned char i;
    
    // Write address
    i2c_start();
    i2c_write(EEPROM_SLAVE_WRITE);
    i2c_ack();
    i2c_write(address);
    i2c_ack();
    
    // Restart for read
    i2c_rep_start();
    i2c_write(EEPROM_SLAVE_READ);
    i2c_ack();
    
    // Read bytes
    for (i = 0; i < length; i++) {
        buffer[i] = i2c_read();  // Get byte
        if (i == length - 1) 
        {
            i2c_no_ack();        // NACK on last byte
        } else {
            i2c_ack();           // ACK for others
        }
    }
    
    i2c_stop();
}

void eeprom_wait_for_write(void)
{
    /* Poll the EEPROM by sending a start condition and device address */
    /* The EEPROM will not ACK until write cycle is complete */
    while (1)
    {
        i2c_start();
        i2c_write(EEPROM_SLAVE_WRITE);
        
        if (!ACKSTAT)  // If ACK received, write is complete
        {
            i2c_stop();
            break;
        }
        
        i2c_stop();
        
        /* Small delay before retrying */
        __delay_us(100);
    }
}