#include <msp430.h>
#include "..\base.h"
#include "TA1.h"
#include "event.h"

LOCAL UChar count1 = 0;
LOCAL UChar count2 = 0;
LOCAL UChar state1 = 0;
LOCAL UChar state2 = 0;

#pragma FUNC_ALWAYS_INLINE(TA1_init)
GLOBAL Void TA1_init(Void)
{

    CLRBIT(TA1CTL, MC0 | MC1  // stop mode
           | TAIE// disable interrupt
           | TAIFG);// clear interrupt flag
    CLRBIT(TA1CCTL0, CM1 | CM0  // no capture mode
           | CAP// compare mode
           | CCIE// disable interrupt
           | CCIFG);// clear interrupt flag
    TA1CCR0 = 96 - 1;            // set up Compare Register
    TA1EX0 = TAIDEX_7;        // set up expansion register
    TA1CTL = TASSEL__ACLK     // 614.4 kHz
    | MC__UP           // Up Mode
            | ID__8            // /8
            | TACLR;           // clear and start Timer
    SETBIT(TA1CTL, TAIE | TAIFG);       // enable interrupt
}

#pragma vector = TIMER1_A1_VECTOR
__interrupt Void TIMER1_A1_ISR(Void)
{

    /*
     * Der Inhalt der ISR ist zu implementieren
     */

    //btn1
    if (TSTBIT(P1IN, BIT0))
    {
        if (count1 < 5)
        {
            count1++;
        }
    } else {
        if (count1 > 0) {
            count1--;
        }
    }

    if (!state1)
    {
        if (count1 == 5)
        {
            state1 = 1;
            // send button event
            Event_set(EVENT_1);       // set up event
        }
    }
    else
    {
        if (count1 == 0)
        {
            state1 = 0;
        }
    }

    //btn2
        if (TSTBIT(P1IN, BIT1))
        {
            if (count2 < 5)
            {
                count2++;
            }
        } else {
            if (count2 > 0) {
                count2--;
            }
        }

        if (!state2)
        {
            if (count2 == 5)
            {
                state2 = 1;
                // send button event
                Event_set(EVENT_2);       // set up event
            }
        }
        else
        {
            if (count2 == 0)
            {
                state2 = 0;
            }
        }

    CLRBIT(TA1CTL, TAIFG); // clear interrupt flag
    __low_power_mode_off_on_exit(); // restore Active Mode on return
}
