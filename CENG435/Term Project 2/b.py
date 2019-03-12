import socket
from random import randint
import os, threading
import Queue
import time
import md5

s1 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) #Socket object for transfer data to Destination Node via Router 1, R1.
s2 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) #Socket object for transfer data to Destination Node via Router 2, R2.

msgs = Queue.Queue() #Queue for received packets from Source Node.
acks = Queue.Queue() #Queue for received acknowledgement from Router 1 and Router 2.
sndpkt = Queue.Queue() #Backup Queue for received packets from Source Node.

nextseqnum = 0 #It stores sequence number of last sent packet + 1, initially 0.
base = 0 #It stores last correctly received acknowledgement number, initially 0.
n = 5 #It stores window size, we choose as a 5.
t0 = time.time() #It stores time for timeout mechanism.

#This function puts sequence number into payload
def make_pkt(nextseqnum, data, checksum): 
	data += "%06d" % (nextseqnum) #Puts 6 digit sequence number into payload.
	return data

#This function starts the timer.
def start_timer():
	global t0 #Gets global time value, "t0".
	t0 = time.time() #Updates "t0" to current time.

#This function receives acknowledgements from Destination Node.
def rdt_rcv():
	global nextseqnum #Gets global next sequence number value, "nextseqnum".
	global base #Gets global base value, "base".
	global n #Gets global window size value, "n".
	global t0 #Gets global time value, "t0".

	while 1:
		if time.time() - t0 >= 0.2: #Checks timeout on each iteration.
			timeout_send()
			pass
		try:
			ack = acks.get_nowait() #If there is a acknowledgement in "acks" queue, it gets and removes from "acks" queue.
			if ack >= base: #Checks received acknowledgement is expected acks or not.
				base = ack + 1 #If received acknowledgement is expected ack, increment base by one.
				rdt_send() #After incrementing base, calls "rdt_send()" function to send queued packet.
		except:
			continue #If there is no acknowledgement in "acks" queue, continue to infinite loop to handle acknowledgements when they received.

#This function is called when timeout occurs. It is used for send packets which between base and next sequence number.
def timeout_send():
	q = sndpkt.queue #It copies an all elements in "sndpkt" queue into "q" array.
	for i in range(base-1, nextseqnum):
		pkt = make_pkt(i, q[i], 1) #It gets payload from "q" array and makes packet with it while calling "make_pkt" function.
		# print('PACKET RETRANSMISSION')
		udt_send(pkt) #It calls "udt_send" function for send packets which between base and next squence number.
	del q #Deletes "q" array to memory utilization.

#This function sends the packets to destination except for timeout case.
def rdt_send():
	global nextseqnum #Gets global next sequence number value, "nextseqnum".
	global base #Gets global base value, "base".
	global n #Gets global window size value, "n".
	while 1:
		if nextseqnum < base + n: #It checks whether there is unsent packets into window or not.
			try:
				data = msgs.get_nowait() #It tries getting payload from "msgs" queue and deleting this payload from this queue. If another threads is using this queue, "get_nowait()" method cannot get the data from queue, so we called in "try" block.
				pkt = make_pkt(nextseqnum=nextseqnum, data=data, checksum=1) #It makes packet with received payload while calling "make_pkt" function.
				udt_send(pkt) #It calls "udt_send" function for send packet
				nextseqnum += 1 #Increment next squence number by one after each new packet sent.
			except:
				continue #If another threads is using this queue, continue infinite loop to try again.
		else: #If there is no unsent packets into window, finish the function.
			break

#This function calculates checksum, put it into payload and choose router. After these, it calls "sendR1" or "sendR2" functions according to choosen router.
def udt_send(data):
	rndm = randint(1, 2) #Selects number randomly between 1 and 2. If "1" is choosen, it means packets will go R1, else packets will go R2.
	checksum = md5.new() #Generates new "MD5" object to calculate checksum.
	checksum.update(str(data)) #Updates the "MD5" object wtih the payload of packet to calculate checksum.
	data += checksum.digest() #Checksum value obtained with this method.
	if rndm == 1:
		sendR1(data) #Calls "sendR1" function if "rndm" value equals "1".
	else:
		sendR2(data) #Calls "sendR2" function if "rndm" value equals "2".

