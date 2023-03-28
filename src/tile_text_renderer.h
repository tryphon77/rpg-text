#ifndef TILE_TEXT_RENDERER_H_
#define TILE_TEXT_RENDERER_H_

#include <genesis.h>

#include "tfont.h"

typedef struct TileTextRenderer_ TileTextRenderer;
typedef void (*RendererCallback)(TileTextRenderer *, u16);

/* TileTextRenderer : object rendering a dialog string on screen, using a plane

	font 		:	pointer to the 8x8 bitmap font used to render the text

	buffer		:	pointer to an array of 32 bytes in RAM, used for building 
					the current character

	text		:	pointer to the next byte of the string to analyze
					(initialized at the beginning of the string)

	base_vpos	:	address in VRAM of the 1st pattern of the rendered text
					(with tiles attributes)

	vpos		:	(internal) address in VRAM of the current pattern rendered

	max_vpos	:	(internal) last address in VRAM of the rendered text

	font_plane	:	MSB of the character code 
					if 0x00 then characters 0x00 to 0xFF can be used
					if 0x01 then characters 0x100 to 0x1FF can be used
					and so on

	preshift	:	nb of pixels between the left edge of current tile 
					and left edge of currently rendere character

	plane		:	plane to drawn onto (BG_A or BG_B)

	left		:	left position of the renderer (in tiles unit in the plane)

	top			:	top position of the renderer (in tiles unit in the plane)

	width		:	width of the renderer (in tiles unit)

	height		:	height of the renderer (in tiles unit)

	pixel_width	:	(internal) width of the renderer in pixels

	pixel_x		:	(internal) position of the left edge of next letter in pixels

	current_line_in_buffer	:	line number of the current line in the buffer
								(increases each new line of text, then cycles)
	
	first_line_in_tilemap	:	line number of the first line in the tilemap 
								(increases when text is scrolling, then cycles)

	is_scrolling:	TRUE if scrolling needed before the next line 

	fg_color	:	foreground color

	bg_color	:	background_color

	bg_row		:	(internal) a row of 8 pixels in the background color

	new_word	:	TRUE if next letter starts a new word

	callback	:	callback function for command in dialog
*/
	
typedef struct TileTextRenderer_ {
	const TFont* font;
	u32* buffer;
	const u8 *text;
	u16 base_vpos;
	u16 vpos;
	u16 max_vpos;
	u16 font_plane;
	u8 preshift;
	VDPPlane plane;
	u16 left;
	u16 top;
	u16 width;
	u16 height;
	u16 pixel_width;
	u16 pixel_x;
	u8 current_line_in_buffer; // id of the current buffer block (!= first_line_in_tilemap)
	u8 first_line_in_tilemap; // position on plane of the current line
	u8 is_scrolling;
	u8 fg_color;
	u8 bg_color;
	u32 bg_row;
	u8 new_word;
	RendererCallback callback;
} TileTextRenderer;


void TTR_space(TileTextRenderer *renderer, u16 space);
u16 TTR_init(TileTextRenderer *renderer, const TFont *font, u32* buffer, u16 vpos, VDPPlane plane, u16 left, u16 top, u16 width, u16 height, const u8 *text);
void TTR_new_line(TileTextRenderer *renderer);
void TTR_next_letter(TileTextRenderer *renderer);


void TTR_set_font_plane(TileTextRenderer *renderer);
u8 TTR_get_next_byte(TileTextRenderer *renderer);
u16 TTR_get_next_word(TileTextRenderer *renderer);
void TTR_advance_by(TileTextRenderer *renderer, s16 step);
void TTR_set_fg_color(TileTextRenderer *renderer, u8 color);

#endif
