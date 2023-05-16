#include <msp430.h>
#include "..\base.h"
#include "event.h"
#include "TA0.h"
#include "UCA1.h"

// Datentyp eines Funktionspointers
typedef Void (* VoidFunc)(Void);

LOCAL Void State0(Void);
LOCAL Void State1(Void);

// lokale Zustandsvariablen
LOCAL VoidFunc state;
LOCAL UInt idx;
LOCAL UInt tmp;

#pragma FUNC_ALWAYS_INLINE(Handler1_init)
GLOBAL Void Handler1_init(Void) {
   state = State0;
}

#pragma FUNC_ALWAYS_INLINE(Handler1)
GLOBAL Void Handler1(Void) {
   (*state)();
}

LOCAL Void State0(Void) {
   if (Event_tst(EVENT_UPDATE)) {
      Event_clr(EVENT_UPDATE);
      tmp = counter;
      idx = 1;
      state = State1;
      Event_set(EVENT_DONE);
   }
}

LOCAL Void State1(Void) {
   if (Event_tst(EVENT_DONE)) {
      Event_clr(EVENT_DONE);
      if (idx LE 4) {
         UChar ch = 0x0F BAND tmp;
         ch += '0';
         UCA1_emit(idx, ch);
         tmp >>= 4;
         idx++;
      } else {
         state = State0;
      }
   }
}


