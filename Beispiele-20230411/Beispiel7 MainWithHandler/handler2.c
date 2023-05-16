#include <msp430.h>
#include "..\base.h"
#include "handler1.h"
#include "handler2.h"
#include "event.h"

LOCAL  Char table[TABSIZE];
LOCAL  UInt idx;
GLOBAL Char * const tab = table;

#pragma FUNC_ALWAYS_INLINE(Handler2_init)
GLOBAL Void Handler2_init(Void) {
   idx = 0;
}

GLOBAL Void Handler2(Void) {
   if (Event_tst(EVENT_RUN2))
      return;

   Event_clr(EVENT_RUN2);
   Char ch = (Char)rnd_value;
   if (NOT between('A', ch, 'Z'))
      return;
   table[idx++] = ch;
   if (idx EQ TABSIZE) {
      idx = 0;
      Event_set(EVENT_RUN3);
   }
}
