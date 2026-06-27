#include "dashboard.h"
#include "adc.h"
#include "clcd.h"
#include "ds1307.h"
#include "log.h"
#include "login.h"
#include "main.h"
#include "matrix_keypad.h"
#include <builtins.h>
#include <xc.h>

unsigned int speed = 0;
unsigned int val = 0;
unsigned char col = 0;
char arr[5];
unsigned char clock_reg[3] ={0};
unsigned char calender_reg[4] ={0};
unsigned char time[9];
unsigned char date[11];

static void get_time(void);

void dashboard(void)
{
  // read speed from pot
  unsigned long sum = 0;
  for (int i = 0; i < 20; i++) // reads 20 values and finds the average of it
  {
    sum = sum + read_adc(4);
  }
  val = sum / 20; // average
  
  speed = (((unsigned long) val * MAXSPEED) / 1023); // digital speed value to 0-249

  unsigned char key = read_switches(STATE_CHANGE); // reads digital keypad

  if (col) // values to be transmitted if collision is detected
  {
    speed = 0;
    arr[4] = 'C';
  } else
  {
    // gear and collision switch
    if (key == MK_SW1) // increments gear from N->1->2->3->4->5->6->R
    {
      if (arr[4] == 'N')
        arr[4] = '1';
      else if (arr[4] >= '1' && arr[4] < '6')
        arr[4]++;
      else if (arr[4] == '6')
        arr[4] = 'R';

      get_time();
      store_log();

    } else if (key == MK_SW2) // decrements gear from R->6->5->4->3->2->1->N
    {
      if (arr[4] == 'R')
        arr[4] = '6';
      else if (arr[4] > '1' && arr[4] <= '6')
        arr[4]--;
      else if (arr[4] == '1')
        arr[4] = 'N';

      get_time();
      store_log();
    } else if (key == MK_SW3) // collision switch
    {
      col = 1; // collision flag is set
      speed = 0;
      arr[4] = 'C';

      get_time();
      store_log();
    }
  }

  if (key == MK_SW4) // menu toggle
  {
    // user login
    login();

    menutoggle = 1; // toggle main menu
    page = 1;

    CLEAR_DISP_SCREEN;
    clcd_print((const unsigned char *)"MAIN MENU", LINE1(0));
    __delay_ms(1000);
  }

  if (!menutoggle)
  {
    get_time();
    display_time();
    speed_to_str(speed, arr); // converts integer value to string
    clcd_print((const unsigned char *)arr, LINE2(0));
    clcd_putch(arr[4], LINE2(6));
  }
}

void display_time(void)
{
  clcd_print(time, LINE2(8));
}

static void get_time(void)
{
  clock_reg[0] = read_ds1307(HOUR_ADDR);
  clock_reg[1] = read_ds1307(MIN_ADDR);
  clock_reg[2] = read_ds1307(SEC_ADDR);

  if (clock_reg[0] & 0x40)
  {
    time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
    time[1] = '0' + (clock_reg[0] & 0x0F);
  } else
  {
    time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
    time[1] = '0' + (clock_reg[0] & 0x0F);
  }
  time[2] = ':';
  time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
  time[4] = '0' + (clock_reg[1] & 0x0F);
  time[5] = ':';
  time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
  time[7] = '0' + (clock_reg[2] & 0x0F);
  time[8] = '\0';
}

// converts integer value to string

void speed_to_str(unsigned int speed, char *str)
{
  unsigned char hundreds;
  unsigned char tens;
  unsigned char ones;

  // extracts digits
  hundreds = speed / 100;
  tens = (speed / 10) % 10;
  ones = speed % 10;

  if (hundreds == 0) // replaces leading zeros in hundred place with ' '
    str[0] = ' ';
  else
    str[0] = hundreds + '0';

  if (hundreds == 0 &&
      tens == 0) // replaces leading zeros in tens place with ' '
    str[1] = ' ';
  else
    str[1] = tens + '0';

  str[2] = ones + '0';
  str[3] = '\0';
}

void settime(void)
{
  CLEAR_DISP_SCREEN;
  unsigned char sec=0;
  unsigned char min=0;
  unsigned char hr=0;

  hr = read_ds1307(HOUR_ADDR);
  min = read_ds1307(MIN_ADDR);
  sec = read_ds1307(SEC_ADDR);

  hr = (((hr >> 4) & 0x03) * 10) + (hr & 0x0F);
  min = (((min >> 4) * 10 )+ (min & 0x0F));
  sec = (((sec >> 4) * 10) + (sec & 0x0F));

  clcd_print((const unsigned char *)"SET  TIME", LINE1(0));
  __delay_ms(1000);

  unsigned char mode = 0;

  CLEAR_DISP_SCREEN;
  while (1)
  {
    unsigned char key = read_switches(STATE_CHANGE);

    if (key == MK_SW6)
    {
      mode++;
      if (mode > 2)
        mode = 2;
    } else if (key == MK_SW7)
    {
      mode--;
      if (mode == 255)
        mode = 0;
    } else if (key == MK_SW5)
    {
      sec = (((sec / 10) << 4 | (sec % 10)));
      min = (((min / 10) << 4 | (min % 10)));
      hr = (((hr / 10) << 4 | (hr % 10)));

      // Clear bit 6 to ensure 24-hour mode
      hr &= 0xBF;  // Clear bit 6 (0x40)

      write_ds1307(SEC_ADDR, sec);
      write_ds1307(MIN_ADDR, min);
      write_ds1307(HOUR_ADDR, hr);

      CLEAR_DISP_SCREEN;
      clcd_print((const unsigned char *)"TIME SET!...", LINE1(0));
      __delay_ms(1000);
      CLEAR_DISP_SCREEN;
      return;
    }

    switch (mode) {
    case 0:
    {
      if (key == MK_SW1)
      {
        CLEAR_DISP_SCREEN;

        hr++;
        if (hr > 23)
        {
          hr = 0;
        }
      } else if (key == MK_SW2)
      {
        CLEAR_DISP_SCREEN;
        hr--;
        if (hr == 255)
          hr = 23;
      }
      break;
    }
    case 1:
    {
      if (key == MK_SW1)
      {
        CLEAR_DISP_SCREEN;
        min++;
        if (min > 59)
        {
          min = 0;
        }
      } else if (key == MK_SW2)
      {
        CLEAR_DISP_SCREEN;
        min--;
        if (min == 255)
          min = 59;
      }
      break;
    }
    case 2:
    {
      if (key == MK_SW1)
      {
        CLEAR_DISP_SCREEN;
        sec++;
        if (sec > 59)
        {
          sec = 0;
        }
      } else if (key == MK_SW2)
      {
        CLEAR_DISP_SCREEN;
        sec--;
        if (sec == 255)
          sec = 59;
      }
      break;
    }
    }
    time[0] = (hr / 10) + '0';
    time[1] = (hr % 10) + '0';
    time[2] = ':';

    time[3] = (min / 10) + '0';
    time[4] = (min % 10) + '0';
    time[5] = ':';

    time[6] = (sec / 10) + '0';
    time[7] = (sec % 10) + '0';
    time[8] = '\0';

    clcd_print(time, LINE2(0));

    if (mode == 0)
    {
      clcd_print((const unsigned char *)"EDIT HOURS  ", LINE1(0));
    } else if (mode == 1)
    {
      clcd_print((const unsigned char *)"EDIT MINUTES", LINE1(0));
    } else if (mode == 2)
    {
      clcd_print((const unsigned char *)"EDIT SEC    ", LINE1(0));
    }
  }
}