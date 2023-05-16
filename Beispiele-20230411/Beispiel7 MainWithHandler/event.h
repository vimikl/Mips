#include "..\base.h"

#ifndef EVENT_H_
#define EVENT_H_

typedef unsigned int TEvent;

#define NO_EVENTS   0x0000
#define EVENT_1     0x0001
#define EVENT_2     0x0002
#define EVENT_3     0x0004
#define EVENT_4     0x0008
#define EVENT_5     0x0010
#define EVENT_6     0x0020
#define EVENT_7     0x0040
#define EVENT_8     0x0080
#define EVENT_9     0x0100
#define EVENT_10    0x0200
#define EVENT_11    0x0400
#define EVENT_12    0x0800
#define EVENT_13    0x1000
#define EVENT_14    0x2000
#define EVENT_15    0x4000
#define EVENT_16    0x8000
#define ALL_EVENTS  0xFFFF

#define EVENT_RUN1  EVENT_1  //
#define EVENT_RUN2  EVENT_2  //
#define EVENT_RUN3  EVENT_3  //

EXTERN Void Event_init(Void);
EXTERN Void Event_wait(Void);
EXTERN Void Event_set(TEvent);
EXTERN Void Event_clr(TEvent);
EXTERN Bool Event_tst(TEvent);
EXTERN Bool Event_err(Void);

#endif /* EVENT_H_ */
