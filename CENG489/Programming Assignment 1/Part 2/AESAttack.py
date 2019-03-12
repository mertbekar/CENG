import vulnerable
import sys
from Crypto.Cipher import AES


C = str(sys.argv[1])
dp = str(sys.argv[2])
#plaintext = "plaintextplaintextplaintextplaintext"
#C = vulnerable.encr(plaintext)
#C = "aaaaaaaabbbbbbbbccccccccddddddddeeeeeeeeffffffff"

def Main():
	#valid = False
	T = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
	c = list(C)
	DP = list(dp)
	DP.append(chr(1))
	lenDP = len(DP)
	for i in range(0, lenDP):
		original = c[31-i]
		array = []
		index = -1
		for j in range(0, 256):
			c[31-i] = chr(j)
			if vulnerable.decr(''.join(c)) == "SUCCESS":
				array.append(j)
		for j in range(0, len(array)):
			if array[j] != ord(original):
				index = array[j]
				#print index
				break
		if index == -1:
			index = ord(original)
		c[31-i] = chr(index)
		T[15-i] = chr(ord(c[31-i])^(i+1))
		if i != lenDP-1:
			for j in range(0, i+1):
				c[31-j] = chr(ord(T[15-j])^(i+2))
	for i in range(0, lenDP):
		c[31-i] = chr(ord(T[15-i])^ord(DP[lenDP-i-1]))
	
	#out = ''.join(c)
	#f = open("cipher.txt", "w")
	#f.write(out)
	#f.close()
	sys.stdout.write(''.join(c))
	#print c
	#cipher = AES.new(vulnerable.key, AES.MODE_CBC, vulnerable.iv)
	#print cipher.decrypt(''.join(c))

if __name__ == '__main__':
    Main()
