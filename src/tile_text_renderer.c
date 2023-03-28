#include <genesis.h>

#include "tile_text_renderer.h"
#include "tfont.h"


// future
static void buffer_to_vdp(TileTextRenderer*);
static void draw_tilemap(TileTextRenderer*, u16);


static void clear_buffer(TileTextRenderer *renderer) {	
	u32 *dest = renderer->buffer; // tile_to_ship;
	for (u16 i = 0 ; i < 8 ; i++) {
		*(dest++) = renderer->bg_row;
	}
}

static void ship_and_clear_buffer(TileTextRenderer *renderer) {
	buffer_to_vdp(renderer);
	renderer->vpos++;
	if (renderer->vpos >= renderer->max_vpos) renderer->vpos = renderer->base_vpos;
	clear_buffer(renderer);
}	

static void char_to_buffer(TileTextRenderer *renderer, u16 chr) { // Font *font, u16 chr, u16 fg, u16 preshift) {
	chr |= renderer->font_plane;
	const TFont *font = renderer->font;
	const u8 *src = font->data + 8*chr;
	u32 *dest = renderer->buffer; // tile_to_ship;
	u16 preshift = renderer->preshift;
	
	u32 mask = ~(0xF << ((7  - preshift)*4));
	u32 fg1 = renderer->fg_color << ((7 - preshift)*4);

	u8 width = font->widths[chr];
	renderer->pixel_x += width;
	
	for (u16 i = 0 ; i < width ; i++) {
		u8 data = *(src++);

		for (u16 j = 0 ; j < 8 ; j++) {
			if (data & 0x80) {
				*dest &= mask;
				*dest |= fg1; 
			}
			dest++;			
			data <<= 1;
			}
		fg1 >>= 4;
		mask = 0xF0000000 | (mask >> 4);
		renderer->preshift++;
		renderer->preshift &= 7;
		
		if (~mask == 0) {
			ship_and_clear_buffer(renderer);
			mask = ~0xF0000000;
			fg1 = renderer->fg_color << 28;
		}
		dest -= 8;
	}
}

void TTR_space(TileTextRenderer *renderer, u16 space) {
	renderer->preshift += space;
	while (renderer->preshift > 7) {
		ship_and_clear_buffer(renderer);
		renderer->preshift -= 8;
	}
}

static void buffer_to_vdp(TileTextRenderer *renderer) {
	VDP_loadTileData(renderer->buffer, renderer->vpos, 1, CPU);
}	

static void draw_tilemap(TileTextRenderer *renderer, u16 nrow) {
	u16 top = renderer->top;	
	u16 left = renderer->left;
	u16 width = renderer->width;
	u16 height = renderer->height;
	
	u16 vpos = renderer->base_vpos + nrow * renderer->width;
	
	for (u16 y = 0 ; y < height ; y++) {
		for (u16 x = 0 ; x < width ; x++) {
			VDP_setTileMapXY(renderer->plane, vpos++, left + x, top + y);
		}
		if (vpos >= renderer->max_vpos) vpos = renderer->base_vpos;
	}
}

static void clear_row(TileTextRenderer *renderer, u16 nrow) {
	vu32 *pl = (u32*) VDP_CTRL_PORT;
	*pl = VDP_WRITE_VRAM_ADDR((renderer->base_vpos + nrow*renderer->width) << 5);	
	vu32 *pw = (u32*) VDP_DATA_PORT;

	for (u16 x = 0 ; x < renderer->width ; x++) {
		*pw = renderer->bg_row;
		*pw = renderer->bg_row;
		*pw = renderer->bg_row;
		*pw = renderer->bg_row;
		*pw = renderer->bg_row;
		*pw = renderer->bg_row;
		*pw = renderer->bg_row;
		*pw = renderer->bg_row;
	}
}	

