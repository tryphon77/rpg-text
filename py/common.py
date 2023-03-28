# -*- coding: utf-8 -*-
import png
import numpy as np

def list_of_int(l):
	if type(l) == list:
		return l
	return [int(x.strip()) for x in l.strip("()").split(",")]

def load_png(path):
	r = png.Reader(filename=path)
	w, h, arr, info = r.read()
#	print("info: %s" % info)
	pal_ = [(r, g, b, 255) for r, g, b, in info['palette']]
	nc = len(pal_)
	pal_ += ((0, 0, 0, 0),) * (256 - nc)
	pal = np.array(pal_, dtype=np.uint8)
	
	arr = np.array(list(arr), dtype=np.uint8)
	arr.shape = (h, w)
	return {"depth": 8, "palette": pal, "data": arr}

def save_png(path, data, pal):
	h, w = data.shape
	wr = png.Writer(w, h, palette=pal, bitdepth=8)
	with open(path, 'wb') as f:
		wr.write(f, data)

def array_to_c(name, type_, a, digits=0, max_line=16):
	if not digits:
		if "8" in type_: # in ["u8", "s8"]:
			digits=2
		elif "16" in type_: # in ["u16", "s16"]:
			digits = 4
		elif "32" in type_: # in ["u32", "s32"]:
			digits = 8
			
	res = "%s %s[] = {\n" % (type_, name)
	fmt = "0x%0" + str(digits) + "X" 
	res_ = []
	s = a.shape
	if len(s) == 1:
		for j in range(0, s[0], max_line):
			res_.append("\t" + ", ".join([fmt % x for x in a[j:j+max_line]]))
	elif len(s) == 2:
		h, w = s
		for j in range(h):
			res_.append("\t" + ", ".join([fmt % x for x in a[j]]))
	elif len(s) == 3:
		d1, d2, d3 = s
		for i in range(d1):
			res_.append("\t// %d" % i)
			for j in range(d2):
				res_.append("\t" + ", ".join([fmt % x for x in a[i, j]]))			
		
	res += ",\n".join(res_) + "\n};\n"
	
	return res

def ptrn_to_array(ptrn):
	ptrn_ = np.array(ptrn, dtype=np.uint32) & 15
	ptrn_.shape = (64,)
	res = (ptrn_[0:64:8] << 28) + (ptrn_[1:64:8] << 24) + (ptrn_[2:64:8] << 20) + (ptrn_[3:64:8] << 16) + (ptrn_[4:64:8] << 12) + (ptrn_[5:64:8] << 8) + (ptrn_[6:64:8] << 4) + ptrn_[7:64:8]
	return res

def ptrns_to_array(ptrns):
	res = [ptrn_to_array(ptrn) for ptrn in ptrns]
	return np.array(res, dtype=np.uint32)

def cvt_4x8bpp_to_9bpp(val):
	r = (val[0] >> 5) << 1
	g = (val[1] >> 5) << 1
	b = (val[2] >> 5) << 1
	return (b << 8) | (g << 4) | r

def enlarge(frame, x):
	h, w = frame.shape
	res = np.zeros((h + 2*x, w + 2*x), dtype=frame.dtype)
	res[8 : 8 + h, 8 : 8 + w] = frame[:, :]
	return res

def get_pos_in_str(k, i = 0):
	l = k.index("(", i)
	r = k.index(")", l + 1)
	x_, y_ = k[l + 1 : r].split(",")
	return int(x_.strip()), int(y_.strip())

def get_top_left(frame):
	h, w = frame.shape
	for x0 in range(w):
		if frame[:, x0].any():
			break
	for y0 in range(h):
		if frame[y0, :].any():
			break
	return (x0, y0)
	
def get_bottom_right(frame):
	h, w = frame.shape
	for x1 in range(w-1, -1, -1):
		if frame[:, x1].any():
			break
	for y1 in range(h-1, -1, -1):
		if frame[y1, :].any():
			break
	return (x1, y1)

def first_multiple(x, n=8):
	if x % n:
		return x - (x % n) + n
	return x

def crop(frame, rect):
	x, y, w, h = rect
	x0, y0 = get_top_left(frame[y : y + h, x : x + w])
	x1, y1 = get_bottom_right(frame[y : y + h, x : x + w])
	
	print("crop:", x0, y0, x1, y1)	
	w_, h_ = first_multiple(x1 - x0 + 1), first_multiple(y1 - y0 + 1)
	if w_ <= 0 or h_ <= 0:
		print("  discarded")
		return None
	return (x + x0, y + y0, w_, h_)

def tokenize(line):
	line = line.strip()
	res = []
	
	i = j = 0
	while j < len(line):
		c = line[j]
		j += 1
		if c in [" ", "(", ")", ","]:
			if i < j - 1:
				res.append(line[i : j - 1])
			if c != " ":
				res.append(line[j - 1])
			i = j
	if i < j:
		res.append(line[i : j])
	
	return res
			
	
	
	
	
	