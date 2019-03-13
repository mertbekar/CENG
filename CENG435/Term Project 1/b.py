import socket
from random import randint


#This Function Gets Message That Received From TCP And Generates UDP Packets With This Message.
def tcpToUdp(r, message):
	if r == 1: #Sets The Configurations For R1 Device If Random Choice r Equals To 1.
		host = '10.10.2.2' #IP address Of R1 device interface.
		port = 5001 #Port Number Of Broker To Listen R1 Device.

		server = (host, 5000)

		s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) #Socket Object Initialization Of UDP.
		s.bind(('10.10.2.1', port)) #Binds To Gets Acknowladgement From R1 Device.

	if r == 2: #Sets the Configurations For R2 Device If Random Choice r Equals To 2.
		host = '10.10.4.2' #IP Address Of R2 Device Interface.
		port = 5002 #Port Number Of Broker to Listen R2 Device.

		server = (host, 5000)

		s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) #Socket Object Initialization of UDP.
		s.bind(('10.10.4.1', port)) #Binds to Gets Acknowladgement From R2 Device.

	s.sendto(message.encode('utf-8'), server) #Send Encoded Message to R1 or R2 According To Random Choice
	# data, addr = s.recvfrom(1024) #Receives Acknowladgement From R1 or R2 Up To 1024 Bytes
	s.close() #Closes the Connection.
	return 'data' #Returns Acknowladgement

#Main Function Acts As TCP Server
def Main():
	host = '10.10.1.2' #IP address Of Broker interface.
	port = 5000 #Port Number Of Broker To Listen S Device.

	s = socket.socket() #Socket Object Initialization, Default is TCP.
	s.bind((host, port)) #Binds To Gets Packets From S Device.

	s.listen(1) #Listens 1 TCP Connection At A Time.
	c, addr = s.accept() #Accepts TCP Connection Request That Comes From S Device.

	while(1): #Always Waits Packets From S Device.
		data = c.recv(1024) #Receives Message From S Device Up To 1024 Bytes.
		if not data: #If Received Message Is Empty, Server Stops
			break
		data += 'b' #Appends 'b' To Received Message.
		data = tcpToUdp(randint(1, 2), data) #Sends Received Message And Random Choice To 'tcpToUdp' function.
		data += 'b' #Appends 'b' To Received Acknowladgement From 'tcpToUdp' Function. This Acknowladgement Comes 'tcpToUdp' Function From R1 or R2 Device.
		# c.send(data.encode()) #Sends Encoded Acknowladgement To S Device.

	c.close() #Closes the Connection.

if __name__ == '__main__':
	Main()
