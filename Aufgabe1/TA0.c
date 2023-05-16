#include <msp430.h>
#include "..\base.h"
#include "TA0.h"

LOCAL const ULong muster[6] = {
   0b1111111100, // 10 
   0b111000, // 6
   0b10, // 2
   0b110000000000, // 12
   0b11001100000000, // 14
   0b110011001100000000 // 18
};

LOCAL const Int muster_size[6] = {
   10,
   6,
   2,
   12,
   14,
   18
};

LOCAL Int muster_idx;

LOCAL ULong *blink_muster;
LOCAL Int *blink_muster_size;

LOCAL UChar new_blink_muster;

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

    new_blink_muster = arg;
    switch_muster = 1;
}

#pragma FUNC_ALWAYS_INLINE(TA0_init)
GLOBAL Void TA0_init(Void) {

   blink_muster = (ULong *) &muster[0];
   blink_muster_size = (Int *) &muster_size[0];
   muster_idx = *blink_muster_size;
   
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

   if (*blink_muster & (1 << muster_idx)) {
       SETBIT(P1OUT, BIT2);
   } else {
       CLRBIT(P1OUT, BIT2);
   }

   if (--muster_idx < 0) {
      if (switch_muster) {
          blink_muster = (ULong *) &muster[new_blink_muster];
          blink_muster_size = (Int *) &muster_size[new_blink_muster];
          switch_muster = 0;
      }
      muster_idx = *blink_muster_size;
   }

   CLRBIT(TA0CTL, TAIFG);    // clear interrupt flag
      __low_power_mode_off_on_exit(); // restore Active Mode on return
}
