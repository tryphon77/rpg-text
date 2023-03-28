# -*- coding: utf-8 -*-

from tfont import read_encoding
import yaml


params = [
	("name", str, ""), 
	("path", str, ""), 
]

dialog_table = []
bytes_written = 0
branches = []

def read_condition(buffer, cond, events):
	c = events.index(cond)
	buffer.append(c >> 8)
	buffer.append(c & 0xFF)

def read_conditions(buffer, param, events):
	conds = [c.strip() for c in param.split("|")]
	for cond in conds:
#		print("cond=%s, conds=%s" % (cond, conds))
		read_condition(buffer, cond, events)

	buffer.append(0xFF)
	buffer.append(0xFF)

def read_branch(buffer, param, language):
	branches.append((len(buffer), param, language))
	buffer.append(0)
	buffer.append(0)

def get_offset(name, language):
#	print("get_offset(%s, %s)" % (name, language))
	for name_, language_, offset in dialog_table:
#		print(name_, language_, offset)
		if name_ == name and language_ == language:
			return offset
	raise Exception()

def encode_command(buffer, cmd, language, text_context):
#	print("encode command: [%s]" % cmd)

	escape_char = text_context["escape"]
	cmds = text_context["controls"]
	defs = text_context["define"]
	events = text_context["events"]
	
	params = []
	if "(" in cmd:
		i = cmd.index("(")
		j = cmd.index(")")
		params = [x.strip() for x in cmd[i + 1 : j].split(",")]
		cmd = cmd[:i]
	
	cmd_code = cmds.index(cmd)
	buffer.append(0xFF)
	buffer.append(cmd_code)

	if cmd == "if":
		# param_code += 
		read_conditions(buffer, params[0], events)
		# param_code += 
		read_branch(buffer, params[1], language)
	
	elif cmd == "goto":
		# param_code += 
		read_branch(buffer, params[0], language)
	
	elif cmd == "set" or cmd == "clr":
		read_condition(buffer, params[0], events)

	else:
		for par in params:
			if par.isdigit():
				buffer.append(int(par))
			elif par in defs:
				buffer.append(defs[par])
			else:
				buffer.append(0x42)
				print("cmd=%s" % cmd)
				print("par=%s" % par)
				raise Exception()
			

def encode(buffer, text, language, text_context):

	space_char = text_context["space"]	
	encoding = text_context["encoding"]

	plane_id = 0
	i = 0
	while i < len(text):
		c = text[i]
		i += 1
		if c == "[":
			j = text.index("]", i)
			cmd = text[i : j]
			# res += 
			encode_command(buffer, cmd, language, text_context)
			i = j + 1
		elif c == '\n':
			# res += 
			encode_command(buffer, "cr", language, text_context)
		elif c == ' ':
			buffer.append(space_char)
		else:
#			print("|%s|" % c)
			char_id = encoding.index(c)
			if not ((plane_id << 8) <= char_id < ((plane_id + 1) << 8)):
				plane_id = char_id >> 8
				# res += 
				encode_command(buffer, "set_font_plane(%d)" % plane_id, language, text_context)
			buffer.append(char_id & 0xFF)


def process(context, args, base_dir="", out_dir=""):
	global bytes_written
	
#	print("%s/%s" % (base_dir, args["path"]))
	with open("%s/%s" % (base_dir, args["path"].strip('"')), encoding="utf8") as f:
		doc = f.read()

	doc = doc.replace('\t', '  ')

	dialogs = yaml.load(doc, Loader=yaml.FullLoader)

	text_context_name = args["context"]
	
	text_context = context[text_context_name]

	languages = text_context["languages"]

	encoded_text = []

	res = []
	hdr = []
	
	res.append('#include <genesis.h>')
	res.append('')
	res.append('')
	
	hdr.append("#ifndef %s_DIALOG_H_" % args["name"].upper())
	hdr.append("#define %s_DIALOG_H_" % args["name"].upper())
	hdr.append('')
	hdr.append('')

#	print("dialogs:", dialogs)
	for dialog_name in dialogs:
#		print("dialog_name:", dialog_name)
		dialog = dialogs[dialog_name]
#		print("dialog:", dialog)
		for language in languages:

			dialog_table.append((dialog_name, language, len(encoded_text)))
			encode(encoded_text, dialog[language], language, text_context)


	for source_offset, dest, language in branches:
		dest_offset = get_offset(dest, language)
		offset = dest_offset - source_offset - 2
#		print("write %04X at %04x" % (offset, dest_offset))
		encoded_text[source_offset] = offset >> 8
		encoded_text[source_offset + 1] = offset & 0xFF


	res2 = []
	for k in range(len(dialog_table)):
		dialog_name, language, offset = dialog_table[k]
		res.append("const u16 %s_%s = 0x%X;" % (dialog_name, language, offset))
		hdr.append("const u16 %s_%s;" % (dialog_name, language))

		if k == len(dialog_table) - 1:
			end_offset = len(encoded_text)
		else:
			_, _, end_offset = dialog_table[k + 1]
		res1 = []
		for i in range(0, end_offset - offset, 16):
			res1.append(", ".join(["0x%02X" % x for x in encoded_text[offset : end_offset][i : i + 16]]))
		res2.append(",\n".join(["\t%s" % x for x in res1]))


	res.append("")
	res.append("const u8 %s_data[] = {" % args["name"])
	res.append(",\n\n".join(["/* %s_%s:\n%s*/\n%s" % (item[0], item[1], dialogs[item[0]][item[1]], res2[i]) for i, item in enumerate(dialog_table)]))
	res.append("};")
	res.append("")
	
	hdr.append("")
	hdr.append("extern const u8 %s_data[];" % args["name"])		
	hdr.append("")

	hdr.append("#endif")
	
	with open("%s/%s_dialog.c" % (out_dir, args["name"]), "w", encoding="utf8") as f:
		f.write("\n".join(res))

	with open("%s/%s_dialog.h" % (out_dir, args["name"]), "w", encoding="utf8") as f:
		f.write("\n".join(hdr))
	
	return {}
