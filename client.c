/* Client code */
/* connect to the server, send the input file */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define MAX_TRANSMIT_SIZE 10	

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    FILE *fp; /* file pointer to read the input file */
    char buffer[MAX_TRANSMIT_SIZE];
    char output_file[256];
    int out_file_len = 0, temp;
    
    /* Make sure the client is receiving 4 arguments
       1. input file
       2. output file
       3. IP address of the server
       4. port number
    */
 
    if (argc < 5)
	 {
       		printf("Please enter the 4 parameters, input file, output file, 			IP address and port number\n");
       		exit(0);
   	 }

    
    /* Check whether the filenames are not NULL */

    if (argv[1] == NULL || argv[2] == NULL)	
	 {
       		printf("input / output filenames are not valid, exiting\n");
       		exit(0);
   	 }
   
    /* Get the server name and check if its not NULL */

    server = gethostbyname(argv[3]);

    if (server == NULL) 		
	{
        	printf("No host by such name\n");
       	 	exit(0);
   	 }

    /* Get the port number and check if can be opened, create a TCP socket */

    portno = atoi(argv[4]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
	 {
        	printf("can't open socket, error opening\n");
		exit(0);

   	 }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, 
		server->h_length);
    serv_addr.sin_port = htons(portno);

    /* Establish a connection to the server */

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
	 {
        	printf("connection failure\n");
		exit(0);
   	 }

     /* copy the output filename in argv[2] and write the length and filename 
       to socket */
     
     strcpy(output_file,argv[2]);    
     output_file[strlen(argv[2])] = 0;
     out_file_len = strlen(output_file);
     temp = htonl(out_file_len);
     
     /* Write the length of the file to socket */
     n = write(sockfd,&temp,sizeof(temp));
     if (n < 0)
	 {
 		 printf("writing to socket failed \n");
		 exit(0);
    	 } 
     
     /* Write the filename to the socket */
     n = write(sockfd, output_file, strlen(output_file));
     if (n < 0) 
	{
 		 printf("writing to socket failed \n");
		 exit(0);
     	} 
 
    /* zero the buffer and write the contents of input file to socket */
    bzero(buffer, MAX_TRANSMIT_SIZE);
    fp = fopen(argv[1], "rb");

    if (fp == 0) 
	{
        	printf("File cannot be opened or does not exist\n");
		exit(0);
   	 }

    int bz, i;
    while ((bz = fread(buffer, sizeof(char), MAX_TRANSMIT_SIZE, fp)) > 0) 
	{
    		n = send(sockfd, buffer, bz, 0);
        	bzero(buffer, MAX_TRANSMIT_SIZE);
    		if (n < 0) 
		{
        		 printf("writing to socket failed\n");
			 exit(0);
    		}
       	 	i = i + bz;
        	printf("Bytes sent = %d\n", i);
    	}

    close(sockfd);

    return 0;
}
