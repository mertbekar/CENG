import socket
import time

payloads = []

#This function gets the input from file and puts into "payloads" array.
def create_packet_array():
	with open("input.txt", "r") as f:
		for i in range(10000): #We decided that each packet carries 500 bytes of payload. We have 10000 packets because input file is 5Mb.
			p = f.read(500) #Each packet have 500 bytes payload.
			payloads.append(p) #Appends each 500 bytes payload to "payloads" array.

def Main():
	host = '10.10.1.2' #IP Address of Broker Node.
	port = 5000 #Port Number of Broker Node.

	create_packet_array() #Gets input and creates a "payload" array.

	s = socket.socket() #Creates a socket object.
	s.connect((host, port)) #Source Node connects to Broker Node via created socket object.

	for i, payload in enumerate(payloads): 
		s.send(payload) #Packetize the all payloads and sends to Broker Node.
	last_packet = '' #Create a payload to "fin" packet.
	for i in range(500):
		last_packet += 'X' #Fills the "fin" packet to realize data transfer ends.
	for i in range(10):
		s.send(last_packet) #Sends the "fin" packet.
	s.close() #Closes the Connection.

if __name__ == '__main__':
	Main()
