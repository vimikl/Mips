#include <msp430.h> 
#include "..\base.h"
#include "event.h"
#include "TA0.h"

GLOBAL Int _system_pre_init(Void) {
   // stop watchdog timer
   WDTCTL = WDTPW + WDTHOLD;
   return 0;
}

// DCO mit 8.0 MHz
// ACLK  := DCO / 32 = 250.0 kHz
// SMCLK := DCO / 32 = 250.0 kHz
// MCLK  := DCO / 1  = 8.0 MHz
#pragma FUNC_ALWAYS_INLINE(CS_init)
LOCAL Void CS_init(Void) {
   CSCTL0 = CSKEY;         // enable clock system
   CSCTL1 = DCOFSEL_3;     // DCO frequency = 8.0 MHz
                           // select clock sources
   CSCTL2 = SELA__DCOCLK   // ACLK  <- DCO
          + SELS__DCOCLK   // SMCLK <- DCO
          + SELM__DCOCLK;  // MCLK  <- DCO
                           // set frequency divider
   CSCTL3 = DIVA__32       // ACLK  : /32 = 250.0 kHz
          + DIVS__32       // SMCLK : /32 = 250.0 kHz
          + DIVM__1;       // MCLK  : /1  =   8.0 MHz

   CSCTL4 = XT1OFF         // XT1 disabled
          + XT2OFF;        // XT2 disabled

   CSCTL0_H = 0;           // disable clock system
}

#define VAL_16BIT(arg1, arg2) ((unsigned)(((arg1) << 8) | (arg2)))

#pragma FUNC_ALWAYS_INLINE(GPIO_init)
LOCAL Void GPIO_init(Void) {
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


/**
 * main.c
 */
GLOBAL Void main(Void) {

   CS_init();    // set up Clock System
   GPIO_init();  // set up Port
   Event_init(); // set up Events
   TA0_init();   // set up Timer A

   while(TRUE) {
      Event_wait();
      if (Event_tst(EVENT_IMA)) {
         TGLBIT(P2OUT, BIT7);
         Event_clr(EVENT_IMA);
      }
   }
}
