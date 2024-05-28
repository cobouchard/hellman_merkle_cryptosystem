#!/usr/bin/env python3

import json
import socket
import threading
import time
import os
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
call(["./../mhe", "-g"]) 

temp_file="temp_bob"
def bob_decipher(data):
	with open(temp_file, 'wb') as file:
		file.write(data)

	#decipher the message
	call(["./../mhe", "-d", temp_file, "private_key"])

	#removing temporary file
	os.remove(temp_file)

	#reading the result
	with open("decipher.txt", 'r') as file:
		print("Here was the message of Alice :")
		print(file.read())

#Bob waits for Alice message
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(bob_addr)
data = None

print("Hi, I'm Bob. I have generated public and private keys. I'm waiting for Alice's messages")

while True:
	data, addr_client = sock.recvfrom(1024)
	if data!=None:
		#Bob received Alice message he deciphers it
		print("Received a message from Alice")

		thread = threading.Thread(target=bob_decipher, args=(data,))
		thread.start()


#Bob deciphered Alice message and can read it
