#include <xc.h>
#include "dashboard.h"
#include "adc.h"
#include "clcd.h"
#include "ds1307.h"
#include "matrix_keypad.h"
#include "main.h"
#include "log.h"
#include "login.h"

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
  speed =
      (((unsigned long)val * MAXSPEED) / 1023); // digital speed value to 0-249

  unsigned char key = read_switches(STATE_CHANGE); // reads digital keypad
  if (col) // values to be transmitted if collision is detected
  {
    speed = 0;
    arr[4] = 'C';
  } else {
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
    clcd_print("MAIN MENU", LINE1(0));
    __delay_ms(1000);
  }

  if (!menutoggle) {
    get_time();
    display_time();
    speed_to_str(speed, arr); // converts integer value to string
    clcd_print(arr, LINE2(0));
    clcd_putch(arr[4], LINE2(6));
  }
}

void display_time(void) 
{
  clcd_print(time, LINE2(8));

  if (clock_reg[0] & 0x40) {
    if (clock_reg[0] & 0x20) {
      clcd_print("PM", LINE1(13));
    } else {
      clcd_print("AM", LINE1(13));
    }
  }
}

static void get_time(void) 
{
  clock_reg[0] = read_ds1307(HOUR_ADDR);
  clock_reg[1] = read_ds1307(MIN_ADDR);
  clock_reg[2] = read_ds1307(SEC_ADDR);

  if (clock_reg[0] & 0x40) {
    time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
    time[1] = '0' + (clock_reg[0] & 0x0F);
  } else {
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