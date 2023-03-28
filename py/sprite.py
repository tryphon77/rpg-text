# -*- coding: utf-8 -*-

from dataclasses import dataclass

from common import load_png, cvt_4x8bpp_to_9bpp, enlarge, crop, get_top_left, array_to_c, ptrns_to_array, list_of_int, tokenize
import numpy as np

def translate_by(rect_or_pos, delta):
	dx, dy = delta
	if len(rect_or_pos) == 2:
		x, y = rect_or_pos
		return x + dx, y + dy
	elif len(rect_or_pos) == 4:
		x, y, w, h = rect_or_pos
		return x + dx, y + dy, w, h
	
	raise Exception("Bad operand")



@dataclass
class Sheet:
	# image file
	image: np.array = None
	
	width: int = 0
	height: int = 0
	
	cell_width: int = 0
	cell_height: int = 0
	
	color_min: int = 256
	color_max: int = -1	

	palette: np.array = None
	

@dataclass
class TileSet:
	id: int = -1
	
	data:np.array = None
	
	rects: list = None
	
	patterns: list = None

	# position of the origin of the sprite. (0, 0) = top left
	hotspot: tuple = (0, 0)
	
	offset: int = 0
	

@dataclass
class Frame:
	"""A Frame is defined by a cell in the sheet, or described by the .res file"""
	
	# identifier (0 is top-left in the sheet)
	id: int = -1
	
	# size of the cell, by default the one given in the .red command line
	cellsize: tuple = ()
		
	# bitmap of the cell, enlarged by 8 pixels on each edge for splitting 
	data: np.array = None

	# the frame of which it's a copy, or None if it's a new frame
#	copy_of = None

	# id of the tileset it belongs to, -1 if not in a tileset (serves as base ?)
#	tileset_id: int = -1
	tileset: TileSet = None
	
	# True iff the data of the rects have been processed ?
	frame_processed: bool = False

	# list of rects compositing the frame. Beware : origin is at (8, 8) due to enlargement of the frame data
	rects: tuple = tuple([])

	# True iff frame is blank (2 situations : to be ignored, or to be computed from .res file)
	is_blank: bool = False
	
	# position of the origin of the sprite. (0, 0) = top left
	hotspot: tuple = (0, 0)

	# number of tiles in the frame
	num_tiles: int = 0

	# number of tiles in the frame
	num_sprites: int = 0

	# the frame that serves as a base for the transforms
	base = None
	 
	# list of str commands to transform the frame
	transforms: tuple = tuple([])
	
	exported: bool = False


@dataclass
class Transform:
	# type of transform (copy, hmirror, vmirror, hvmirror)
	cmd: str = ""
	
	# base frame
	base_frame: Frame = None
	
	# offsets
	dx: int = 0
	dy: int = 0


@dataclass
class VDPSprite:
	# id number of the VDPSprite
	id: int = -1

	# offset in VRAM
	# for dynamic sprite : offset from the 1st tile of the tileset of the current frame
	# for static sprite : offset from the 1st tile of the 1st tileset
	offset: int = -1
	
	# offset between the 1st tile of this VDPSprite in the current frame and the next VDPSprite
	# in the current frame 
	numTiles: int = -1
	
	# offset between the top left corner of the current VDPSprite and the hotspot of the frame
	offsetX: int = 0
	offsetY: int = 0
	
	# same thing when the frame is flipped
	offsetXFlip: int = 0
	offsetYFlip: int = 0
	
	# cell size in pixels
	width: int = 0
	height: int = 0
	
	# size flags wwhh as coded in the SAT
	size: int = 0

	# flip flags as coded in the VDPSprite struct
	flip: int = 0

@dataclass
class Animation:
	# animation name (used for export)
	name: str = ""
	
	# list of pairs (frame, timer) whare frame is a Frame, and timer is an int
	frames_times: tuple = ()
	
	# index of the looping frame in frames_times
	loop: int = 0

