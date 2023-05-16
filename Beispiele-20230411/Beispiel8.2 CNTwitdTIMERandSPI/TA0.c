#include <msp430.h>
#include "..\base.h"
#include "event.h"

#define ACKFRQ   614400              // Hz
#define TA0FRQ   ((ACKFRQ / 8) / 8)  // Hz
#define TICK(t)  ((UInt)(TA0FRQ * (t) / 1000) - 1) // t in Millisekunden

GLOBAL UInt counter;

#pragma FUNC_ALWAYS_INLINE(TA0_init)
GLOBAL Void TA0_init(Void) {
   counter = 0;
   CLRBIT(TA0CTL, MC0 | MC1   // stop mode
                  | TAIE      // disable interrupt
                  | TAIFG);   // clear interrupt flag
   CLRBIT(TA0CCTL0, CM1 | CM0 // no capture mode
                  | CAP       // compare mode
                  | CCIE      // disable interrupt
                  | CCIFG);   // clear interrupt flag
   TA0CCR0  = TICK(1000);     // set up Compare Register
   TA0EX0   = TAIDEX_7;       // set up expansion register /8
   TA0CTL   = TASSEL__ACLK    // 614.4 kHz
            | MC__UP          // Up Mode
            | ID__8           // /8
            | TACLR;          // clear and start Timer
   SETBIT(TA0CTL, TAIE);      // enable interrupt
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt Void TIMER0_A1(Void) {
   CLRBIT(TA0CTL, TAIFG);           // clear interrupt flag
   counter += 1;
   Event_set(EVENT_UPDATE);         // set up event
   __low_power_mode_off_on_exit();  // restore Active Mode on return
}
