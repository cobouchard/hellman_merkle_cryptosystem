#!/usr/bin/env python3

"""

We suppose Charlie is in between Alice and Bob, he is for example a router somewhere between them.
Charlie will attack the message and find what Alice sent to Bob using Merkle-Hellman cryptosystem (probably a key for asymetric cryptography)

"""

import json
import socket
import threading
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
	

def attack(data):
	print("Attack")
	print(data)
	#call(None) #this should call the compiled C to decipher
	

#starting an udp server to communicate

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(charlie_addr)
data = None

while True:
	data, addr_client = sock.recvfrom(1024)
	if data!=None:
		print(addr_client)
		
		#we send the data to Bob/Alice as expected
		if(addr_client==alice_addr):
			print("Forwarding message to Bob")
			sock.sendto(data, bob_addr)
		elif(addr_client==bob_addr):
			print("Forwarding message to Alice")
			sock.sendto(data, alice_addr)

		#but we also try do decipher it
		thread = threading.Thread(target=attack, args=(data,))
		thread.start()