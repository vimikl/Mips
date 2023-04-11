#include <msp430.h> 
#include "..\base.h"

// die LED am Pin 7 vom Port 2 soll mit
// einer Frequenz von 1 Hz blinken
// => High dauert 0.5 Sek, Low dauert 0.5 Sek.

GLOBAL Int _system_pre_init(Void) {
   // stop watchdog timer
   WDTCTL = WDTPW + WDTHOLD;
   return 0;
}

//   DCO: 8.0 MHz
//  ACLK: DCO / 32 = 250.0 kHz
// SMCLK: DCO / 32 = 250.0 kHz
//  MCLK: DCO /  1 =   8.0 MHz
#pragma FUNC_ALWAYS_INLINE(CS_init)
LOCAL Void CS_init(Void) {
   CSCTL0 = CSKEY;         // enable clock system
   CSCTL1 = DCOFSEL_3;     // DCO frequency = 8.0 MHz
                           // select clock sources
   CSCTL2 = SELA__DCOCLK   // ACLK  <- DCO
          | SELS__DCOCLK   // SMCLK <- DCO
          | SELM__DCOCLK;  // MCLK  <- DCO
                           // set frequency divider
   CSCTL3 = DIVA__32       // ACLK  : /32 = 250.0 kHz
          | DIVS__32       // SMCLK : /32 = 250.0 kHz
          | DIVM__1;       // MCLK  : /1  =   8.0 MHz

   CSCTL4 = XT1OFF         // XT1 disabled
          | XT2OFF;        // XT2 disabled

   CSCTL0_H = 0;           // disable clock system
}

#pragma FUNC_ALWAYS_INLINE(Port_init)
LOCAL Void Port_init(Void) {
   // Port 2: Pin 7 => output, LED1

   //                   Port2       Port1
   //               Bit 76543210    76543210
   PAOUT  = VAL_16BIT(0b00000000, 0b00000000); // clear all outputs
   PADIR  = VAL_16BIT(0b10000000, 0b00000000); // direction, set outputs
   PAIFG  = VAL_16BIT(0b00000000, 0b00000000); // clear all interrupt flags
   PAIE   = VAL_16BIT(0b00000000, 0b00000000); // disable all GPIO interrupts
   PASEL0 = VAL_16BIT(0b00000000, 0b00000000);
   PASEL1 = VAL_16BIT(0b00000000, 0b00000000);
   PAREN  = VAL_16BIT(0b00000000, 0b00000000); // without pull up

   //                   Port4       Port3
   //               Bit 76543210    76543210
   PBOUT  = VAL_16BIT(0b00000000, 0b00000000); // clear all outputs
   PBDIR  = VAL_16BIT(0b00000000, 0b00000000); // direction, set outputs
   PBIFG  = VAL_16BIT(0b00000000, 0b00000000); // clear all interrupt flags
   PBIE   = VAL_16BIT(0b00000000, 0b00000000); // disable all GPIO interrupts
   PBSEL0 = VAL_16BIT(0b00000000, 0b00000000);
   PBSEL1 = VAL_16BIT(0b00000000, 0b00000000);
   PBREN  = VAL_16BIT(0b00000000, 0b00000000); // without pull up

}

// Zeitbasis = 500 ms
// Teilungsfaktor = 8.0 MHz * Zeitbasis = 4e6
// Skalierungsfaktor = Teilungsfaktor {/32} {/8} {/5} = 3125
#pragma FUNC_ALWAYS_INLINE(Timer_init)
LOCAL Void Timer_init(Void) {
   CLRBIT(TA0CTL,   MC0 | MC1  // stop mode
                  | TAIE       // disable interrupt
                  | TAIFG);    // clear interrupt Flag
   CLRBIT(TA0CCTL0, CM1 | CM0  // no capture mode
                  | CAP        // compare mode
                  | CCIE       // disable interrupt
                  | CCIFG);    // clear interrupt flag
   TA0CCR0  = 3125-1;          // set up Compare Register CCR0
   TA0EX0   = TAIDEX_4;        // set up expansion register /5
   TA0CTL   = TASSEL__ACLK     // 250.0 kHz
            | MC__UP           // Up Mode
            | ID__8            // /8
            | TACLR;           // clear and start Timer
}

/**
 * main.c
 */
GLOBAL Void main(Void) {

   CS_init();
   Port_init();
   Timer_init();

   while(TRUE) {
      while (TSTBIT(TA0CTL, TAIFG) EQ 0) ;  // wait on interrupt
      TGLBIT(P2OUT,  BIT7);                 // toggle led
      CLRBIT(TA0CTL, TAIFG);                // clear interrupt flag
   }
}