@dataclass
class SpriteDefinition:
	name: str = ""
	width: int = 0
	height: int = 0
	palette: tuple = ()
	animations: list = None
	max_num_tiles: int = 0
	max_num_sprites: int = 0
		
gl_rect_id = 0 # id of a rect in the entire tileset
def append_vdp_sprite(vdp_sprites, frame, rect, hflip = False, vflip = False, dx = 0, dy = 0):
	global gl_rect_id

	x, y, w, h = rect
#	print("VDPSprite.id=%d, .rect=%s, frame_id=%d, .width=%d, height=%d" % (gl_rect_id, rect, frame.id, w, h))

	# dims in tiles
	tw, th = w//8, h//8

	# offsets if not flipped
	offset_y = y + dy
	offset_x = x + dx
	
	# offsets if flipped
	offset_y_flipped = -offset_y - h
	offset_x_flipped = -offset_x - w
	
	vdp_sprite = VDPSprite(
		id = gl_rect_id,
		offset = frame.tileset.offset, 
		numTiles = 0,
		offsetY = offset_y_flipped if vflip else offset_y,
		offsetYFlip = offset_y if vflip else offset_y_flipped,
		width =  w,
		height = h,
		size = ((tw - 1) << 2) + (th - 1),
		flip = (hflip << 4) | (vflip << 5),
		offsetX = offset_x_flipped if hflip else offset_x,
		offsetXFlip = offset_x if hflip else offset_x_flipped
	)
	
	vdp_sprites.append(vdp_sprite)
	gl_rect_id += 1


def basic_sprite_cutter(frame, opt):
	h, w = frame.shape
	rects_ = []
	x0, y0 = get_top_left(frame)
	
	for y in range(y0, h, 32):
		for x in range(x0, w, 32):
			x1 = min(w, x + 32)
			y1 = min(h, y + 32)
			print(x, x1, w, y, y1, h)
			rects_.append((x, y, x1 - x, y1 - y))

	rects = []
	for r_ in rects_:
		r = crop(frame, r_)

		if r:
			# make sure the rect desn't go beyond the frame
			# it'd generate negative numbers for Flip offsets
			x_, y_, w_, h_ = r
			if x_ + w_ > w - 8:
				x_ = w - 8 - w_
			if y_ + h_ > h - 8:
				print(r, (w, h))
				y_ = h - 8 - h_
				print("-> ", (x_, y_, w_, h_))
			r = (x_, y_, w_, h_)
			rects.append(r)

	return rects

def hstrip_sprite_cutter(frame, opt):
	return []

def vstrip_sprite_cutter(frame, opt):
	return []
	
def grid_sprite_cutter(frame, opt):
	return []

def genetic_sprite_cutter(frame, opt):
	return []

def score_sprites(rects):
	return 8*len(rects)

def score_tiles(rects):
	val = 0
	for _, _, w, h in rects:
		val += w*h//2
	return val

def score_balanced(rects):
	return score_sprites(rects) + score_tiles(rects)


def find_best_solution(solutions, opt):
	score_fun = {
		"BALANCED": score_balanced,
		"TILES": score_tiles,
		"SPRITES": score_sprites
	}[opt]

	return sorted(solutions, key = lambda s: -score_fun(s))[0]

def get_sheet(args, base_dir):
	file_data = load_png("%s/%s" % (base_dir, args["path"].strip('"')))
	assert file_data["depth"] == 8
		
	palette = file_data["palette"]

	sheet_height, sheet_width = file_data["data"].shape
	file_data["data"] &= 15

	cell_width, cell_height = args["cellwidth"], args["cellheight"]
	
	# determine which colors to export as sprite.palette
	color_min = 256
	color_max = -1	

	for y in range(sheet_height):
		for x in range(sheet_width):
			c = file_data["data"][y, x]
			color_min = min(color_min, c)
			color_max = max(color_max, c)
	
	palette_24bpp = file_data["palette"][color_min : color_max + 1]
	palette = np.array([cvt_4x8bpp_to_9bpp(val) for val in palette_24bpp], dtype=np.uint16)
		
	return Sheet(file_data["data"], sheet_width, sheet_height, cell_width, cell_height, color_min, color_max, palette)

	
