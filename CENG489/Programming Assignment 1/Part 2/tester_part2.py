#!/usr/bin/python2
import string
import random
import subprocess
from Crypto.Cipher import AES
import shutil
import os
import traceback
import sys

BLACKLIST = chr(0)+'\n\r"\\'
FRM_STR = "| {:^3s} | {:^20s} | {:^20s} | {:^32} | {:^32} | {:s} "
INPUT_FORMAT_FLAG = "RAWBYTES"
OUTPUT_FORMAT_FLAG = "STDOUT"

def ContainsAny(s, l):
    return True in [c in s for c in l]

def pkcs7(plaintext):
  pad_length = 16 - len(plaintext) % 16
  pad = chr(pad_length) * pad_length
  return plaintext + pad

def GenerateKeyIVPair():
  key = "".join(chr(random.randint(0,255)) for _ in range(16))
  iv = "".join(chr(random.randint(0,255)) for _ in range(16))
  return (key, iv)

def GenerateText(s = 33, e=47):
  size = random.randint(s, e)
  text = "".join(random.choice(string.ascii_letters+string.punctuation+string.digits) for _ in range(size))
  #print text
  return text

def GetHex(t):
  return "".join("{:02x}".format(ord(c)) for c in t)

def GenerateCiphertext():
  key, iv = GenerateKeyIVPair()
  cipher = AES.new(key, AES.MODE_CBC, iv)
  plaintext = GenerateText()
  ciphertext = cipher.encrypt(pkcs7(plaintext))
  return ciphertext, key, iv

def CreateVulnerableScript(key, iv):
  f = open("vulnerable.py", "r").readlines()
  if f[0] != "# -*- coding: utf-8 -*-\n":
    f.insert(0, "# -*- coding: utf-8 -*-\n")
  for i in range(10):
    if f[i].startswith("key = \""):
      f[i] = "key = \""+key+"\"\n"
    if f[i].startswith("iv = \""):
      f[i] = "iv = \""+iv+"\"\n"
  a = open("vulnerable.py", "w")
  a.writelines(f)
  a.close()

def main():
    points = 0
    print FRM_STR.format("NO", "Desired(Hex)", "Output(Hex)", "Key(Hex)", "IV(Hex)", "Comment")
    print 150*"-"
    for run_no in range(100):
        if os.path.exists("vulnerable.pyc"):
          os.remove("vulnerable.pyc")
        
        desired = GenerateText(3,8)
        ciphertext, key, iv = GenerateCiphertext()
        while(ContainsAny(ciphertext, BLACKLIST) or ContainsAny(key, BLACKLIST) or ContainsAny(iv, BLACKLIST)):
            ciphertext, key, iv = GenerateCiphertext()

        CreateVulnerableScript(key, iv)
        
        if(INPUT_FORMAT_FLAG == "HEXSTRING"):
            ciphertext = GetHex(ciphertext)
        elif(INPUT_FORMAT_FLAG == "RAWBYTES"):
            pass # We already have ciphertext as raw bytes.

        p = subprocess.Popen(["python2", "AESAttack.py", ciphertext, desired], stdout=subprocess.PIPE)
        out, err = p.communicate()
        
        c = AES.new(key, AES.MODE_CBC, iv)

        if(OUTPUT_FORMAT_FLAG == "FILE"):
            out = open("ciphertext.txt", "rb").read()
        elif(OUTPUT_FORMAT_FLAG == "STDOUT"):
            pass # Default is to read from stdout.
        
        res = c.decrypt(out)
        desired += chr(1)

        if(res.endswith(desired)):
            points += 1
            print FRM_STR.format(str(run_no), GetHex(desired), GetHex(res[-(len(desired)):]), GetHex(key), GetHex(iv), "+")
        else:
            print FRM_STR.format(str(run_no), GetHex(desired), GetHex(res[-(len(desired)):]), GetHex(key), GetHex(iv), "Desired and Output doesn't match")
    print "You got {}/100!".format(points)


shutil.copyfile("vulnerable.py", "vulnerable.py.orig")
os.environ["PYTHONDONTWRITEBYTECODE"]="1"
try:
    if( "-h" in sys.argv):
        INPUT_FORMAT_FLAG = "HEXSTRING"
    if( "-f" in sys.argv):
        OUTPUT_FORMAT_FLAG = "FILE"
    main()
except Exception as e:
    traceback.print_exc()
finally:
    os.environ["PYTHONDONTWRITEBYTECODE"]="0"
    shutil.move("vulnerable.py.orig", "vulnerable.py")
