/*
 * File:   newmain.c
 * Author: jevin
 *
 * Created on 10 June, 2026, 7:37 PM
 */


#include <xc.h>
#include "i2c.h"
#include "clcd.h"
#include "adc.h"
#include "matrix_keypad.h"
#include "eeprom.h"
#include "ds1307.h"
#include "uart.h"
#include  "main.h"
#include "timer0.h"
#include "dashboard.h"

unsigned int speed = 0;
unsigned int val = 0;
char arr[5];
unsigned char col = 0;
unsigned char page = 1;
unsigned char menutoggle = 0;

unsigned char clock_reg[3];
unsigned char calender_reg[4];
unsigned char time[9];
unsigned char date[11];

void main(void)
{
  init_i2c();
  init_clcd();
  init_adc();
  init_matrix_keypad();
  init_ds1307();
  init_uart();
  
  init_timer0();

  GIE = 1;
  PEIE = 1;
  TRISA5=1; //POT

  arr[4]='N'; // starting value for gear
  CLEAR_DISP_SCREEN;
  clcd_print("SPEED G TIME", LINE1(0));

  while (1)
  {
    
    if (!menutoggle)
    {
      dashboard();
    } else
    {
      menu();
    }
  }
  return;
}
