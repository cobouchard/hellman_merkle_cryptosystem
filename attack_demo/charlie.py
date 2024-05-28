#!/usr/bin/env python3

"""

We suppose Charlie is in between Alice and Bob, he is for example a router somewhere between them.
Charlie will attack the message and find what Alice sent to Bob using Merkle-Hellman cryptosystem (probably a key for asymetric cryptography)

"""

import json
import socket
import threading
import time
import os
from subprocess import call

alice_addr = 0
bob_addr = 0
charlie_addr = 0

#reading addresses from a file

with open("info.json") as json_file:
	try:
		json_data = json.load(json_file)

		alice_addr_ip = json_data['alice_addr']
		alice_port = json_data['alice_port']
		alice_addr = (alice_addr_ip,alice_port)

		bob_addr_ip = json_data['bob_addr']
		bob_port = json_data['bob_port']
		bob_addr = (bob_addr_ip,bob_port)

		charlie_addr_ip = json_data['charlie_addr']
		charlie_port = json_data['charlie_port']
		charlie_addr = (charlie_addr_ip,charlie_port)

	except Exception as e:
		print("Coudln't open \"info.json\"")
		raise e
	

temp_file="temp_charlie"
def attack(data):
	with open(temp_file, 'wb') as file:
		file.write(data)

	time.sleep(2)

	#attack of the ciphered message
	call(["./../mhe", "-a", "public_key", temp_file]) 
	
	#removing temporary file
	os.remove(temp_file)

	#reading the result
	with open("decipher.txt", 'r') as file:
		print("Here was the message of Alice :")
		print(file.read())
	

#starting an udp server to communicate

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(charlie_addr)
data = None

print("Hi, I'm Charlie. I will transfer every message from Alice to Bob. I will also attack their message using Bob's public key")

while True:
	data, addr_client = sock.recvfrom(1024)
	if data!=None:
		#we send the data to Bob/Alice as expected
		if(addr_client==alice_addr):
			print("Forwarding message to Bob")
			sock.sendto(data, bob_addr)
		else:
			print("Incorrect sender")


		#but we also try do decipher it
		thread = threading.Thread(target=attack, args=(data,))
		thread.start()