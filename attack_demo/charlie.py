#!/usr/bin/env python3

"""

We suppose Charlie is in between Alice and Bob, he is for example a router somewhere between them.
Charlie will attack the message and find what Alice sent to Bob using Merkle-Hellman cryptosystem (probably a key for asymetric cryptography)

"""

import json

alice_addr = 0
bob_addr = 0
charlie_addr = 0


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

		print(alice_addr)
		print(bob_addr)
		print(charlie_addr)
	except Exception as e:
		print("Coudln't open \"info.json\"")
		raise e
	

