#include <xc.h>
#define LED PORTBbits.RB0
// External flag and timing variables for LED control
extern unsigned char blinkflag;
extern unsigned int led_delay;
extern unsigned int led_threshold;
extern unsigned char atflag; 

void __interrupt() isr(void)
{
  // Static counter for LED blinking logic
  static unsigned short blinkcount;

  // Check Timer0 overflow interrupt
  if (TMR0IF)
  {
    // LED control when AT mode is active
    if (atflag)
    {
      // Toggle LED based on threshold timing
      if (led_threshold != 0)
      {
        if (led_delay++ >= led_threshold)
        {
          // Reset delay and toggle LED
          led_delay = 0;
          LED = !LED;
        }
      }
      
    }
    // Reload Timer0 with offset for consistent timing
    TMR0 = TMR0 + 8;

    // Generate blink flag at lower frequency (every ~10000 overflows)
    if (blinkcount++ == 10000)
    {
      blinkcount=0;
      blinkflag=!blinkflag;
    }
    // Clear Timer0 interrupt flag
    TMR0IF = 0;
  }
}