#!/usr/bin/env python3

import json
import socket
from subprocess import call


bob_addr = 0

with open("info.json") as json_file:
	try:
		json_data = json.load(json_file)

		bob_addr_ip = json_data['bob_addr']
		bob_port = json_data['bob_port']
		bob_addr = (bob_addr_ip,bob_port)


	except Exception as e:
		print("Couldn't open \"info.json\"")
		raise e

#Bob generates public and private keys
call(["./../mhe -g"]) 

#Bob waits for Alice message
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(bob_addr)
data = None

while True:
	data, addr_client = sock.recvfrom(1024)
	if data!=None:
		#Bob received Alice message he deciphers it

		#call(['./mhe data'])
		break;

#Bob deciphered Alice message and can read it
