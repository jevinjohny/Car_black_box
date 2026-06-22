#include <xc.h>
#define LED PORTBbits.RB0
extern unsigned char blinkflag;
extern unsigned int led_delay;
extern unsigned int led_threshold;
extern unsigned char atflag; 

void __interrupt() isr(void)
{
  static unsigned short blinkcount;

  if (TMR0IF)
  {
    if (atflag)
    {
      if (led_threshold != 0)
      {
        if (led_delay++ >= led_threshold)
        {
          led_delay = 0;
          LED = !LED;
        }
      }
      
    }
    TMR0 = TMR0 + 8;

    if (blinkcount++ == 10000)
    {
      blinkcount=0;
      blinkflag=!blinkflag;
    }
    TMR0IF = 0;
  }
}