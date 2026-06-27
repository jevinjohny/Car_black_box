#include "clcd.h"
#include "main.h"
#include "matrix_keypad.h"
#include "login.h"
#include "log.h"
#include <builtins.h>
#include <pic18.h>
#include <xc.h>

// main menu

void menu(void)
{
    unsigned char key = read_switches(STATE_CHANGE); // reads digital keypad

    if (key == MK_SW1) // page down
    {
        CLEAR_DISP_SCREEN;
        if (page <= 4)
            page++;
        else
            page = 5;
    }
    else if (key == MK_SW2) // page up
    {
        CLEAR_DISP_SCREEN;
        if (page > 1)
            page--;
        else
            page = 1;
    }
    else if (key == MK_SW4)
    {
        CLEAR_DISP_SCREEN;
        menutoggle = 0;
        clcd_print("SPEED G TIME", LINE1(0));
    }

    if (!menutoggle)
        return;

    // page scrolling
    switch (page)
    {
    case 1:
    {
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
        clcd_print("2.CLEAR LOG", LINE1(0));
        clcd_print("3.DWNLD LOG", LINE2(0));
        clcd_putch('<', LINE1(14));
        if (key == MK_SW5)
        {
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