u16 TTR_init(
	TileTextRenderer *renderer, 
	const TFont *font, 
	u32* buffer, 
	u16 vpos, 
	VDPPlane plane, 
	u16 left, u16 top, u16 width, u16 height, 
	const u8 *text
) {

	u16 numTile = 0;
	
	renderer->font = font;
	renderer->buffer = buffer;
	renderer->base_vpos = vpos;
	renderer->vpos = vpos;
	renderer->font_plane = 0;
	renderer->preshift = 0;
	renderer->fg_color = 1;
	renderer->bg_color = 11;
	renderer->text = text;
	renderer->new_word = TRUE;
	
	renderer->top = top;
	renderer->left = left;
	renderer->width = width;
	renderer->height = height;
	
	renderer->pixel_x = 0;
	renderer->pixel_width = width << 3;
	
	renderer->current_line_in_buffer = 0;
	renderer->first_line_in_tilemap = 0;
	renderer->is_scrolling = FALSE;
		
	u32 row = (renderer->bg_color << 4) | (renderer->bg_color);
	row |= (row << 8);
	row |= (row << 16);
	renderer->bg_row = row;

	renderer->callback = NULL;
	
//	renderer->update = update_display;
	
	for (u16 y = 0 ; y < height ; y++) {
		for (u16 x = 0 ; x < width ; x++) {
			VDP_setTileMapXY(plane, vpos++, left + x, top + y);
		}
	}
	renderer->max_vpos = vpos;

	vu32 *pl = (u32*) VDP_CTRL_PORT;
	*pl = VDP_WRITE_VRAM_ADDR(renderer->vpos << 5);	
	vu32 *pw = (u32*) VDP_DATA_PORT;
	
	for (u16 y = 0 ; y < height ; y++) {
		for (u16 x = 0 ; x < width ; x++) {
			*pw = renderer->bg_row;
			*pw = renderer->bg_row;
			*pw = renderer->bg_row;
			*pw = renderer->bg_row;
			*pw = renderer->bg_row;
			*pw = renderer->bg_row;
			*pw = renderer->bg_row;
			*pw = renderer->bg_row;
			numTile++;
		}
	}
	
	clear_buffer(renderer);
	
	return numTile;
}

void TTR_new_line(TileTextRenderer *renderer) {
	kprintf("new_line()");
	renderer->pixel_x = 0;
	renderer->preshift = 0;

	renderer->current_line_in_buffer++;
	if (renderer->current_line_in_buffer >= renderer->height) {
		renderer->current_line_in_buffer = 0;
		renderer->is_scrolling = TRUE;
	}
	renderer->vpos = renderer->base_vpos + renderer->current_line_in_buffer * renderer->width;

	if (renderer->is_scrolling) {
		renderer->first_line_in_tilemap++;
		if (renderer->first_line_in_tilemap >= renderer->height) {
			renderer->first_line_in_tilemap = 0;
		}
		draw_tilemap(renderer, renderer->first_line_in_tilemap);
		clear_row(renderer, renderer->current_line_in_buffer);
	}

	clear_buffer(renderer);
}

void TTR_next_letter(TileTextRenderer *renderer) {
	u8 chr_id;
	u16 width = get_word_width(renderer->font, renderer->font_plane, renderer->text);
	
	if (renderer->new_word) {
		kprintf("pixel_x=%d, new word width=%d, width=%d", renderer->pixel_x, width, renderer->pixel_width);
		if (renderer->pixel_x + width > renderer->pixel_width) TTR_new_line(renderer);
		renderer->new_word = FALSE;
	}
	
	chr_id = *(renderer->text++);
	kprintf("read: %02X at %p", chr_id, renderer->text-1);
	if (chr_id == ESCAPE_CHAR) {
		chr_id = *(renderer->text++);
		renderer->callback(renderer, chr_id);
	}
	else {
		if (chr_id == renderer->font->space) renderer->new_word = TRUE;
		char_to_buffer(renderer, chr_id) ; // &font8, i, 15, p);
		buffer_to_vdp(renderer);
	}
}


void TTR_set_font_plane(TileTextRenderer *renderer) {
	kprintf("renderer_font_plane");
	renderer->font_plane = *(renderer->text++) << 8;
	kprintf("font_plane = %d", renderer->font_plane);
}

void TTR_set_callback(TileTextRenderer *renderer, RendererCallback callback) {
	renderer->callback = callback;
}

u8 TTR_get_next_byte(TileTextRenderer *renderer) {
	return *(renderer->text++);
}

u16 TTR_get_next_word(TileTextRenderer *renderer) {
	u16 res;
	
	res = *(renderer->text++);
	res <<= 8;
	res |= *(renderer->text++);
	return res;
}

void TTR_advance_by(TileTextRenderer *renderer, s16 step) {
	renderer->text += step;
}

void TTR_set_fg_color(TileTextRenderer *renderer, u8 color) {
	renderer->fg_color = color;
}
