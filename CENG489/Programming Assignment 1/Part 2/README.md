# Part 2 Tester

#### How to run:
  + ### !!! MAKE BACKUPS !!!
  + Put "`tester_part2.py`" into the directory of "`AESAttack.py`" and "`vulnerable.py`"
  
  	 	[test@test ~]$ ls
        AESAttack.py tester_part2.py vulnerable.py
  + [*NEW*] Tester now handles two flags
    -h : This passes the original ciphertext to AESattack.py as a hex string of length 96 (e.g. "01020304..2e2f30", two ascii chars per byte)
    -f : This flag tells the tester to read the output of AESAttack.py from a "ciphertext.txt" file.
    If yout omit these flags the defaults are assumed. (ciphertext passed in as raw bytes and output is read from stdout).
    You can use flags seperately.
  + Run "`tester_part2.py`" with `python2`. It is recommended that you run it in a wide teminal
  the output is pretty long.
  
        [test[at]test ~]$ python2 tester_part2.py [-h] [-f]
        [test[at]text ~]$ python2 tester_part2.py -h      # Ciphertext as hex string, output to stdout
        [test[at]text ~]$ python2 tester_part2.py -f      # Ciphertext as raw bytes, output to file
        [test[at]text ~]$ python2 tester_part2.py -h -f   # Ciphertext as hex string, output to file
        [test[at]text ~]$ python2 tester_part2.py         # Ciphertext as raw bytes, output to stdout

  + See your score.

#### Output fields and their meanings:
<b>No</b>: Test number  
<b>Desired(Hex)</b>: The hexadecimal representation of characters in the desired string (DP).
If the DP was "AAA" then the Desired(Hex) is "41414101".  
<b>Output(Hex)</b>: Last N characters of the decryption of your output. Should be equal to Desired(Hex) column. Representaion format is the same.  
<b>Key(Hex)</b>: Hexadecimal representaion of the key used in AES.  
<b>IV(Hex)</b>: Hexadecimal representation of the IV used in AES.  
<b>Comment</b>: Result of the test case. "+" means everything is OK.  

For bugs & hugs please use Piazza

-hbostann

