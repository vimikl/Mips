#include <msp430.h>
#include "..\base.h"
#include "handler1.h"
#include "event.h"

// Application Note 4400
// Pseudo Random Number Generation Using Linear Feedback Shift Registers
// Quelle: https://www.maximintegrated.com/en/app-notes/index.mvp/id/4400

#define POLY_MASK_32 0xB4BCD35C
#define POLY_MASK_31 0x7A5BC2E3

GLOBAL Int   rnd_value;
LOCAL  ULong lfsr32;
LOCAL  ULong lfsr31;

#pragma FUNC_ALWAYS_INLINE(Handler1_init)
GLOBAL Void Handler1_init(Void) {
   rnd_value = 0;
   lfsr32 = 0xABCDE;
   lfsr31 = 0x23456789;
}

LOCAL Void shift_lfsr(ULong *lfsr, ULong mask) {
   Int feedback = *lfsr BAND 0x1;
   *lfsr >>= 1;
   if (feedback EQ 1) {
      *lfsr ^= mask;
   }
}

#pragma FUNC_ALWAYS_INLINE(get_random)
LOCAL Int get_random(Void) {
   shift_lfsr(&lfsr32, POLY_MASK_32);
   shift_lfsr(&lfsr32, POLY_MASK_32);
   shift_lfsr(&lfsr31, POLY_MASK_31);
   return (lfsr32 XOR lfsr31) BAND 0xFFFF;
}

GLOBAL Void Handler1(Void) {
   if (NOT Event_tst(EVENT_RUN1))
      return;

   Event_clr(EVENT_RUN1);
   rnd_value = get_random();
   Event_set(EVENT_RUN2);
}
