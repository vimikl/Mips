/*
 * TA0.c
 *
 *  Created on: 22.04.2018
 *      Author: Admin
 */

#include <msp430.h>
#include "..\base.h"
#include "event.h"

// Zeitbasis = 500 ms
// Teilungsfaktor = 8.0 MHz * Zeitbasis = 4000000
// Skalierungsfaktor = Teilungsfaktor {/32} {/8} {/5} = 3125

#pragma FUNC_ALWAYS_INLINE(TA0_init)
GLOBAL Void TA0_init(Void) {
    CLRBIT(TA0CTL,   MC0       // stop mode
                   + MC1       // stop mode
                   + TAIE      // disable interrupt
                   + TAIFG);   // clear interrupt Flag
    CLRBIT(TA0CCTL0, CM1       // no capture mode
                   + CM0       // no capture mode
                   + CAP       // compare mode
                   + CCIE      // disable interrupt
                   + CCIFG);   // clear interrupt flag
    TA0CCR0  = 3125-1;        // set up Compare Register CCR0
    TA0EX0   = TAIDEX_4;       // set up expansion register /5
    TA0CTL   = TASSEL__ACLK    // 250.0 kHz
             + MC__UP          // Up Mode
             + ID__8           // /8
             + TACLR;          // clear and start Timer
    SETBIT(TA0CTL, TAIE);      // enable interrupt
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt Void TIMER0_A1_ISR(Void) {
   Event_set(EVENT_IMA);            // set up event
   CLRBIT(TA0CTL, TAIFG);           // clear interrupt flag
   __low_power_mode_off_on_exit();  // restore Active Mode on return

}
