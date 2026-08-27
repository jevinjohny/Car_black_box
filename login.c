#include "login.h"

// LED blink status indicator
unsigned char blinkflag = 0;
// User password input buffer
unsigned char pass[PSWD_LEN];
// Original/correct password stored in EEPROM
unsigned char orgpass[PSWD_LEN] = {0, 1, 0, 1, 1, 0, 1, 0};
// Current position in password entry
unsigned char ind = 0;
// Flag to indicate key was pressed
unsigned char keyflag = 0;
// Flag to indicate AT mode (attempts tracking)
unsigned char atflag = 0;
// Count of failed password attempts
unsigned char passcount = 0;
// Flag to indicate password mismatch
unsigned char passcheck = 0;

// LED delay counter for blinking rate
unsigned int led_delay = 0;
// LED threshold for blink frequency control
unsigned int led_threshold = 0;

// User login function with password authentication
void login(void) 
{
  // Ensure LED is off initially
  LED = 0;

  // Display login prompt
  CLEAR_DISP_SCREEN;
  clcd_print((const unsigned char *)"ENTER PASSWORD", LINE1(0));
  clcd_print((const unsigned char *)"_", LINE2(0));
  // Delay variables for timing
  unsigned int delay = 1000;
  unsigned int delay1 = 0;
  unsigned int delay2 = 0;

  // Main login loop - accept password input
  while (1) {
    keyflag = 0;
    // Read keypad input
    unsigned char key = read_switches(STATE_CHANGE);
    // Button 1: Enter 0 bit
    if (key == MK_SW1 && ind < PSWD_LEN) {
      keyflag = 1;
      pass[ind++] = 0;
      clcd_print((const unsigned char *)"*", LINE2(ind - 1));
    } 
    // Button 2: Enter 1 bit
    else if (key == MK_SW2 && ind < PSWD_LEN) {
      keyflag = 1;
      pass[ind++] = 1;
      clcd_print((const unsigned char *)"*", LINE2(ind - 1));
    }

    // Check password when all digits entered
    if (ind == PSWD_LEN) {
      atflag = 1;
      passcheck = 0;
      // Compare entered password with original
      for (int i = 0; i < PSWD_LEN; i++) {
        if (pass[i] != orgpass[i]) {
          passcheck = 1;
          break;
        }
      }

      CLEAR_DISP_SCREEN;
      // Check if maximum attempts exceeded
      if (passcount >= 4) {
        clcd_print((const unsigned char *)"ATTEMPT FINISHED", LINE1(0));
        // Activate error LED
        led_threshold = 10;
        clcd_print((const unsigned char *)"RESET BOARD", LINE2(0));

        while (1)
          ;
      } else {
        // Password incorrect - show remaining attempts
        if (passcheck) {
          passcount++;
          clcd_print((const unsigned char *)"WRONG PASSWORD", LINE1(0));
          // First attempt failed
          if (passcount == 1) {
            clcd_print((const unsigned char *)"4 CHANCE REM", LINE2(0));
            led_threshold = 8000;
            __delay_ms(500);

          } 
          // Second attempt failed
          else if (passcount == 2) {
            clcd_print((const unsigned char *)"3 CHANCE REM", LINE2(0));
            led_threshold = 6000;
            __delay_ms(500);

          } 
          // Third attempt failed
          else if (passcount == 3) {
            clcd_print((const unsigned char *)"2 CHANCE REM", LINE2(0));
            led_threshold = 4000;
            __delay_ms(500);

          } 
          // Fourth attempt failed
          else if (passcount == 4) {
            clcd_print((const unsigned char *)"1 CHANCE REM", LINE2(0));
            led_threshold = 2000;
            __delay_ms(500);
          }

        } else {
          // Password correct - grant access
          clcd_print((const unsigned char *)"ACCESS GRANTED", LINE1(0));
          // Reset all authentication variables
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
      // Delay before clearing screen
      for (unsigned long int d = 0; d < 50000; d++);
      CLEAR_DISP_SCREEN;
      clcd_print((const unsigned char *)"ENTER PASSWORD", LINE1(0));
      clcd_print("_", LINE2(0));
      // Reset index for next password entry
      ind = 0;
    }

    // Display blinking cursor when no key pressed
    if (keyflag == 0) {
      if (blinkflag)
        clcd_print((const unsigned char *)" ", LINE2(ind));
      else
        clcd_print((const unsigned char *)"_", LINE2(ind));
    }
  }
}

// Change the user password in EEPROM
void changepassword(void) 
{
  CLEAR_DISP_SCREEN;
  clcd_print((const unsigned char *)"NEW PASSWORD", LINE1(0));

  __delay_ms(1000);

  unsigned char duppass[PSWD_LEN] = {0};
  unsigned char i = 0;
  unsigned char cfmpass[PSWD_LEN] = {0};
  unsigned char j = 0;
  while (1) 
  {
    unsigned char key = read_switches(STATE_CHANGE);
    if (key == MK_SW1) 
    {
      clcd_putch('*', LINE2(i));
      duppass[i] = 0;
      i++;
    } else if (key == MK_SW2) {
      clcd_putch('*', LINE2(i));
      duppass[i] = 1;
      i++;
    }

    if (i == PSWD_LEN) 
    {
      CLEAR_DISP_SCREEN;
      break;
    }
  }

  unsigned char flag = 0;
  clcd_print((const unsigned char *)"CFM PASSWORD", LINE1(0));
  while (1) 
  {
    unsigned char key = read_switches(STATE_CHANGE);
  
    if (key == MK_SW1) 
    {
      clcd_putch('*', LINE2(j));
      cfmpass[j] = 0;
      j++;
    } 
    else if (key == MK_SW2) 
    {
      clcd_putch('*', LINE2(j));
      cfmpass[j] = 1;
      j++;
    }
  
    if (j == PSWD_LEN) 
    {
      for (int k = 0; k < PSWD_LEN; k++) 
      {
        if (cfmpass[k] != duppass[k]) 
        {
          CLEAR_DISP_SCREEN;
          clcd_print((const unsigned char *)"PsWD MISMATCH", LINE1(0));
          clcd_print((const unsigned char *)"PsWD NOT SET", LINE2(0));
          __delay_ms(1000);
  
          CLEAR_DISP_SCREEN;
          flag = 1;
          return;
        }
      }
      if (!flag) {
        for (int k = 0; k < PSWD_LEN; k++) 
        {
          orgpass[k] = duppass[k];
        }
        CLEAR_DISP_SCREEN;
        clcd_print((const unsigned char *)"NEW PWD SET..", LINE1(0));
        __delay_ms(1000);
        return;
      }
    }
  }
}
