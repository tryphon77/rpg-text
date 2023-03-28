#include <genesis.h>
#include <kdebug.h>

#include "tfont.h"
#include "events_manager.h"
#include "tile_text_renderer.h"

#include "font8_tfont.h"
#include "paseo_dialog.h"
#include "gfx.h"


#define escape_char 0xFF
#define TEXTBOX_TYPE_ID 42


u16 ind;
u16 timer;
u16 joy;

u32 tile_to_ship[8];
TileTextRenderer text_renderer;

u16 tbox_tiles;


/* scenes can be started inside dialogs (using [load(x)] commaand) 
   They are stored in an array of pointer to functions */

void scene_black() {
	kprintf("scene_black");
	PAL_setColors(48, (u16*) palette_black, 16, CPU);
}
	
void scene_intro_1() {
	kprintf("scene_intro_1");
	
	u16 image_index = ind;
	VDP_loadTileSet(intro_1.tileset, ind, DMA);
	ind += intro_1.tileset->numTile;

	// This one is not packed
	TileMap *tmap = intro_1.tilemap;

	// draw backgrounds
	VDP_setTileMapEx(
		BG_A, 
		tmap, 
		TILE_ATTR_FULL(PAL3, FALSE, FALSE, FALSE, image_index), 
		6, 2,  
		0, 0, 28, 14, 
		CPU
	);

	PAL_setColors(48, (u16*) intro_1.palette->data, 16, CPU);
}	

void scene_intro_2() {
	kprintf("scene_intro_2");
	
	u16 image_index = ind;
	VDP_loadTileSet(intro_2.tileset, ind, DMA);
	ind += intro_2.tileset->numTile;

	// This one is not packed
	TileMap *tmap = intro_2.tilemap;

	// draw backgrounds
	VDP_setTileMapEx(
		BG_A, 
		tmap, 
		TILE_ATTR_FULL(PAL3, FALSE, FALSE, FALSE, image_index), 
		6, 2,  
		0, 0, 28, 14, 
		CPU
	);

	PAL_setColors(48, (u16*) intro_2.palette->data, 16, CPU);
}	

void (*scenes[])() = {
	scene_black,
	scene_intro_1,
	scene_intro_2
};


/* TextBox is an object representing the dialog box at the bottom of the screen
   It inherits from the SGDK Object struct. */
   
typedef struct {
	Object; // parent struct
	s16 left; // rect of the box, exprimed in tiles coordinates
	s16 top; // relative to the plane
	u16 width;
	u16 height;
	
	u16 plane;
	
	TileTextRenderer *renderer; // Text renderer

	u8 timer; // for pause
} TextBox;

TextBox textbox;


/* update functions fot the textbox */

// normal update : one letter at a time
void textbox_update_display(Object *obj) {
	TextBox *tbox = (TextBox *) obj;
	TTR_next_letter(tbox->renderer);
}

// when [wait] is encountered : 
// wait for the C buttion to be pressed
void textbox_update_wait(Object *obj) {
	TextBox *tbox = (TextBox *) obj;
	if (joy & BUTTON_C) {
		tbox->update = textbox_update_display;
	}
}

// when [pause] is encountered
void textbox_update_pause(Object *obj) {
	TextBox *tbox = (TextBox *) obj;
	if (tbox->timer-- == 0) {
		tbox->update = textbox_update_display;
	}
}


/* commands to execute when the TilerTextRenderer
   callback function is triggered */

// when [set(event)] is triggered
// set event to TRUE
void CMD_set(TileTextRenderer *renderer) {
	kprintf("renderer_set");
	
	u16 event_id = TTR_get_next_word(renderer);
	kprintf("event_id=%d get_event=%d", event_id, get_event(event_id));
	
	set_event(event_id);
	kprintf("event_id=%d get_event=%d", event_id, get_event(event_id));
}

// when [clr(event) is triggered
// set event to FALSE
void CMD_clr(TileTextRenderer *renderer) {
	kprintf("renderer_set");
	
	u16 event_id = TTR_get_next_word(renderer);
	kprintf("event_id=%d get_event=%d", event_id, get_event(event_id));
	
	clr_event(event_id);
	kprintf("event_id=%d get_event=%d", event_id, get_event(event_id));
}

// when [load(scene)] is triggered
// loads the corresponding scene
void CMD_load(TileTextRenderer *renderer) {
	u8 scene_id = TTR_get_next_byte(renderer);
	scenes[scene_id]();
	kprintf("load scene %d", scene_id);
}

