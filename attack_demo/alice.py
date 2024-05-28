#!/usr/bin/env python3

import json
import socket
import os
from subprocess import call

charlie_addr = 0
result_file = 0
alice_port = 0

with open("info.json") as json_file:
	try:
		json_data = json.load(json_file)

		charlie_addr_ip = json_data['charlie_addr']
		charlie_port = json_data['charlie_port']
		charlie_addr = (charlie_addr_ip,charlie_port)
		alice_port = json_data['alice_port']

		result_file = json_data['result_file']

	except Exception as e:
		print("Couldn't open \"info.json\"")
		raise e


print("Hi, I'm Alice. Let's send a message to Bob !")
user_input = input("Enter message: ")

msg_file = "msg.txt"
with open(msg_file, 'w') as file:
	file.write(user_input)

print("Ciphering data...")
call(["./../mhe", "-c", "msg.txt", "public_key"]) 

os.remove(msg_file)

#open the result of cipher and send it
data=None
with open(result_file, mode='rb') as file:
	data = file.read()

print("Sending message to Bob via Charlie")
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('', alice_port))
sock.sendto(data, charlie_addr)
print("Message sent")