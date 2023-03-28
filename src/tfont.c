#include <genesis.h>

#include "tfont.h"


u16 get_word_width(const TFont *font, u16 font_plane, const u8 *text) {
	u16 chr;
	u16 width = 0;
	
	while ((chr = *(text++))) {
		if ((chr == font->space) || (chr == ESCAPE_CHAR)) break;
		chr |= font_plane;
		width += font->widths[chr];
	}
	
	return width;
}
