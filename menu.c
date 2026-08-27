#include "clcd.h"
#include "main.h"
#include "matrix_keypad.h"
#include "login.h"
#include "log.h"
#include <builtins.h>
#include <pic18.h>
#include <xc.h>

// Main menu system with 5 pages of options

void menu(void)
{
  // Read keypad input
  unsigned char key = read_switches(STATE_CHANGE);

  // Page down navigation (SW8)
  if (key == MK_SW8)
  {
    CLEAR_DISP_SCREEN;
    if (page <= 4)
      page++;
    else
      page = 5;
  }
  // Page up navigation (SW7)
  else if (key == MK_SW7)
  {
    CLEAR_DISP_SCREEN;
    if (page > 1)
      page--;
    else
      page = 1;
  }
  // Exit to dashboard (SW4)
  else if (key == MK_SW4)
  {
    CLEAR_DISP_SCREEN;
    menutoggle = 0;
    clcd_print("SPEED G TIME", LINE1(0));
  }
  // Exit to dashboard (SW6)
  else if (key==MK_SW6)
  {
    CLEAR_DISP_SCREEN;
    menutoggle = 0;
    clcd_print("SPEED G TIME", LINE1(0));
  }

  // Exit if menu not toggled
  if (!menutoggle)
    return;

  // Display menu options based on current page
  switch (page)
  {
  case 1:
  {
    // Page 1: View Log option
    clcd_print("1.VIEW LOG", LINE1(0));
    clcd_print("2.CLEAR LOG", LINE2(0));
    clcd_putch('<', LINE1(14));
    if (key == MK_SW5)
    {
      viewlog();
    }
    break;
  }
  case 2:
  {
    // Page 2: Clear Log option
    clcd_print("2.CLEAR LOG", LINE1(0));
    clcd_print("3.DWNLD LOG", LINE2(0));
    clcd_putch('<', LINE1(14));
    if (key == MK_SW5)
    {
      // Reset log counters and clear
      logcount = 0;
      logindex = 0;
      CLEAR_DISP_SCREEN;
      clcd_print("LOG CLEARED...", LINE1(0));
      __delay_ms(1000);
      CLEAR_DISP_SCREEN;
    }
    break;
  }
  case 3:
  {
    // Page 3: Download Log option
    clcd_print("3.DWNLD LOG", LINE1(0));
    clcd_print("4.SET TIME", LINE2(0));
    clcd_putch('<', LINE1(14));
    if (key == MK_SW5)
    {
      downloadlog();
    }
    break;
  }
  case 4:
  {
    // Page 4: Set Time option
    clcd_print("4.SET TIME", LINE1(0));
    clcd_print("5.CHNG PSWRD", LINE2(0));
    clcd_putch('<', LINE1(14));
    if (key == MK_SW5)
    {
      settime();
    }
    break;
  }
  case 5:
  {
    // Page 5: Change Password option
    clcd_print("5.CHNG PSWRD", LINE1(0));
    clcd_putch('<', LINE1(14));
    if (key == MK_SW5)
    {
      changepassword();
    }
    break;
  }
  }
}





