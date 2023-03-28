#include <genesis.h>


u32 events[256] = {0};

bool get_event(u16 event_id) {
	u32 *event_ptr = events + (event_id >> 5);
	return (*(event_ptr) >> (event_id & 0x1F)) != 0;
}

void set_event(u16 event_id) {
	u32 *ptr_to_event = events + (event_id >> 5);
	*(ptr_to_event) |= (1 << (event_id & 0X1F));
}

void clr_event(u16 event_id) {
	u32 *ptr_to_event = events + (event_id >> 5);
	*(ptr_to_event) &= ~(1 << (event_id & 0X1F));
}
