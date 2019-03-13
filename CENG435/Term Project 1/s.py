import socket
import time


def Main():
	host = '10.10.1.2' ##IP Address of Broker Device Interface.
	port = 5000 # Port Number of Broker Device.

	s = socket.socket() #Socket Object Initialization, Default is TCP.
	s.connect((host, port)) #TCP Connection.

	for i in range(2000): #Generates 2000 Packets
		start = time.time() #Generates Creation Time of Packets
		message = str(int(start * 1000)) + ("%04d" % (i)) + 's' #Creates a Message With This Format: Creation Time + Packet ID + 's'.
		s.send(message.encode()) #Send Encoded Message to Broker.
		# data = str(s.recv(1024)) #Receives Acknowladgement From Broker Up To 1024 Bytes.
		# data += 's' #Appends 's' To Received Acknowladgement.
		# print('Packet ID: ' + data[13:17] + ', Path: ' + data[17:]) #Prints Acknowladgement.
	s.close() #Closes the Connection.

if __name__ == '__main__':
	Main()