// when [if(condition, n)] is triggered
// skip the n following bytes of the dialog
// if the condition is satisfied
void CMD_if(TileTextRenderer *renderer) {
	kprintf("renderer_if");
	
	u8 branch = TRUE;
	while (TRUE) {
		u16 condition = TTR_get_next_word(renderer);
		kprintf("condition=%04X", condition);
		if (condition == 0xFFFF) break;
		
		if (!get_event(condition)) branch = FALSE;
	}

	u16 dest = TTR_get_next_word(renderer);
	kprintf("dest=%04X", dest);

	if (branch) {
		TTR_advance_by(renderer, dest);
		kprintf("branch to %p", renderer->text);
	}
}

// when [goto(n)] is triggered
// skip the n following bytes of the dialog
void CMD_goto(TileTextRenderer *renderer) {
}

// callback function for the TilerTextRenderer
void tbox_renderer_callback(TileTextRenderer *renderer, u16 cmd_id) {
	switch (cmd_id) {
		case 0:
			TTR_new_line(renderer);
			break;
		case 1:
			TTR_set_font_plane(renderer);
			break;
		case 2:
			textbox.update = NULL;
			break;
		case 3:
			textbox.update = textbox_update_wait;
			break;
		case 4:
			textbox.timer = 120;
			textbox.update = textbox_update_pause;
			break;
		case 5:
			break;
		case 6:
			CMD_load(renderer);
			break;
		case 7:
			TTR_set_fg_color(renderer, 1);
			break;
		case 8:
			TTR_set_fg_color(renderer, 1);
			break;
		case 9:
			TTR_set_fg_color(renderer, 4);
			break;
		case 10:
			TTR_set_fg_color(renderer, 4);
			break;
		case 11:
			TTR_set_fg_color(renderer, 4);
			break;
		case 12:
			CMD_if(renderer);
			break;
		case 13:
			CMD_goto(renderer);
			break;
		case 14:
			CMD_set(renderer);
			break;
		case 15:
			CMD_clr(renderer);
			break;
		default:
			break;
	}
}


/* initializes the TextBox fields (but doesn't display it) */
void textbox_init(TextBox *tbox, s16 left, s16 top, s16 width, u16 height) {
	tbox->type = TEXTBOX_TYPE_ID;
	tbox->left = left;
	tbox->top = top;
	tbox->width = width;
	tbox->height = height;
	
	tbox->plane = BG_A;
	
	ind += TTR_init(
		&text_renderer, 
		&font8, 
		tile_to_ship, 
		ind, 
		BG_A,
		left + 1, top + 1, width - 2, height - 2,
		paseo_data + intro_french
	);

	text_renderer.callback = tbox_renderer_callback;
	tbox->renderer = &text_renderer;
}

/* display the TextBox */
void textbox_display(TextBox *tbox) {
	VDP_setTileMapXY(tbox->plane, tbox_tiles + 0, tbox->left, tbox->top);
	VDP_setTileMapXY(tbox->plane, tbox_tiles + 2, tbox->left + tbox->width - 1, tbox->top);
	VDP_setTileMapXY(tbox->plane, tbox_tiles + 7, tbox->left, tbox->top + tbox->height - 1);
	VDP_setTileMapXY(tbox->plane, tbox_tiles + 9, tbox->left + tbox->width - 1, tbox->top + tbox->height - 1);
	
	for (u16 y = 1 ; y < tbox->height - 1 ; y++) {
		VDP_setTileMapXY(tbox->plane, tbox_tiles + 4, tbox->left, tbox->top + y);
		VDP_setTileMapXY(tbox->plane, tbox_tiles + 6 , tbox->left + tbox->width - 1, tbox->top + y);
	}
		
	for (u16 x = 1 ; x < tbox->width - 1; x++) {
		VDP_setTileMapXY(tbox->plane, tbox_tiles + 1, tbox->left + x, tbox->top);
		VDP_setTileMapXY(tbox->plane, tbox_tiles + 8, tbox->left + x, tbox->top + tbox->height - 1);
	}
	
	tbox->update = textbox_update_display;
}



int main(bool hard) {
	ind = TILE_USER_INDEX;

	// load tiles of the edges of the dialog box
	tbox_tiles = ind;
	VDP_loadTileSet(&gui, ind, CPU);
	ind += gui.numTile;
		
    // initialization
    VDP_setScreenWidth320();
	VDP_setScreenHeight224();

    PAL_setColors(0, palette_A.data, 16, CPU);
	
    // just to monitor frame CPU usage
    SYS_showFrameLoad(TRUE);

	// initialize and display TextBox
	textbox_init(&textbox, 2, 20, 36, 6);
	textbox_display(&textbox);
	
    while(TRUE) {
		joy = JOY_readJoypad(JOY_1);

		if (textbox.update) {
			textbox.update((Object*) &textbox) ;
		}

        SYS_doVBlankProcess();
    }

    return 0;
}
