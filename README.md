# Public-Key-Stream-Cipher-Chat-Terminal

Created by Bryce Golamco.

## Instructions
To run the client/server chat terminal in linux:
```
make all
```
```
Execute ./client_output   (For the client-side)
```
```
Type in the IP address and the keys.
```
```
Execute ./serv_output     (For the server-side)
```
```
Type in a key for the server.
```
```
Ensure that both keys obtained are the same after the handshake is complete.
```

## Description

This chat terminal is encrypted using the Vernam Cipher with the key obtained from a Diffie-Hellman exchange.
This is only a one way chat terminal with the client sending messages to the server

## Other Notes
When choosing a key, note that you should choose 2-digit numbers as larger numbers may overflow on the 64-bits.