def get_palette_and_frames(sheet, args):
	frame_id = 0
	frames = []
	
	cell_width, cell_height = sheet.cell_width, sheet.cell_height
	for y in range(0, sheet.height, cell_height):
		frames_row = []
		for x in range(0, sheet.width, cell_width):
			data = enlarge(sheet.image[y : y + cell_height, x : x + cell_width], 8)
			if (data == 0).all():
				frame = Frame(
					id=frame_id,
					cellsize = (cell_width, cell_height),
					is_blank = True
				)
			else:
				for frame_ in frames:
					if (not frame_.is_blank) and (data == frame_.data).all():
						frame = frame_
						break
				else:
					frame = Frame(
						id=frame_id,
						cellsize = (cell_width, cell_height),
						data= data,
					)
				print("frame size:", frame.data.shape)
			
			frames.append(frame)
			frames_row.append(frame)

			frame_id += 1

	# mise à jour des kwargs spécifiques à chaque frame
	
	if "all" in args:
		for frame in frames:
			for k in args["all"]:
				setattr(frame, k, args["all"][k])
					  
	for frame_group in args:
		if type(frame_group) is tuple:
			attr = args[frame_group]
			for frame_id in frame_group:
				frame = frames[frame_id]
				for k in attr:
					if k == "cellsize":
						frame.cellsize = list_of_int(attr["cellsize"])
					else:
						setattr(frame, k, attr[k])
	
	# calcul du hotspot de chaque frame
	
	for frame in frames:
		cellwidth, cellheight = frame.cellsize
		
		if type(frame.hotspot) == str:
			hx, hy = frame.hotspot.strip("()").split(",")
			hx, hy = hx.strip(), hy.strip()
			if hx == "LEFT":
				hotspot_x = 0
			elif hx == "CENTER":
				hotspot_x = cellwidth//2
			elif hx == "RIGHT":
				hotspot_x = cellwidth
			else:
				hotspot_x = int(hx)
		
			if hy == "TOP":
				hotspot_y = 0
			elif hy == "CENTER":
				hotspot_y = cellheight//2
			elif hy == "BOTTOM":
				hotspot_y = cellheight
			else:
				hotspot_y = int(hy)
			frame.hotspot = (hotspot_x, hotspot_y)

	
		# parsing transform commands
		transforms = []
		for line in frame.transforms:
			line = tokenize(line)
			
			cmd = line[0]
			base_frame = frame
			dx, dy = 0, 0
			if len(line) > 1 and line[1] == "at":
				j = 2
			elif len(line) > 1:
				base_frame = frames[int(line[1])]
				j = 3
			
			if len(line) > j and line[j] == "(":
				dx, dy = int(line[j + 1]), int(line[j + 3])
			
			transforms.append(Transform(cmd, base_frame, dx, dy))
		
		if transforms:
			frame.is_blank = False
			frame.transforms = transforms
	
	return sheet.palette, frames


def get_animations(sheet, args, frames):
	if "animations" in args:
		animations = []

		for anim_name in args["animations"]:
			anim = args["animations"][anim_name]
			anim_frames = list_of_int(anim["frames"])
			anim_timings = list_of_int(anim["timings"])
			animations.append(Animation(
				name = anim_name,
				frames_times = list(zip([frames[f] for f in anim_frames[:-1]], anim_timings)),
				loop = anim["frames"][-1]
			))
			
	else:
		animations = []
		f_id = 0
		for j in range(sheet.nb_rows):
			anim_frames = []
			for i in range(sheet.nb_cols):
				frame = frames[f_id]
				if frame.is_blank:
					break
				anim_frames.append((frame, args["time"]))
				f_id += 1
			animations.append(Animation(
				name = "animation_%d_%d" % (i, j),
				frames_times = anim_frames,
				loop = 0
			))
	
	return animations


