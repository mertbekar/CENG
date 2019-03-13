import os, threading
import socket
import time
import csv
import sys

delays = []

#It Creates .CSV File And Writes 'delay' Array Which Stores End-To-End Delays For Each Packet Into .CSV File.
def create_histogram(values):
	values = [int(round(val)) for val in values] #It Cast Delays To Integer.
	histogram = [0 for val in range(max(values) + 10)] #Initial Histogram Started With Zero Values.
	for val in values:
		histogram[val] += 1 #Counts Number Of Delays.
	with open("histogram_1.csv", "wb") as resultFile:
		wr = csv.writer(resultFile, dialect="excel")
		wr.writerow(histogram)
	with open("delays_1.csv", "wb") as resultFile:
		wr = csv.writer(resultFile, dialect="excel")
		wr.writerow(delays)

def receive(host):
	port = 5000 #Port Number Of Broker To Listen R1 or R2 Device.

	s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) #Socket Object Initialization of UDP.
	s.bind((host, port)) #Binds To Gets Packets From R1 or R2 Device.

	while 1: #Always Waits Packets From R1 or R2 Device.
		data, addr = s.recvfrom(1024) #Receives Message From R1 or R2 Device Up To 1024 Bytes.
		end = int(time.time() * 1000) #Generates Arrival Time of Packets In Terms Of Milisecond.
		data = str(data.decode('utf-8')) #Decodes Received Message.

		delay = end - int(data[:13]) #Calculates End-To-End Delay. It Gets Creation Time From Message Of Packet.
		delays.append(delay) #Appends End-To-End Delay .
		packet_idx = data[13:17] #Gets Packet ID From Message of Packet.

		print('Packet ID: ' + packet_idx + ', Delay: ' + str(delay) + ' ms')
		data += 'd_ACK_d' #Appends 'd_ACK_d' To Message.
		# s.sendto(data.encode('utf-8'), addr) #Sends Encoded Acknowladgement To R1 or R2 Devices.

		if int(packet_idx) == 1999:
			print('LAST PACKET')
			create_histogram(delays)
	s.close()

def Main():
	threading.Thread(target=receive, args=['10.10.3.2']).start()
	threading.Thread(target=receive, args=['10.10.5.2']).start()

if __name__ == '__main__':
    Main()
