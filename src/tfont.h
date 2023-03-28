#ifndef TFONT_H_
#define TFONT_H_

#include <genesis.h>

#define ESCAPE_CHAR 0xFF


/* Struct to display a Variable Width Font. 

	A font can contain up to 65536 characters. 
	Each character is 8x8 pixels, and is coded with 8 bytes (1 bit = 1 pixel)
	Bitmap data is stored as consecutive COLUMNS (and not lines) of pixels
	
	width	:	first multiple of 8 >= maximum width of a character
				For the moment, only 8 is supported
	
	height	:	first multiple of 8 >= maximum height of a character
				For the moment, only 8 is supported
	
	data	:	an array of 8*N u8 (for N characters), 
				coding the bitmap data of each character

	widths	:	an array of N u8, coding the width of each character
	
	space	:	the code of the space character. 
				Must be in the range 0-0xFE
				If the code is 0xXY, then all characters with codes 0xMNXY
				must be spaces too.
				For example, if space = 0x20, then characters 0x120, 0x220, etc.
				are spaces.
*/

typedef struct TFont {
	u8 width;
	u8 height;
	const u8* data;
	const u8* widths;
	u8 space;
} TFont;

u16 get_word_width(const TFont *font, u16 font_plane, const u8 *text);

#endif