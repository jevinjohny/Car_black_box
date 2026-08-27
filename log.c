#include "main.h"
#include "log.h"
#include "clcd.h"
#include "externaleeprom.h"
#include "matrix_keypad.h"
#include <builtins.h>

// Current index for storing new logs (circular buffer)
unsigned char logindex = 0;
// Total number of logs stored
unsigned char logcount = 0;

void store_log(void)
{
  unsigned char addr;
  unsigned char speedstr[4];
  unsigned char i;

  // Convert speed to string format
  speed_to_str(speed, speedstr);

  // Calculate EEPROM address for current log entry
  addr = logindex * LOGSIZE;

  // Write timestamp to EEPROM
  for (i = 0; i < 8; i++)
  {
    eeprom_write_byte(addr++, time[i]);
  }

  // Write separator space
  eeprom_write_byte(addr++, ' ');

  // Write current gear/event
  eeprom_write_byte(addr++, arr[4]);

  // Write separator space
  eeprom_write_byte(addr++, ' ');

  // Write vehicle speed
  eeprom_write_byte(addr++, speedstr[0]);
  eeprom_write_byte(addr++, speedstr[1]);
  eeprom_write_byte(addr++, speedstr[2]);

  // Move to next log index
  logindex++;

  // Wrap around if circular buffer is full
  if (logindex >= MAXLOGS)
  {
    logindex = 0;
  }

  // Increment log count if buffer not yet full
  if (logcount < MAXLOGS)
  {
    logcount++;
  }
}

void viewlog(void)
{
  // Check if any logs exist
  if (logcount == 0)
  {
    CLEAR_DISP_SCREEN;
    clcd_print("NO LOGS", LINE1(0));
    __delay_ms(1000);
    CLEAR_DISP_SCREEN;
    return;
  }

  // Determine starting index (oldest log)
  unsigned char start;

  // If buffer not full, start from beginning; otherwise from current index
  if (logcount < MAXLOGS)
  {
    start = 0;
  }
  else
  {
    start = logindex;
  }

  // EEPROM address and read buffer for log data
  unsigned char adr = 0;
  unsigned char rdlog[LOGSIZE + 1];

  // Navigate through logs
  unsigned char j = 0;
  while (j < logcount)
  {
    // Calculate log number using circular buffer wraparound
    unsigned char logno = (start + j) % MAXLOGS;
    adr = logno * LOGSIZE;

    // Read log entry from EEPROM
    for (int i = 0; i < LOGSIZE; i++)
    {
      rdlog[i] = read_external(adr);
      adr++;
    }

    // Null-terminate the log string
    rdlog[LOGSIZE] = '\0';

    // Display current log on LCD with header
    CLEAR_DISP_SCREEN;
    clcd_print("#   TIME   G  SPD", LINE1(0));
    clcd_putch((j) + '0', LINE2(0));  // Log number
    clcd_print(rdlog, LINE2(2));       // Log data

    // Handle navigation keys
    unsigned char key = read_switches(STATE_CHANGE);
    if (key == MK_SW8)  // Next log
    {
      // Move to next log
      j++;
      // Prevent overflow
      if (j == logcount)
      {
        j = logcount - 1;
      }
    }
    else if (key == MK_SW7)  // Previous log
    {
      j--;
      // Prevent underflow (wrap to 0)
      if (j == 255)
      {
        j = 0;
      }
    }
    else if (key == MK_SW6)  // Exit log view
    {
      CLEAR_DISP_SCREEN;
      return;
    }
  }
  CLEAR_DISP_SCREEN;
}

void downloadlog(void)
{
  // Download all logs via UART
  CLEAR_DISP_SCREEN;
  // Check if logs exist
  if (logcount == 0)
  {
    puts("NO logs\n");
    return;
  }

  // Determine starting index
  unsigned char start;

  // Start from beginning if buffer not full, else from oldest entry
  if (logcount < MAXLOGS)
  {
    start = 0;
  }
  else
  {
    start = logindex;
  }

  // Print header for log download
  puts("#    TIME  G  SPD\r\n");
  unsigned char adr = 0;
  unsigned char rdlog[LOGSIZE + 1];

  // Transmit all logs
  unsigned char j = 0;
  while (j < logcount)
  {
    // Calculate log index in circular buffer
    unsigned char logno;
    logno = (start + j) % MAXLOGS;
    adr = logno * LOGSIZE;

    // Read log entry from EEPROM
    for (int i = 0; i < LOGSIZE; i++)
    {
      rdlog[i] = read_external(adr);
      adr++;
    }

    // Null-terminate the log string
    rdlog[LOGSIZE] = '\0';

    // Send log number and data over UART
    putch('0' + j);
    putch(' ');
    puts(rdlog);
    puts("\r\n");

    j++;
  }
  // Display completion message
  clcd_print("DOWNLOAD LOGS", LINE1(0));
  clcd_print("FINISHED", LINE2(0));
  __delay_ms(1000);
  CLEAR_DISP_SCREEN;
}
