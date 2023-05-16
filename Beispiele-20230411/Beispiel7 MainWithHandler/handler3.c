#include <msp430.h>
#include "..\base.h"
#include "handler2.h"
#include "handler3.h"
#include "event.h"

#define VERSION 6

#pragma FUNC_ALWAYS_INLINE(swap)
LOCAL Void swap(Char *arg1, Char *arg2) {
   Char ch = *arg2;
   *arg2 = *arg1;
   *arg1 = ch;
}

#if (VERSION EQ 1)
LOCAL Void  sort(Char * arr, Int n) {
   UInt i, j;
   for (i = 0; i LT n-1; i++) {
      for (j = 0; j LT n-i-1; j++) {
         if (arr[j] GT arr[j+1]) {
            swap(&arr[j], &arr[j+1]);
         }
      }
   }
}
#elif (VERSION EQ 2)
// Übergang von for-Schleifen auf while-Schleifen
LOCAL Void  sort(Char * arr, Int n) {
   UInt i = 0;
   while (i LT n-1) {
      UInt j = 0;
      while (j LT n-i-1) {
         if (arr[j] GT arr[j+1]) {
            swap(&arr[j], &arr[j+1]);
         }
         j++;
      }
      i++;
   }
}
#elif (VERSION EQ 3)
// Übergang von Schleifen auf eine Zustandsmaschine
typedef enum {S0, S1, S2} TState;

LOCAL Void  sort(Char * arr, Int n) {
   UInt i, j;
   TState state = S0;
   while (1) {
     switch (state) {
        case S0:
           i = 0;
           state = S1;
           break;
        case S1:
           if (i LT n-1) {
              j = 0;
              state = S2;
           } else {
              state = S0;
              return;
           }
           break;
        case S2:
           if (j LT n-i-1) {
              if (arr[j] GT arr[j+1]) {
                 swap(&arr[j], &arr[j+1]);
              }
              j++;
           } else {
              i++;
              state = S1;
           }
           break;
        default:
           break;
     }
   }
}
#elif (VERSION EQ 4)

#endif

#if ((VERSION EQ 1) OR (VERSION EQ 2) OR (VERSION EQ 3))
GLOBAL Void Handler3_init() { }

GLOBAL Void Handler3(Void) {
   if (Event_tst(EVENT_RUN3)) {
      Event_clr(EVENT_RUN3);
      sort(tab, TABSIZE);
   }
}
#elif (VERSION EQ 4)
// Eliminierung der while-Schleife aus der Funktion
// durch die Erweiterung um Events

typedef enum {S0, S1, S2} TState;

LOCAL TState state;
LOCAL UInt i, j;

#pragma FUNC_ALWAYS_INLINE(Handler3_init)
GLOBAL Void Handler3_init() {
   state = S0;
}

GLOBAL Void Handler3(Void) {
   switch (state) {
      case S0:
         if (Event_tst(EVENT_RUN3)) {
            i = 0;
            state = S1;
         }
         break;
      case S1:
         if (i LT TABSIZE-1) {
            j = 0;
            state = S2;
         } else {
            Event_clr(EVENT_RUN3);
            state = S0;
         }
         break;
      case S2:
         if (j LT TABSIZE-i-1) {
            if (tab[j] GT tab[j+1]) {
               swap(&tab[j], &tab[j+1]);
            }
            j++;
         } else {
            i++;
            state = S1;
         }
         break;
      default:
         break;
   }
}

#elif (VERSION EQ 5)
// Eliminierung der switch-Anweisung
// jede Zustand ist eine separate Funktion
// die Auswahl eines Zustandes erfolgt über
// eine Tabelle mit Funktionspointern

#define TRUNSIZE  3
typedef enum {S0=0, S1, S2} TState;

// lokale Zustandsvariablen
LOCAL TState state;
LOCAL UInt i, j;

#pragma FUNC_ALWAYS_INLINE(Handler3_init)
GLOBAL Void Handler3_init() {
   state = S0;
}

LOCAL Void State0(Void) {
   if (Event_tst(EVENT_RUN3)) {
      i = 0;
      state = S1;
   }
}

LOCAL Void State1(Void) {
   if (i LT TABSIZE-1) {
      j = 0;
      state = S2;
   } else {
      Event_clr(EVENT_RUN3);
      state = S0;
   }
}

LOCAL Void State2(Void) {
   if (j LT TABSIZE-i-1) {
      if (tab[j] GT tab[j+1]) {
         swap(&tab[j], &tab[j+1]);
      }
      j++;
   } else {
      i++;
      state = S1;
   }
}

// Datentyp eines konstanten Funktionspointers
typedef Void (* const VoidFunc)(Void);

// Tabelle mit konstanten Funktionspointern
LOCAL const VoidFunc run[TRUNSIZE] = {State0,
                                      State1,
                                      State2};

GLOBAL Void Handler3(Void) {
   if (state LT TRUNSIZE) {
      run[state]();
   }
}
#elif (VERSION EQ 6)

// Datentyp eines Funktionspointers
typedef Void (* VoidFunc)(Void);

LOCAL Void State0(Void);
LOCAL Void State1(Void);
LOCAL Void State2(Void);

// lokale Zustandsvariablen
LOCAL VoidFunc  state;
LOCAL UInt i, j;

#pragma FUNC_ALWAYS_INLINE(Handler3_init)
GLOBAL Void Handler3_init() {
   state =  State0;
}

LOCAL Void State0(Void) {
   if (Event_tst(EVENT_RUN3)) {
      i = 0;
      state = State1;
   }
}

LOCAL Void State1(Void) {
   if (i LT TABSIZE-1) {
      j = 0;
      state = State2;
   } else {
      Event_clr(EVENT_RUN3);
      state = State0;
   }
}

LOCAL Void State2(Void) {
   if (j LT TABSIZE-i-1) {
      if (tab[j] GT tab[j+1]) {
         swap(&tab[j], &tab[j+1]);
      }
      j++;
   } else {
      i++;
      state = State1;
   }
}

GLOBAL Void Handler3(Void) {
   (*state)();
}

#endif
