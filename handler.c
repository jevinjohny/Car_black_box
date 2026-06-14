


#include <xc.h>
#include "i2c.h"
#include "clcd.h"
#include "adc.h"
#include "matrix_keypad.h"
#include "eeprom.h"
#include "ds1307.h"
#include "uart.h"
#include  "main.h"

void dashboard (void)
{
  //read speed from pot 
  unsigned long sum=0;
  for (int i=0; i < 20; i++)// reads 20 values and finds the average of it
  {
    sum=sum + read_adc(4);
  }
  val =sum / 20; // average
  speed = (((unsigned long) val * MAXSPEED) / 1023); //digital speed value to 0-249

  //gear and collision switch
  unsigned char key =read_switches(STATE_CHANGE); //reads digital keypad 
  if (key == MK_SW1)//increments gear from N->1->2->3->4->5->6->R
  {
    if (arr[4] == 'N')
      arr[4]='1';
    else if (arr[4] >= '1' && arr[4] < '6')
      arr[4]++;
    else if (arr[4] == '6')
      arr[4]='R';
  } else if (key == MK_SW2)//decrements gear from R->6->5->4->3->2->1->N
  {
    if (arr[4] == 'R')
      arr[4]='6';
    else if (arr[4] > '1' && arr[4] <= '6')
      arr[4]--;
    else if (arr[4] == '1')
      arr[4]='N';
  } else if (key == MK_SW3)// collision switch
  {
    col=1; // collision flag is set
    arr[4]='C';
  } else if (key == MK_SW4 )//menu toggle
  {
    menutoggle=1; //toggle main menu
    page=1;
    CLEAR_DISP_SCREEN;
    clcd_print("MAIN MENU", LINE1(0));
    __delay_ms(100);
  }

  if (col) // values to be transmitted if collision is detected
  {
    speed=0;
    arr[4]='C';
  }

  speed_to_str(speed, arr); // converts integer value to string
  clcd_print(arr, LINE2(0));
  clcd_putch(arr[4], LINE2(6));
}


//main menu
void menu(void)
{
  unsigned char key =read_switches(STATE_CHANGE); //reads digital keypad

  if (key == MK_SW6)//page down
  {
    CLEAR_DISP_SCREEN;
    if (page <= 3 )
      page++;
    else
      page=4;
  } else if (key == MK_SW5)//page up
  {
    CLEAR_DISP_SCREEN;
    if (page > 1)
      page--;
    else
      page=1;
  } else if (key == MK_SW4)
  {
    CLEAR_DISP_SCREEN;
    menutoggle=0;
    clcd_print("SPEED G TIME", LINE1(0));
  }

  if (!menutoggle)
    return;

  //page scrolling
  switch (page) 
  {
    case 1:
    {
      clcd_print("1.VIEW LOG", LINE1(0));
      clcd_print("2.CLEAR LOG", LINE2(0));
      break;
    }
    case 2:
    {
      clcd_print("2.CLEAR LOG", LINE1(0));
      clcd_print("3.DWNLD LOG", LINE2(0));
      break;
    }
    case 3:
    {
      clcd_print("3.DWNLD LOG", LINE1(0));
      clcd_print("4.SET TIME", LINE2(0));
      break;
    }
    case 4:
    {
      clcd_print("4.SET TIME", LINE1(0));
      clcd_print("5.CHNG PSWRD", LINE2(0));
      break;
    }
  }
}


// converts integer value to string
void speed_to_str(unsigned int speed, char *str)
{
  unsigned char hundreds;
  unsigned char tens;
  unsigned char ones;

  //extracts digits
  hundreds = speed / 100;
  tens = (speed / 10) % 10;
  ones = speed % 10;

  if (hundreds == 0)//replaces leading zeros in hundred place with ' '
    str[0] = ' ';
  else
    str[0] = hundreds + '0';

  if (hundreds == 0 && tens == 0)//replaces leading zeros in tens place with ' '
    str[1] = ' ';
  else
    str[1] = tens + '0';

  str[2] = ones + '0';
  str[3] = '\0';
}