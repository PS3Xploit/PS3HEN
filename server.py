#!/usr/bin/env python3

"""
	Main web server.
"""

import os
import sys
import socket
import socketserver, http.server
import urllib.parse, urllib.request, urllib.error, urllib.parse
from capstone import Cs, CS_ARCH_PPC, CS_MODE_BIG_ENDIAN

PORT = 8080
DUMP_PATH = "dumps"

"""
	Display module exports
"""
def display_mexport(addr, data, libname):
	m = module.ModExport(data)

	print("--------------------------------------------------------------------")
	print("- Module Export: %s 0x%x -"%(libname, addr))
	print(m)
	print("--------------------------------------------------------------------")

"""
	Display module imports
"""
def display_mimport(addr, data, libname):
	"""
		Check for modules in 0xE0000000 space
	"""
	if (addr & 0xE0000000) == 0xE0000000:
		m = module.ModImportSmall(data)
		
		print("--------------------------------------------------------------------")
		print("- Module Import (Small): %s 0x%x -"%(libname, addr))
		print(m)
		print("--------------------------------------------------------------------")
	else:
		m = module.ModImport(data)
		
		print("--------------------------------------------------------------------")
		print("- Module Import: %s 0x%x -"%(libname, addr))
		print(m)
		print("--------------------------------------------------------------------")  

"""
	Display module info
"""
def display_minfo(addr, data):
	m = module.ModInfo(data)
	base_addr = addr + module.MODIF_SIZE - m.ent_top
	nexports = (m.ent_end - m.ent_top) / module.MODX_SIZE
	nimports = (m.stub_end - m.stub_top) / module.MODIM_SIZE

	print("--------------------------------------------------------------------")
	print("- Module Info: %s 0x%x -"%(m.modname, addr))
	print("-> Base address: 0x%x"%(base_addr))
	print("-> # Exports: 0x%x"%(nexports))
	print("-> Export table at: 0x%x"%(base_addr + m.ent_top))
	print("-> # Imports: 0x%x"%(nimports))
	print("-> Import table at: 0x%x"%(base_addr + m.stub_top))
	print(m)
	print("--------------------------------------------------------------------")

"""
	Dump data
"""
def dump_data(data, file_name):
	if not os.path.exists(DUMP_PATH):
		os.makedirs(DUMP_PATH)
	else:
		remove_directory(file_name)
	fp = open(DUMP_PATH + "/" + file_name, "ab")
	fp.write(data)
	fp.close()
	
"""
	Dump text
"""
def dump_text(text, file_name):
	if not os.path.exists(DUMP_PATH):
		os.makedirs(DUMP_PATH)
	else:
		remove_directory(file_name) 
	outPath = DUMP_PATH + "/" + file_name
	if len(file_name) > 1:
		if file_name[1] == ':':
			outPath = file_name
	fp = open(outPath,"a")
	fp.write(text)
	fp.close()
	
"""
	Make directory
"""
def make_directory(dir_name):
	if not os.path.exists(DUMP_PATH + "/" + dir_name):
		os.makedirs(DUMP_PATH + "/" + dir_name)
		
"""
	Remove directory
"""
def remove_directory(dir_name):
	if os.path.exists(DUMP_PATH + "/" + dir_name):
		os.removedirs(DUMP_PATH + "/" + dir_name)

"""
	Display data
"""
def display_data(addr, src, length = 8, n = 8):
	filter_=''.join([(len(repr(chr(x)))==3) and chr(x) or '.' for x in range(256)])
	result=[]
	for i in range(0, len(src), length):
	   s = src[i:i+length]
	   hexa = ''.join(["%02X"%ord(x) for x in s])
	   hexa = ' '.join(["".join(hexa[j:j+n]) for j in range(0, len(hexa), n)])
	   printable = s.translate(filter_)
	   result.append("0x%08X: %-*s %s\n" % (addr + i, length*3, hexa, printable))
	return ''.join(result)


"""
	Disassemble data
"""
def disassemble(addr, data):
	none = 0
	md = Cs(CS_ARCH_PPC, CS_MODE_BIG_ENDIAN)
	disassed = md.disasm(data, addr)
	for i in disassed:
		none = 1
		print("0x%x:\t%s\t%s" %(i.address, i.mnemonic, i.op_str))
	if none != 1:
		print("Couldn't disassemble at 0x%x"%(addr))

"""
	Web Server
"""
class PS3WebServer(http.server.SimpleHTTPRequestHandler):
	"""
		GET Request Handler
		Used for debugging and interactive shell stuff
	"""
	def do_GET(self):
		if self.path.startswith('/Debug'):
			print('[+] DBG: ', end=' ')
			parsed = urllib.parse.parse_qs(urllib.parse.urlparse(self.path).query)
			dbg = parsed['dbg'][0]
			print(dbg)
		# handle dump
		elif self.path == '/Command':
			sockfd = self.request
			self.send_response(200)
			self.send_header('Content-type', 'text/html')
			self.end_headers()
			cmd = input("%> ")
			self.wfile.write(cmd)
		# normal requests
		else:
			http.server.SimpleHTTPRequestHandler.do_GET(self)

	"""
		POST Request Handler
		This is only used to dump data from the Vita's memory
	"""
	def do_POST(self):
		length = int(self.headers.getheader('content-length'))
		if length:
			rdata = self.rfile.read(length)
			rdata = urllib.parse.parse_qs(rdata)
			addr = 0
			extra = ""

			try:
				addr = int(rdata['addr'][0], 0)
			except KeyError:
				print("[+] Warning: addr not received")
			try:
				data = rdata['data'][0]
			except KeyError:
				print("[+] Error: dump not received")
				return
			try:
				typ = rdata['type'][0]
			except KeyError:
				print("[+] Error: msg type not received")
				return

			try:
				extra = rdata['extra'][0]
			except KeyError:
				pass

			if(typ == 'read'):
				print(display_data(addr, data.decode('hex')))

			if(typ == 'disasm'):
				disassemble(addr, data.decode('hex'))

			if(typ == 'dump'):
				fname = extra
				dump_data(data.decode('hex'), fname)
				
			if(typ == 'log'):
				fname = extra
				dump_text(data, fname)
				
			if(typ == 'mkdir'):
				make_directory(data)
				
			if(typ == 'rmdir'):
				remove_directory(data)

			if(typ == "mod_info"):
				display_minfo(addr, data.decode('hex'))

			if(typ == "mod_export"):
				display_mexport(addr, data.decode('hex'), extra.decode('hex'))

			if(typ == "mod_import"):
				display_mimport(addr, data.decode('hex'), extra.decode('hex'))
				
		
				
"""
	Print IP+PORT
"""

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.connect(("gmail.com", 80))
localIP = s.getsockname()[0]
s.close()

print("Starting server on " + localIP + ":" + str(PORT))

"""
	Start server
"""
socketserver.TCPServer.allow_reuse_address = True
server = socketserver.TCPServer(('', PORT), PS3WebServer)
server.serve_forever()