#This function listens the Source Node and puts received payloads into queues. It treats like Tcp Server.
def listenSource():
	global msgs #Gets global "msgs" queue.
	host = '10.10.1.2' #IP Address of Broker Node.
	port = 5000 #Port Number of Broker Node.

	s = socket.socket() #Creates a socket object.
	s.bind((host, port)) #Binds created socket object to Broker Node.

	s.listen(1) #Allows 1 connection to listen.
	c, addr = s.accept() #Accepts TCP connection.
	i = 0
	while(1):
		i += 1
		data = c.recv(500) #Receives 500 bytes of data.
		if not data:
			break
		print i
		while 1:
			try:
				msgs.put_nowait(data) #If another threads do not use "msgs" queue, puts received data into this queue.
				break
			except:
				#print('SOMETHING with msgs') 
				continue #If another threads use "msgs" queue, continue infinite loop to try again.
		while 1:
			try:
				sndpkt.put_nowait(data) #If another threads do not use "sndpkt" queue, puts received data into this queue.
				break
			except:
				#print('SOMETHING with sndpkt')
				continue #If another threads use "sndpkt" queue, continue infinite loop to try again.
	c.close() #Close the connection after data transfer that between Source Node and Broker Node ends.

#This function sends packets to Destination Node via Router 1, R1.
def sendR1(message):
	global s1 #Gets global socket object for "b-r1-d" path, "s1".
	global f
	start_timer() #Starts to timer to timeout mechanism.
	s1.sendto(message, ('10.10.3.2', 5000)) #Sends packets "d interface" of link which is on between Router 1 and Destination Node.

#This function sends packets to Destination Node via Router 2, R2.
def sendR2(message):
	global s2 #Gets global socket object for "b-r2-d" path, "s1".
	global f
	start_timer() #Starts to timer to timeout mechanism.
	s2.sendto(message, ('10.10.5.2', 5000)) #Sends packets "d interface" of link which is on between Router 2 and Destination Node.

#This function listen Destination Node. It receives acknowledgement comes from Destination Node via R1.
def listenR1():
	global acks #Gets global "acks" queue, "acks".
	global s1 #Gets global socket object for "b-r1-d" path, "s1".
	global t0 #Gets global time value, "t0".
	s1.bind(('10.10.2.1', 5001)) #Binds "s1" socket object to Broker Node.
	while(1):
		data, addr = s1.recvfrom(6) #Receives 6 digit acknowledgement.
		if not data:
			print('NOT DATA')
			break
		while(1):
			try:
				ack = int(str(data)) #Converts received acknowledgement number to integer.
				# print('ACK r1 '+str(ack))
				acks.put_nowait(ack) #If another threads do not use "acks" queue, puts received acknowledgement into this queue.
				break
			except(e):
				print(e)
	s1.close()

#This function listen Destination Node. It receives acknowledgement comes from Destination Node via R2.
def listenR2():
	global acks #Gets global "acks" queue, "acks".
	global s2 #Gets global socket object for "b-r2-d" path, "s2".
	s2.bind(('10.10.4.1', 5002)) #Binds "s2" socket object to Broker Node.
	while(1):
		data, addr = s2.recvfrom(6) #Receives 6 digit acknowledgement.
		if not data:
			break
		while(1):
			try:
				ack = int(str(data)) #Converts received acknowledgement number to integer.
				# print('ACK r2 '+str(ack))
				acks.put_nowait(ack) #If another threads do not use "acks" queue, puts received acknowledgement into this queue.
				break
			except(e):
				print(e)
	s2.close()

def Main():
	threading.Thread(target=listenSource).start() #Creates a thread for "listenSource" function to listen Source Node simultaneously and starts created thread.
	threading.Thread(target=listenR1).start() #Creates a thread for "listenR1" function to receives acknowledgements which come from Destination Node via R1 simultaneously and starts created thread.
	threading.Thread(target=listenR2).start() #Creates a thread for "listenR1" function to receives acknowledgements which come from Destination Node via R2 simultaneously and starts created thread.
	start_timer() #Start timer for first window.
	rdt_send() #Sends packets which into first window.
	rdt_rcv() #After "fin" packet send, main process waits acknowlegements and calls "rdt_send" for each received acknowledgement with calling this function.

if __name__ == '__main__':
	Main()
