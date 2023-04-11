#include <msp430.h>
#include "..\base.h"

// 1. GGT von Zeitvorgaben berechnen => Zeitbasis
// 2. Berechne Teilungsfaktor := Grundfrequenz des Timers * Zeitbasis
// 3. Zerlege den Teilungsfaktor mit Hilfe der Werte aus den Mengen
//    {/1, /2, /4, /8} und {/1, /2, /3, /4, /5, /6, /7, /8} so, dass
//    der Skalierungsfaktor eine möglichst kleine Ganzzahl ist
//
// Beispiel:
// 1. GGT(740 ms, 260 ms, 330 ms, 1450 ms) = 10 ms Zeitbasis
// 2. 614.4 kHz * 10 ms = 6144 (Teilungsfaktor)
// 3. 6144 {/8} {/8} = 96 (Skalierungsfaktor)

#define HIGH 0x8000
#define LOW  0x0000

#define ACKFRQ   614.4  // kHz
#define TIMEBASE 10     // ms
#define SCALING  ((UInt)(ACKFRQ * TIMEBASE))
#define TICK(t)  (((SCALING / 8) / 8) * ((t) / TIMEBASE) - 1)
#define TABSIZE 5

// Zeiten in der Tabelle sind in ms
LOCAL const Int muster[TABSIZE] = {
   HIGH | TICK( 740), // High
   LOW  | TICK( 260), // Low
   HIGH | TICK( 330), // High
   LOW  | TICK(1450), // Low
   0};
/*
LOCAL const Int muster[TABSIZE] = {
   HIGH | TICK( 10), // High
   LOW  | TICK( 10), // Low
   HIGH | TICK( 10), // High
   LOW  | TICK( 10), // Low
   0};
*/
LOCAL const Int *ptr;

#pragma FUNC_ALWAYS_INLINE(TA0_init)
GLOBAL Void TA0_init(Void) {
   ptr  = &muster[0];

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
__interrupt Void TIMER0_A1(Void) {

   UInt cnt = *ptr++;

   if (TSTBIT(cnt, HIGH)) {
      SETBIT(P2OUT, BIT7);
   } else {
      CLRBIT(P2OUT, BIT7);
   }

   CLRBIT(TA0CTL, TAIFG);    // clear interrupt flag
   TA0CCR0 = ~HIGH BAND cnt;

   if (*ptr EQ 0) {
      ptr = &muster[0];
   }

}
