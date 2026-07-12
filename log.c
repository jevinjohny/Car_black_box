#include "main.h"
#include "log.h"
#include "clcd.h"
#include "externaleeprom.h"
#include "matrix_keypad.h"
#include <builtins.h>

unsigned char logindex = 0;
unsigned char logcount = 0;

void store_log(void) {
  unsigned char addr;
  unsigned char speedstr[4];
  unsigned char i;

  speed_to_str(speed, speedstr);

  addr = logindex * LOGSIZE;

  // time
  for (i = 0; i < 8; i++) {
    eeprom_write_byte(addr++, time[i]);
  }

  // space
  eeprom_write_byte(addr++, ' ');

  // event
  eeprom_write_byte(addr++, arr[4]);

  // space
  eeprom_write_byte(addr++, ' ');

  // speed
  eeprom_write_byte(addr++, speedstr[0]);
  eeprom_write_byte(addr++, speedstr[1]);
  eeprom_write_byte(addr++, speedstr[2]);

  logindex++;

  if (logindex >= MAXLOGS) 
  {
    logindex = 0;
  }

  if (logcount < MAXLOGS) 
  {
    logcount++;
  }
}

void viewlog(void) 
{
  if (logcount == 0) 
  {
    CLEAR_DISP_SCREEN;
    clcd_print("NO LOGS", LINE1(0));
    __delay_ms(1000);
    CLEAR_DISP_SCREEN;
    return;
  }

  unsigned char start;

  if (logcount < MAXLOGS) 
  {
    start = 0;
  } 
  else 
  {
    start = logindex;
  }

  unsigned char adr = 0;
  unsigned char rdlog[LOGSIZE + 1];

  unsigned char j = 0;
  while (j < logcount) 
  {
    unsigned char logno = (start + j) % MAXLOGS;
    adr = logno * LOGSIZE;

    for (int i = 0; i < LOGSIZE; i++) 
    {
      rdlog[i] = read_external(adr);
      adr++;
    }

    // eeprom_read_sequence(adr, rdlog, LOGSIZE);
    rdlog[LOGSIZE] = '\0';

    CLEAR_DISP_SCREEN;
    clcd_print("#   TIME   G  SPD", LINE1(0));
    clcd_putch((j ) + '0', LINE2(0));
    clcd_print(rdlog, LINE2(2));

    unsigned char key = read_switches(STATE_CHANGE);
    if (key==MK_SW8)
    {
      j++;
      if (j==logcount)
      {
        j=logcount-1;
      }
    }
    else if (key==MK_SW7)
    {
      j--;
      if (j == 255)
      {
        j=0;
      }
    }
    else if (key==MK_SW6)
    {
      CLEAR_DISP_SCREEN;
      return;
    }
  }
  CLEAR_DISP_SCREEN;
}

void downloadlog(void) 
{
  CLEAR_DISP_SCREEN;
  if (logcount == 0) 
  {
    puts("NO logs\n");
    return;
  }

  unsigned char start;

  if (logcount < MAXLOGS) 
  {
    start = 0;
  } 
  else 
  {
    start = logindex;
  }

  puts("#    TIME  G  SPD\r\n");
  unsigned char adr = 0;
  unsigned char rdlog[LOGSIZE + 1];

  unsigned char j = 0;
  while (j < logcount) 
  {
    unsigned char logno;

    logno = (start + j) % MAXLOGS;
    adr = logno * LOGSIZE;

    for (int i = 0; i < LOGSIZE; i++) 
    {
      rdlog[i] = read_external(adr);
      adr++;
    }

    // eeprom_read_sequence(adr, rdlog, LOGSIZE);

    rdlog[LOGSIZE] = '\0';

    putch('0' + j);
    putch(' ');
    puts(rdlog);
    puts("\r\n");

    j++;
  }
  clcd_print("DOWNLOAD LOGS", LINE1(0));
  clcd_print("FINISHED", LINE2(0));
  __delay_ms(1000);
  CLEAR_DISP_SCREEN;
}
