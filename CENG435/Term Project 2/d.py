import os, threading
import socket
import time
import csv
import sys
import Queue
import md5

expectedseqnum = 0 #It stores expected squence number from Broker Node.
received_file = '' #Initially empty string. We used it for putting received packets' payloads.
lock = threading.Lock() #Generates a "lock" object for prevent race condition.
t0 = None #It stores time for calculate file transfer time.

def receive(host):
	global lock #Gets global "lock" object, "lock".
	global expectedseqnum #Gets global expected sequence number, "expectedseqnum".
	global received_file #Gets global string, "received_file".
	global t0 #Gets global time value, "t0".

	port = 5000 #Port Number of Destination Node.

	s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) #Creates UDP socket object.
	s.bind((host, port)) #Binds created UDP socket object to Destination Node.
	while 1:
		data, addr = s.recvfrom(522) #Receives 522 bytes data, 500 bytes is payload, 6 bytes is sequence number of packet, 16 bytes is checksum value.
		#print(str(data[-22:-16]) + '-' + str(expectedseqnum))
		payload = str(data[:-16]) #First 500 bytes of data is payload. It assigned "payload" value.
		checksum = md5.new() #Generates new "MD5" object to calculate new checksum value.
		checksum.update(payload) #Updates the "MD5" object wtih the payload of received packet to calculate checksum.
		check = checksum.digest() #Checksum value obtained with this method.
		if check != data[-16:]: #Last 16 digit of data is original checksum of packet. If calculated checksum does not equal to it, received packet corrupted.
			#print('CORRUPTED DATA')
			continue #If received packet corrupted, we do not send acknowledgement for it and continue infinite loop to receive new packet.
		data2 = data[:-16]
		data2 = int(data2[-6:]) #Last 6 bytes of data is sequence number of received packet. It assigned "data2" value.
		lock.acquire() #Locks the this block to prevent race condition between two threads.
		if expectedseqnum == 1: 
			t0 = time.time() #If expected sequence number equals 1, it means first packet received correctly, sets "t0" value to time of at the moment.
		if data2 == expectedseqnum: 
			expectedseqnum += 1 #If received packets' sequence number equals to expected sequence number, increments expected sequence number by 1 because acknowledgement will be sent for received packet.
			if data[:10] == 'XXXXXXXXXX':
				print('LAST PACKET RECEIVED, TERMINATING')
				print('FILE TRANSMISSION TIME:')
				print(time.time() - t0) #If received packet "fin", calculates file transfer time and cuts the infinite loop for finihing file transfer.
				# f = open("output.txt", "w")
				# f.write(received_file)
				# f.close()
				break #Cuts the infinite loop.
			received_file += data[:-22] #If received packet does not "fin", append received packets' payload to "received_file" string.
		else: #If received packets' sequence number does not equal to expected sequence number, expected sequence number is not incremented.
			print('data2 != expectedseqnum')
		ack = "%06d" % (expectedseqnum) #Creating 6 digit acknowledgement with using expected sequence number.
		s.sendto(ack, addr) #Sends acknowledgement to Broker Node via same path.
		lock.release() #Release the "lock" object.

	s.close() #Closes to connection after file transfer ends.

def Main():
    threading.Thread(target=receive, args=['10.10.3.2']).start() #Creates a thread for "receive" function to receives packets which come from Broker Node via R1 simultaneously and sends acknowledgement for received packets and starts created thread.
    threading.Thread(target=receive, args=['10.10.5.2']).start() #Creates a thread for "receive" function to receives packets which come from Broker Node via R2 simultaneously and sends acknowledgement for received packets and starts created thread.

if __name__ == '__main__':
    Main()
