#include "login.h"

unsigned char blinkflag = 0;
unsigned char pass[PSWD_LEN];
unsigned char orgpass[PSWD_LEN] ={0, 1, 0, 1, 1, 0, 1, 0};
unsigned char ind = 0;
unsigned char keyflag = 0;
unsigned char atflag = 0;
unsigned char passcount = 0;
unsigned char passcheck = 0;

unsigned int led_delay = 0;
unsigned int led_threshold = 0;

// user login
void login(void)
{
  LED = 0;

  CLEAR_DISP_SCREEN;
  clcd_print((const unsigned char *)"ENTER PASSWORD", LINE1(0));
  clcd_print((const unsigned char *)"_", LINE2(0));
  unsigned int delay = 1000;
  unsigned int delay1 = 0;
  unsigned int delay2 = 0;

  while (1)
  {
    keyflag = 0;
    unsigned char key = read_switches(STATE_CHANGE);
    if (key == MK_SW1 && ind < PSWD_LEN)
    {
      keyflag = 1;
      pass[ind++] = 0;
      clcd_print((const unsigned char *)"*", LINE2(ind - 1));
    } else if (key == MK_SW2 && ind < PSWD_LEN)
    {
      keyflag = 1;
      pass[ind++] = 1;
      clcd_print((const unsigned char *)"*", LINE2(ind - 1));
    }

    if (ind == PSWD_LEN)
    {
      atflag = 1;
      passcheck = 0;
      for (int i = 0; i < PSWD_LEN; i++)
      {
        if (pass[i] != orgpass[i])
        {
          passcheck = 1;
          break;
        }
      }

      CLEAR_DISP_SCREEN;
      if (passcount >= 4)
      {
        clcd_print((const unsigned char *)"ATTEMPT FINISHED", LINE1(0));
        led_threshold = 10;
        clcd_print((const unsigned char *)"RESET BOARD", LINE2(0));

        while (1);
      } else
      {
        if (passcheck)
        {
          passcount++;
          clcd_print((const unsigned char *)"WRONG PASSWORD", LINE1(0));
          if (passcount == 1)
          {
            clcd_print((const unsigned char *)"4 CHANCE REM", LINE2(0));
            led_threshold = 8000;
            __delay_ms(500);

          } else if (passcount == 2)
          {
            clcd_print((const unsigned char *)"3 CHANCE REM", LINE2(0));
            led_threshold = 6000;
            __delay_ms(500);

          } else if (passcount == 3)
          {
            clcd_print((const unsigned char *)"2 CHANCE REM", LINE2(0));
            led_threshold = 4000;
            __delay_ms(500);

          } else if (passcount == 4)
          {
            clcd_print((const unsigned char *)"1 CHANCE REM", LINE2(0));
            led_threshold = 2000;
            __delay_ms(500);
          }

        }
        else
        {
          clcd_print((const unsigned char *)"ACCESS GRANTED", LINE1(0));
          LED = 0;
          led_threshold = 0;
          atflag = 0;
          passcount = 0;
          passcheck = 0;
          led_delay = 0;
          ind = 0;
          keyflag = 0;

          return;
        }
      }
      for (unsigned long int d = 0; d < 50000; d++)
        ;
      CLEAR_DISP_SCREEN;
      clcd_print((const unsigned char *)"ENTER PASSWORD", LINE1(0));
      clcd_print("_", LINE2(0));
      //      led_threshold=0;
      ind = 0;
    }

    if (keyflag == 0)
    {
      if (blinkflag)
        clcd_print((const unsigned char *)" ", LINE2(ind));
      else
        clcd_print((const unsigned char *)"_", LINE2(ind));
    }
  }
}

//change the menu password
void changepassword(void)
{
  CLEAR_DISP_SCREEN;
  clcd_print((const unsigned char *)"NEW PASSWORD", LINE1(0));

  unsigned char i = 0;
  __delay_ms(1000);

  unsigned char duppass[PSWD_LEN] ={0};
  while (1)
  {
    unsigned char key = read_switches(STATE_CHANGE);

    if (key == MK_SW1)
    {
      clcd_putch('*', LINE2(i));
      duppass[i] = 0;
      i++;
    }
    else if (key == MK_SW2)
    {
      clcd_putch('*', LINE2(i));
      duppass[i] = 1;
      i++;
    }

    if (i == PSWD_LEN)
    {
      for (int j = 0; j < PSWD_LEN; j++)
      {
        orgpass[j] = duppass[j];
      }

      CLEAR_DISP_SCREEN;
      return;
    }
  }
}
