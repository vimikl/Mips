#include <msp430.h>
#include "..\base.h"
#include "UCA1.h"

#define ACKFRQ   614400              // Hz
#define TA0FRQ   ((ACKFRQ / 8) / 8)  // Hz
#define TICK(t)  ((UInt)(TA0FRQ * (t) / 1000) - 1) // t in Millisekunden

LOCAL UInt counter;

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
   TA0CCR0  = TICK(1000);     // set up Compare Register for 500 ms
   TA0EX0   = TAIDEX_7;       // set up expansion register /8
   TA0CTL   = TASSEL__ACLK    // 614.4 kHz
            | MC__UP          // Up Mode
            | ID__8           // /8
            | TACLR;          // clear and start Timer
   SETBIT(TA0CTL, TAIE);      // enable interrupt
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt Void TIMER0_A1(Void) {
   SETBIT(P3OUT, BIT4);   // TEST on
   UInt i;
   UInt tmp = counter;
   CLRBIT(TA0CTL, TAIFG); // clear interrupt flag
   for(i=1; i LE 4; i++) {
      UChar ch = 0x0F BAND tmp;
      ch += '0';
      UCA1_emit(i, ch);
      tmp >>= 4;
   }
   counter += 1;
   CLRBIT(P3OUT, BIT4);   // TEST off
}
