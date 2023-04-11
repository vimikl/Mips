#include <msp430.h>
#include "..\base.h"

// Zeitbasis = 500 ms
// Teilungsfaktor = 614.4 kHz * Zeitbasis = 307200
// Skalierungsfaktor = Teilungsfaktor {/8} {/8} = 4800

#pragma FUNC_ALWAYS_INLINE(TA0_init)
GLOBAL Void TA0_init(Void) {
   CLRBIT(TA0CTL,   MC0 | MC1  // stop mode
                  | TAIE       // disable interrupt
                  | TAIFG);    // clear interrupt Flag
   CLRBIT(TA0CCTL0, CM1 | CM0  // no capture mode
                  | CAP        // compare mode
                  | CCIE       // disable interrupt
                  | CCIFG);    // clear interrupt flag
   TA0CCR0  = 4800-1;          // set up Compare Register
   TA0EX0   = TAIDEX_7;        // set up expansion register
   TA0CTL   = TASSEL__ACLK     // 614.4 kHz
            | MC__UP           // Up Mode
            | ID__8            // /8
            | TACLR;           // clear and start Timer
   SETBIT(TA0CTL, TAIE);       // enable interrupt
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt Void TIMER0_A1_ISR(Void) {
   TGLBIT(P2OUT, BIT7);      // toggle led
   CLRBIT(TA0CTL, TAIFG);    // clear interrupt flag
}

