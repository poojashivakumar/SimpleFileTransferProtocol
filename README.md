# SimpleFileTransferProtocol

- This project consists of building an SFTP (Simple File Transfer Protocol). 
- It consists of a client and a server that exchange one file using TCP. 
- The client accepts 4 arguments: the name of the two files (<input> and <output>), and the IP address (or Internet name) 
and the port number of the server. The server starts first and waits for a connection request. 
- The client requests a connection and then sends the name of the file <output> for output and the data in file <input> 
to the server, which saves the info received in the file <output>. 
- The client reads the file and sends the data in chunks of 10 bytes. After sending the file, the client closes the 
connection and exits. 
- The server receives the data and writes the file in chunks of 5 bytes.
- The server needs to know when the transmission is over so that it can close the file. 
- After executing, <input> and <output> looks the same. 
- SFTP is built on top of TCP and can trasfer text and binary files. 

This project is coded in C language and uses socket libraries.


