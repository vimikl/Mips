#include <msp430.h>
#include "..\base.h"
#include "TA0.h"

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
}

#pragma FUNC_ALWAYS_INLINE(TA0_init)
GLOBAL Void TA0_init(Void) {

   CLRBIT(TA0CTL, MC0 | MC1   // stop mode
                  | TAIE      // disable interrupt
                  | TAIFG);   // clear interrupt flag
   CLRBIT(TA0CCTL0, CM1 | CM0 // no capture mode
                  | CAP       // compare mode
                  | CCIE      // disable interrupt
                  | CCIFG);   // clear interrupt flag
   TA0CCR0  = 0;              // set up Compare Register
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
}
