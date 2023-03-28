# -*- coding: utf-8 -*-

from common import load_png, list_of_int
import numpy as np

params = [
	("name", str, ""), 
	("path", str, ""), 
]

ascii_encoding = [None] * 0x20 + [chr(x) for x in range(0x20, 0x80)]

def read_encoding(path, base_dir=""):
	if path == "ascii":
		return ascii_encoding
	
	with open("%s/%s" % (base_dir, path), encoding="utf8") as f:
		data = f.readlines()
	res = []
	for line in data:
		res += list(line.rstrip("\r\n"))
	
#	for i, r in enumerate(res):
#		print("%03X: |%s|" % (i, r))

	return res


def process(context, args, base_dir="", out_dir=""):
	out = []
	hdr = []

#	print(args)
#	print("base_dir=%s, out_dir=%s" % (base_dir, out_dir))

	file_data = load_png("%s/%s" % (base_dir, args["path"].strip('"')))
	assert file_data["depth"] == 8
		
	sheet_height, sheet_width = file_data["data"].shape
	file_data["data"] &= 15

	width, height = list_of_int(args["cellsize"])
	
#	inter_word = int(args["inter_word"])
	space = int(args["space"])

#	print(sheet_width, sheet_height, width, height)
	
	if args["encoding"] == "ascii":
		encoding = ascii_encoding
	else:
		encoding = read_encoding(args["encoding"].strip('"'), base_dir)
	
	char_id = 0
	out.append("#include <genesis.h>")
	out.append("")
	out.append('#include "tfont.h"')
	out.append("")

	out.append("const u8 %s_data[] = {" % args["name"])

	if args["width"] == "auto":
		width_table = []
	else:
		with open("%s/%s" % (base_dir, args["width"].strip('"'))) as f:
			width_raw = f.readlines()
		
		width_table = []
		for line in width_raw:
			width_table += [int(x) for x in line.strip().split()]

	for y in range(0, sheet_height, height):
		for x in range(0, sheet_width, width):
			letter = file_data["data"][y : y + height, x : x + width]
#			print("===")
#			print(x, y)
#			print(letter)
#			d = np.zeros((height,), dtype=np.uint32)
			d2 = np.zeros((width,), dtype=np.uint32)

			for j in range(width):
				col = letter[j, :]&1
				d2 += col << (height - j - 1)

#			print(char_id)
			char = encoding[char_id]
#			print("char %X: |%s|" % (char_id, char))

			if args["width"] == "auto":
				if char == " ":
					w = space
				else:
					for w in range(width):
						row = letter[:, w]&1
						if (row == 0).all():
							break
		#				d += (letter[:, i]&1) << (width - i - 1)
					w += 1
				
				width_table.append(w)
			
#			out.append("\t" + (", ".join(["0x%02X" % x for x in d])) + (",\t// %X : %s //" % (char_id, char if char else ".")))
			out.append("\t" + (", ".join(["0x%02X" % x for x in d2])) + (",\t// %X : %s //" % (char_id, char if char else ".")))
			char_id += 1

	out.append("};")
	out.append("")
	
	out.append("const u8 %s_width_table[] = {" % args["name"])
	
	for i, w in enumerate(width_table):
		char = encoding[i]
		out.append("\t%d,\t // %X : %s //" % (w, i, char if char else "."))

	out.append("};")
	out.append("")
	
	out.append("TFont %s = {" % args["name"])
	out.append("\t%d," % width)
	out.append("\t%d," % height)
	out.append("\t%s_data," % args["name"])
	out.append("\t%s_width_table," % args["name"])
	out.append("\t%d" % args["space"])
	out.append("};")
	out.append("")
	
	hdr.append("#ifndef %s_H_" % args["name"].upper())
	hdr.append("#define %s_H_" % args["name"].upper())

	hdr.append("")
	hdr.append("TFont %s;" % args["name"])
	hdr.append("")

	hdr.append("#endif")


	with open("%s/%s_tfont.c" % (out_dir, args["name"]), "w", encoding="utf8") as f:
		f.write("\n".join(out))

	with open("%s/%s_tfont.h" % (out_dir, args["name"]), "w", encoding="utf8") as f:
		f.write("\n".join(hdr))

	if False:
		for i in range(0, len(width_table), 16):
			print(" ".join(["%d" % x for x in width_table[i : i + 16]]))

	return {"font": args["name"]}
