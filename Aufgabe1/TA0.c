#include <msp430.h>
#include "..\base.h"
#include "TA0.h"

LOCAL const ULong muster[6] = {
   0b1111111100, // 10
   0b111000, // 6
   0b10, // 4
   0b110000000000, // 12
   0b11001100000000, // 14
   0b110011001100000000 // 18
};

LOCAL ULong *blink_muster;

LOCAL UChar muster_idx = 0;
LOCAL UChar new_blink_muster = 0;
LOCAL UChar switch_muster = 0;

/*
 * Man soll sich eine geeignete Datenstruktur �berlegen,
 * die eine laufzeiteffiziente Ausf�hrung der ISR erm�glicht.
 */

GLOBAL Void set_blink_muster(UInt arg) {
/*
 * Die Funktion muss so erweitert werden,
 * dass ein Blinkmuster selektiert wird.
 * Diese L�sung h�ngt stark von der gew�hlten
 * Datenstruktur ab.
 */

    switch_muster = 1;
    new_blink_muster = arg;
}

#pragma FUNC_ALWAYS_INLINE(TA0_init)
GLOBAL Void TA0_init(Void) {

   blink_muster = (ULong *) &muster[0];

   CLRBIT(TA0CTL, MC0 | MC1   // stop mode
                  | TAIE      // disable interrupt
                  | TAIFG);   // clear interrupt flag
   CLRBIT(TA0CCTL0, CM1 | CM0 // no capture mode
                  | CAP       // compare mode
                  | CCIE      // disable interrupt
                  | CCIFG);   // clear interrupt flag
   TA0CCR0  = 2400-1;           // set up Compare Register
   TA0EX0   = TAIDEX_7;       // set up expansion register
   TA0CTL   = TASSEL__ACLK    // 614.4 kHz
            | MC__UP          // Up Mode
            | ID__8           // /8
            | TACLR;          // clear and start Timer
   SETBIT(TA0CTL, TAIE        // enable interrupt
                | TAIFG);     // set interrupt flag
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt Void TIMER0_A1_ISR(Void) {
   /*
    * Der Inhalt der ISR ist zu implementieren
    */

   if ((*blink_muster >> muster_idx) & 1)  {
       SETBIT(P1OUT, BIT2);
   } else {
       CLRBIT(P1OUT, BIT2);
   }

   if (!(*blink_muster >> ++muster_idx)) {
      muster_idx = 0;
      if (switch_muster) {
          blink_muster = (ULong *) &muster[new_blink_muster];
          switch_muster = 0;
      }
   }

   CLRBIT(TA0CTL, TAIFG);    // clear interrupt flag
      __low_power_mode_off_on_exit(); // restore Active Mode on return
}
