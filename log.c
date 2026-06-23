#include "log.h"

unsigned char logindex = 0;
unsigned char logcount = 0;

void store_log(void)
{
  unsigned char addr;
  unsigned char speedstr[4];
  unsigned char i;

  speed_to_str(speed, speedstr);

  addr = logindex * LOGSIZE;
  
  // time
  for (i = 0; i < 8; i++)
  {
    write_internal_eeprom(addr++, time[i]);
  }

  // space
  write_internal_eeprom(addr++, ' ');

  // event
  write_internal_eeprom(addr++, 'G');
  write_internal_eeprom(addr++, arr[4]);

  // space
  write_internal_eeprom(addr++, ' ');

  // speed
  write_internal_eeprom(addr++, speedstr[0]);
  write_internal_eeprom(addr++, speedstr[1]);
  write_internal_eeprom(addr++, speedstr[2]);

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
  } else
  {
    start = logindex;
  }

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
      rdlog[i] = read_internal_eeprom(adr);
      adr++;
    }

    rdlog[LOGSIZE] = '\0';

    CLEAR_DISP_SCREEN;
    clcd_print("LOG  TIME G  SPD", LINE1(0));
    clcd_putch((j + 1) + '0', LINE1(4));
    clcd_print(rdlog, LINE2(0));

    j++;
    __delay_ms(2000);
  }
  CLEAR_DISP_SCREEN;
}

void downloadlog(void)
{
  if (logcount == 0)
  {
    puts("NO logs\n");
    return;
  }

  unsigned char start;

  if (logcount < MAXLOGS)
  {
    start = 0;
  } else
  {
    start = logindex;
  }

  puts("# TIME     GEAR SPD\r\n");
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
      rdlog[i] = read_internal_eeprom(adr);
      adr++;
    }

    rdlog[LOGSIZE] = '\0';

    putch('0' + j);
    putch(' ');
    puts(rdlog);
    puts("\r\n");

    j++;
  }
}

