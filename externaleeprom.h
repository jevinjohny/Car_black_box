#ifndef EXTERNALEEPROM_H
#define EXTERNALEEPROM_H

/* AT24C01A Slave Address */
#define EEPROM_SLAVE_WRITE  0xA0  // Write mode (A0-A2 pins grounded)
#define EEPROM_SLAVE_READ   0xA1  // Read mode (A0-A2 pins grounded)

/* AT24C01A Specific Constants */
#define EEPROM_SIZE         128   // 128 bytes total
#define EEPROM_PAGE_SIZE    8     // 8 bytes per page

/* Function Prototypes */
void init_eeprom(void);
void eeprom_write_byte(unsigned char address, unsigned char data);
void eeprom_write_page(unsigned char address, unsigned char *data, unsigned char length);
unsigned char eeprom_read_byte(unsigned char address);
void eeprom_read_sequence(unsigned char address, unsigned char *buffer, unsigned char length);
void eeprom_wait_for_write(void);

#endif