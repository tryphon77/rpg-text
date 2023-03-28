#ifndef EVENTS_MANAGER_H_
#define EVENTS_MANAGER_H_

#include <genesis.h>

/* events is a array of booleans (1024 by default)
   the array is stored as an array of 1024/4 = 256 u32
   events[0] are boolean 0 to 31, and so on */

extern u32 events[];

/* give state of boolean nb. event_id */
bool get_event(u16 event_id);

/* set boolean nb. event_id to TRUE */
void set_event(u16 event_id);

/* set boolean nb. event_od to FALSE */
void clr_event(u16 event_id);

#endif
