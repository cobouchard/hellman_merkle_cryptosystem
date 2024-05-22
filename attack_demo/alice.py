#!/usr/bin/env python3

import json
import socket
from subprocess import call

charlie_addr = 0
result_file = 0
input_file = 0

with open("info.json") as json_file:
	try:
		json_data = json.load(json_file)

		charlie_addr_ip = json_data['charlie_addr']
		charlie_port = json_data['charlie_port']
		charlie_addr = (charlie_addr_ip,charlie_port)

		result_file = json_data['result_file']
		input_file = json_data['input_file']

	except Exception as e:
		print("Couldn't open \"info.json\"")
		raise e



print("Ciphering data...")
#call(None) #call(["./mhe"])

#open the result of cipher and send it
data=None
with open(result_file, mode='rb') as file:
	data = file.read()

print("Sending message to Bob via Charlie")
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(data, charlie_addr)
print("Data sent")