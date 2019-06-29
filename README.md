
#ElGamal 

All thats needed to run the code is to have crypto++ installed on the host computer and making sure the paths are included when running the project.
The main.cpp is included which includes all the code for getting the text, converting it to ASCII decimal, encrypting and decrypting, and lastly changing the ASCII back to Plaintext.



#DES

This is the implementation of DES with CBC mode

The first input is the message that is going to be encrypted, it should be in plain text.
The second input is the 56bit or 8 character key used to encrypt the message.

The code will use the key, break up the plain text into groups of 64bits and encrypt each one using DES and linking them using CBC.
After the encryption is done and the encrypted message is printed on the screen, with a press of a button, the decryption process starts and the original message is displayed in binary and plain text again.

 