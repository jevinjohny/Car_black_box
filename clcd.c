#include <xc.h>
#include "clcd.h"

void clcd_write(unsigned char byte, unsigned char control_bit)
{
  // Set RS pin (0=command, 1=data)
  CLCD_RS = control_bit;
  // Write data to LCD port
  CLCD_PORT = byte;

  // Toggle enable signal (minimum 200ns pulse)
  CLCD_EN = HI;
  CLCD_EN = LO;

  // Set port as input to read busy flag
  PORT_DIR = INPUT;
  CLCD_RW = HI;    // Read mode
  CLCD_RS = INSTRUCTION_COMMAND;

  // Wait for busy flag to clear
  do
  {
    CLCD_EN = HI;
    CLCD_EN = LO;
  } while (CLCD_BUSY);

  // Restore to write mode
  CLCD_RW = LO;
  PORT_DIR = OUTPUT;
}

void init_clcd()
{
  // Configure PortD as output for LCD data bus
  TRISD = 0x00;
  // Configure PortC (except RC7-RC5) as output for LCD control signals
  TRISC = TRISC & 0xF8;

  CLCD_RW = LO;

  // Wait for LCD power-up (minimum 30ms)
  __delay_ms(30);

  /* The CLCD Startup Sequence */
  clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND);
  __delay_us(4100);
  clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND);
  __delay_us(100);
  clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND);
  __delay_us(1);

  CURSOR_HOME;
  __delay_us(100);
  TWO_LINE_5x8_MATRIX_8_BIT;
  __delay_us(100);
  CLEAR_DISP_SCREEN;
  __delay_us(500);
  DISP_ON_AND_CURSOR_OFF;
  __delay_us(100);
}

void clcd_print(const unsigned char *data, unsigned char addr)
{
  clcd_write(addr, INSTRUCTION_COMMAND);
  while (*data != '\0')
  {
    clcd_write(*data++, DATA_COMMAND);
  }
}

void clcd_putch(const unsigned char data, unsigned char addr)
{
  clcd_write(addr, INSTRUCTION_COMMAND);
  clcd_write(data, DATA_COMMAND);
}
