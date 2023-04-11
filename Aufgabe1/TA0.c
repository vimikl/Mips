#include <msp430.h>
#include "..\base.h"
#include "TA0.h"

/*
 * Man soll sich eine geeignete Datenstruktur überlegen,
 * die eine laufzeiteffiziente Ausführung der ISR ermöglicht.
 */

GLOBAL Void set_blink_muster(UInt arg) {
/*
 * Die Funktion muss so erweitert werden,
 * dass ein Blinkmuster selektiert wird.
 * Diese Lösung hängt stark von der gewählten
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
    * const Datenelemente Button: pin, u8 BIT0, .., BIT7 (byte)
    * var Counter cnt Int 0..5 statt u int
    * if (--cnt < 0) 2 Befehle (neg flag )
    * if (cnt++ > 5) 2 Befehle + compare mit 5
    * counter runterzählen effizienters
    * var State enum S0, S1 (compiler sucht passenden datentyp für 0, 1)
    * const event TEvent Event_Btn1
    * const Port Adresse *u8 (char *) & P1IN
    *
    * 2structs für var und const
    * später konstante tabelle tabconstbuttons
    * musterlösung hat ram=stack:32B,data:20B;fram=prg:528B,const:80B
    * structs reihenfolge größter datentyp >> kleinster datentyp
    */

}