def get_tilesets(frames, generate_static_sprite):
	tilesets = []
	tileset_id = 0

	for frame in frames:
		if not frame.tileset:
			if frame.data is not None:
				tileset = TileSet(id=tileset_id, data=frame.data, hotspot=frame.hotspot, rects=frame.rects)
				tileset_id += 1
				print(frame.rects)
				tilesets.append(tileset)
				frame.tileset = tileset

			if frame.transforms:
				# il est possible qu'il y ait un tileset, ou pas
				tileset = frame.transforms[0].base_frame.tileset
				for t in frame.transforms:
					if not generate_static_sprite and t.base_frame.tileset != tileset:
						raise Exception("Only one base frame allowed for dynamic sprites")
				frame.tileset = tileset
	
	return tilesets


def generate_vdp_sprites(tilesets, algo, opt):
	for tileset in tilesets:
		hx, hy = tileset.hotspot
		if tileset.rects:
			tileset.rects = [eval(r) for r in tileset.rects] # [translate_by(eval(r), (8, 8)) for r in frame.rects]
		else:
			solutions = [basic_sprite_cutter(tileset.data, opt)]
			if algo & 1:
				solutions.append(hstrip_sprite_cutter(tileset.data, opt))
			if algo & 2:
				solutions.append(vstrip_sprite_cutter(tileset.data, opt))
			if algo & 4:
				solutions.append(grid_sprite_cutter(tileset.data, opt))
			if algo & 8:
				solutions.append(genetic_sprite_cutter(tileset.data, opt))
			
			sol = find_best_solution(solutions, opt)
			tileset.rects = [translate_by(r, (-8 - hx, -8 - hy)) for r in sol]
		
		generate_patterns(tileset)
			
def generate_patterns(tileset):
	ptrns = []
	hx, hy = tileset.hotspot

	num_tiles = 0

	for x, y, w, h in tileset.rects:
		num_tiles += w*h//64
		
		data = tileset.data[8 + hy + y : 8 + hy + y + h, 8 + hx + x : 8 + hx + x + w]
		for x_ in range(0, w, 8):
			for y_ in range(0, h, 8):
				ptrn = data[y_ : y_ + 8, x_ : x_ + 8].copy()
				assert ptrn.shape == (8, 8)
				ptrns.append(ptrn)

	tileset.patterns = ptrns
	tileset.num_tiles = num_tiles			
	tileset.num_sprites = len(tileset.rects)

