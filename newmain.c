/*
 Documentation :
 *      Name : Jevin Johny .V
 *      Roll no. : 25036B_039
 *      Start Date  : 10/06/2026
        End Date    : 29/06/2026
        Description :  This project implements a car black box system on 
        a PIC18F4580 microcontroller that continuously monitors and logs vehicle events in real-time. 
        The system captures speed data via ADC, current gear position, and synchronized timestamps 
        using an RTC (DS1307), storing all information in a circular buffer in external EEPROM. Users
         interact with the system through a 16x2 LCD display and 4x4 matrix keypad, with a multi-page menu 
         for navigation and binary password authentication for security. Critical features include event logging
          with wraparound buffer management, LED feedback for authentication attempts, and UART capability for 
          downloading logs to a computer. This automotive monitoring solution is ideal for accident analysis, 
          driver behavior tracking, and comprehensive vehicle diagnostics.
*/

#include <xc.h>
#include "i2c.h"
#include "clcd.h"
#include "adc.h"
#include "matrix_keypad.h"
#include "ds1307.h"
#include "uart.h"
#include  "main.h"
#include "timer0.h"
#include "dashboard.h"

// Global variables for speed and input handling
unsigned int speed = 0;
unsigned int val = 0;
char arr[5];
unsigned char col = 0;         // Column counter
unsigned char page = 1;         // Current page number
unsigned char menutoggle = 0;   // Menu toggle flag

// RTC registers and time/date buffers
unsigned char clock_reg[3];
unsigned char calender_reg[4];
unsigned char time[9];          // Time string buffer
unsigned char date[11];         // Date string buffer

void main(void)
{
  // Initialize all peripherals
  init_i2c();
  init_clcd();
  init_adc();
  init_matrix_keypad();
  init_ds1307();
  init_uart();
  
  // Initialize timer for interrupts
  init_timer0();

  // Enable global and peripheral interrupts
  GIE = 1;
  PEIE = 1;
  TRISA5=1; // Potentiometer input

  // Initialize gear to Neutral
  arr[4]='N';
  
  CLEAR_DISP_SCREEN;
  clcd_print("SPEED G    TIME", LINE1(0));

  // Main application loop
  while (1)
  {
    // Toggle between dashboard and menu displays
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
