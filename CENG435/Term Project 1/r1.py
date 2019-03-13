import socket

def receive():
	host = '10.10.2.2' #IP address Of R1 Device interface.
	port = 5000 #Port Number Of R1 To Listen Broker.

	s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) #Socket Object Initialization of UDP.
	s.bind((host, port)) #Binds To Gets Packets From Broker.

	while 1: #Always Waits Packets From Broker.
		data, addr = s.recvfrom(1024) #Receives Message From Broker Up To 1024 Bytes.
		data = data.decode('utf-8') #Decodes Received Message.
		data = sendForward(data) #Calls 'sendForward' Function With Received Message.
		data += 'r1' #Appends 'r1' To Received Acknowladgement From 'sendForward' Function. This Acknowladgement Comes 'sendForward' Function From D Device.
		# s.sendto(data.encode('utf-8'), addr) #Sends Encoded Acknowladgement To Broker.
	s.close() #Closes the Connection.

#This Function Gets Message That Received From Broker And Generates UDP Packets With This Message.
def sendForward(message):
	host = '10.10.3.2' #IP address Of D device interface.
	port = 5001 #Port Number Of R1 Device To Listen D Device.

	server = (host, 5000)

	s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) #Socket Object Initialization Of UDP.
	s.bind(('10.10.3.1', port)) #Binds To Gets Acknowladgement From D Device.
	message += 'r1' #Appends 'r1' To Message.
	s.sendto(message.encode('utf-8'), server) #Send Encoded Message to D Device.
	# data, addr = s.recvfrom(1024) #Receives Acknowladgement From D Device Up To 1024 Bytes.
	s.close() #Closes the Connection.
	return data #Returns Acknowladgement


def Main():
	receive()

if __name__ == '__main__':
    Main()