def update_frames(frames, tilesets, generate_static_sprite):
	tileset_offset = 0
	for tileset in tilesets:
		tileset.offset = tileset_offset
		tileset_offset += sum([w*h//64 for _, _, w, h in tileset.rects])

	for frame in frames:
		frame.vdp_sprites = vdp_sprites = []

		hx, hy = frame.hotspot
		
		if generate_static_sprite:
			frame.num_tiles = 0

		if frame.data is not None:
			for rect in frame.tileset.rects:
				append_vdp_sprite(vdp_sprites, frame, rect)

		if frame.transforms:
			for t in frame.transforms:

				if t.cmd == "copy":
					for rect in t.base_frame.tileset.rects:
						append_vdp_sprite(vdp_sprites, t.base_frame, rect, dx = t.dx, dy = t.dy)

				elif t.cmd == "hmirror":
					for rect in t.base_frame.tileset.rects:
						append_vdp_sprite(vdp_sprites, t.base_frame, rect, hflip = True, dx = t.dx, dy = t.dy)

				elif t.cmd == "vmirror":
					for rect in t.base_frame.tileset.rects:
						append_vdp_sprite(vdp_sprites, t.base_frame, rect, vflip = True, dx = t.dx, dy = t.dy)

				elif t.cmd == "hvmirror":
					for rect in t.base_frame.tileset.rects:
						append_vdp_sprite(vdp_sprites, t.base_frame, rect, hflip = True, vflip = True, dx = t.dx, dy = t.dy)
				else:
					raise Exception("unrecognized command: %s" % t.cmd)

		frame.num_sprites = len(vdp_sprites)
		

def get_sprite_definitions(args, palette, frames, animations, tilesets):
	# création du SpriteDefinition	

	max_num_tiles = 0
	for tileset in tilesets:
		max_num_tiles = max(tileset.num_tiles, max_num_tiles)

	max_num_sprites = 0
	for frame in frames:
		max_num_sprites = max(frame.num_sprites, max_num_sprites)
	
	res = SpriteDefinition(
		name = args["name"],
		width = args["cellwidth"],
		height = args["cellheight"],
		palette = palette,
		animations = animations,
		max_num_tiles = max_num_tiles,
		max_num_sprites = max_num_sprites		
	)
	
	return res


def export_tilesets(output, name, tilesets):
	for tileset in tilesets:
		output.append(array_to_c("%s_tileset_%d_data" % (name, tileset.id), "const u32", ptrns_to_array(tileset.patterns)))
		output.append("")

	# export des tilesets
		
	for tileset in tilesets:
		output.append("const TileSet %s_tileset_%d = {" % (name, tileset.id))
		output.append("\t0, // compression")
		output.append("\t%d, // numTiles" % len(tileset.patterns))
#			output.append("\t(u32*) %s_frame_%d_tileset_data" % (name, frame.frame_id))
		output.append("\t(u32*) %s_tileset_%d_data" % (name, tileset.id))
		output.append("};")
		
		output.append("")


def export_palette(output, name, palette):
	output.append(array_to_c("%s_sprite_palette_data" % name, "const u16", palette))

	output.append("const Palette %s_sprite_palette = {" % name)
	output.append("\t%d, // numColors" % len(palette))
	output.append("\t(u16*) %s_sprite_palette_data" % name)
	output.append("};")
	
	output.append("")


def export_animation_frames(output, name, animations, generate_static_sprite):
	for animation in animations:
		for frame, time in animation.frames_times:
			if not frame.exported:
				frame.exported = True
				# une AnimationFrame par frame d'animation. 
				# Ce n'est pas forcément une Frame (une frame peut ne pas se trouver dans une animation)
				# une frame peut donner plusieurs AnimationFrame si elle est répétée dans l'animation
				rect_id = 0
								
				output.append("const AnimationFrame %s_frame_%d = {" % (name, frame.id))
				output.append("\t%d, // numSprite" % frame.num_sprites)
				output.append("\t%d, // timer" % time)
				output.append("\t%s, // tileset" % ("NULL" if generate_static_sprite else "(TileSet*) &%s_tileset_%d" % (name, frame.tileset.id)))
				output.append("\tNULL, // collision")
				
				output.append("\t{")
	
				if generate_static_sprite:
					frame.vdp_sprites.sort(key=lambda vdps: vdps.offset)

#				else:
#					offset = frame.vdp_sprites[0].offset # tileset[frame.tileset_id].tiles_offset
					
				for i, vdps in enumerate(frame.vdp_sprites):
					if i < len(frame.vdp_sprites) - 1:
						num_tiles = frame.vdp_sprites[i + 1].offset - vdps.offset
					else:
						num_tiles = vdps.numTiles

					output.append("\t\t{")
					output.append("\t\t\t// VDPSprite #%d" % vdps.id)
#					output.append("\t\t\t%d,  // vdp.offset" % vdps.offset)
#					output.append("\t\t\t%d,  // offset" % offset)
					output.append("\t\t\t%d,  // numTiles" % num_tiles)
					output.append("\t\t\t%d, // offsetY" % vdps.offsetY)
					output.append("\t\t\t%d, // offsetYFlip" % vdps.offsetYFlip)
					output.append("\t\t\t%d, // %02X | size (%d x %d)" % (vdps.flip | vdps.size, vdps.flip, vdps.width, vdps.height))
					output.append("\t\t\t%d, // offsetX" % vdps.offsetX)
					output.append("\t\t\t%d, // offsetXFlip" % vdps.offsetXFlip)
					output.append("\t\t},")
						
					rect_id += 1
	
				if not frame.vdp_sprites:
					print(frame.transform)
					raise Exception()

				output.append("\t}")
				output.append("};")
				output.append("")

def export_animations(output, name, animations):
	for anim in animations:
		output.append("const AnimationFrame* const %s_%s_frames[] = {" % (name, anim.name))
		for frame, timing in anim.frames_times:
			output.append("\t(AnimationFrame*) &%s_frame_%d," % (name, frame.id))
		output.append("};")
		
		output.append("")
		

		output.append("const Animation %s_%s = {" % (name, anim.name))
		output.append("\t%d, // numFrames" % len(anim.frames_times))
		output.append("\t%s, // loop" % anim.loop)
		output.append("\t(AnimationFrame**) %s_%s_frames" % (name, anim.name))
	
		output.append("};")
		output.append("")
	
	output.append("const Animation *const %s_sprite_animations[] = {" % name)
	for anim in animations:
		output.append("\t(Animation*) &%s_%s," % (name, anim.name))
	output.append("};")
	
	output.append("")


def export_sprite_definition(output, sprite_def):
	output.append("const SpriteDefinition %s_sprite = {" % sprite_def.name)
	output.append("\t%d, // width" % sprite_def.width)
	output.append("\t%d, // height" % sprite_def.height)
	output.append("\t(Palette*) &%s_sprite_palette," % sprite_def.name)
	output.append("\t%d, // numAnimations" % len(sprite_def.animations))
	output.append("\t(Animation **) %s_sprite_animations, " % sprite_def.name)
	output.append("\t%d, // maxNumTiles" % (sprite_def.max_num_tiles,))
	output.append("\t%d, // maxNumsprites" % (sprite_def.max_num_sprites,))
	output.append("};")
	
	output.append("")
	
def export_all_tilesets(output, name, tilesets, animations):
	# All frames tilesets (for static sprites)

	output.append("const TileSet *const %s_all_tilesets[] = {" % name)
	output.append(",\n".join(["\t&%s_tileset_%d" % (name, tileset.id) for tileset in tilesets] + ["\tNULL"]))
	output.append("};")

	output.append("")

	# Frames indexes (for static sprites)

	indexes = []
	for anim in animations:
		anim_ind_name = "%s_%s_tileset_indexes" % (name, anim.name)
		output.append("const u16 %s[] = {" % anim_ind_name)
		output.append("\t" + (", ".join(["%d" % frame.tileset.offset for frame, _ in anim.frames_times])))
		output.append("};")
		indexes.append(anim_ind_name)

	output.append("")

	output.append("const u16 const* %s_tilesets[] = {" % name)
	output.append(",\n".join(["\t%s" % i for i in indexes]))
	output.append("};")

	output.append("")

def process_sprite(args, base_dir="", out_dir=""):
	sheet = get_sheet(args, base_dir)	
	palette, frames = get_palette_and_frames(sheet, args)
	
	animations = get_animations(sheet, args, frames)
	
	tilesets = get_tilesets(frames, args['generate_static_sprite'])

	generate_vdp_sprites(tilesets, args["algo"], args["opt"])
	
	update_frames(frames, tilesets, args["generate_static_sprite"])
		
	sprite_definitions = get_sprite_definitions(args, palette, frames, animations, tilesets)
	
	out = ["""#include <genesis.h>""", ""]

	export_palette(out, args["name"], palette)

	export_tilesets(out, args["name"], tilesets)
		
	export_animation_frames(out, args["name"], animations, args["generate_static_sprite"])

	export_animations(out, args["name"], animations)

	export_sprite_definition(out, sprite_definitions)
	
	if args["generate_static_sprite"]:
		export_all_tilesets(out, args["name"], tilesets, animations)
	
	with open("%s/%s_sprite.c" % (out_dir, args["name"]), "w") as f:
		f.write("\n".join(out))
		