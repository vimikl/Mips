#include <msp430.h>
#include "..\base.h"

// Zeitbasis = 10 ms
// Teilungsfaktor = 614.4 kHz * Zeitbasis = 6144
// Skalierungsfaktor = Teilungsfaktor {/8} {/8} = 96

#pragma FUNC_ALWAYS_INLINE(TA1_init)
GLOBAL Void TA1_init(Void) {
   CLRBIT(TA1CTL,   MC0 | MC1  // stop mode
                  | TAIE       // disable interrupt
                  | TAIFG);    // clear interrupt flag
   CLRBIT(TA1CCTL0, CM1 | CM0  // no capture mode
                  | CAP        // compare mode
                  | CCIE       // disable interrupt
                  | CCIFG);    // clear interrupt flag
   TA1CCR0  = 96-1;            // set up Compare Register
   TA1EX0   = TAIDEX_7;        // set up expansion register
   TA1CTL   = TASSEL__ACLK     // 614.4 kHz
            | MC__UP           // Up Mode
            | ID__8            // /8
            | TACLR;           // clear and start Timer
   SETBIT(TA1CTL, TAIE);       // enable interrupt
}

#pragma vector = TIMER1_A1_VECTOR
__interrupt Void TIMER1_A1_ISR(Void) {
   if (TSTBIT(P1IN, BIT0 | BIT1)) {
      SETBIT(P1OUT, BIT2);
   } else {
      CLRBIT(P1OUT, BIT2);
   }
   CLRBIT(TA1CTL, TAIFG); // clear interrupt flag
}
