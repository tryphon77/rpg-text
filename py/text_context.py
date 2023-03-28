# -*- coding: utf-8 -*-

import yaml
from tfont import read_encoding

params = [
	("name", str, ""), 
	("path", str, ""), 
]

def process(context, args, base_dir="", out_dir=""):
	print("%s/%s" % (base_dir, args["path"]))
	with open("%s/%s" % (base_dir, args["path"].strip('"'))) as f:
		doc = f.read()

	doc = doc.replace('\t', '  ')

	text_context = yaml.load(doc, Loader=yaml.FullLoader)
	text_context["encoding"] = read_encoding(text_context["encoding"], base_dir)
	return {args["name"]: text_context}
