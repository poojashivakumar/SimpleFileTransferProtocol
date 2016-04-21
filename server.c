/* server code */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_RECEIVE_SIZE 5 	

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[MAX_RECEIVE_SIZE];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     FILE *fp; /* File pointer to the output file */
    
     /* Takes in the port number as argument */

     if (argc < 2) 
	{
        	 printf("Please provide the port number\n");
         	exit(0);
     	}
    
     /* Establish a TCP connection */

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
     if (sockfd < 0) 
	{
        	printf("socket open failed\n");
		exit(0);
     	}

     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
	{
              printf("bind to the socked failed \n");
		exit(0);
     	}
 
     /* Listen to the incoming connection */

     listen(sockfd,1);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

     if (newsockfd < 0) 
	{
          printf("accept failed \n");
	  exit(0);
     	}

     /* read the length of the file and the filename */
     int temp, len;
     char output_file[256];
     n = read(newsockfd, &temp, sizeof(temp));

     if (n < 0) 
	{
		printf("reading of length of file failed \n");
       		exit(0);
     	}

     len = ntohl(temp);
     
     n = read(newsockfd, output_file, len);
     if (n < 0) 
	{
        	printf("reading of filename failed \n");
        	exit(0);
     	}
     
     output_file[len] = 0;

     /* zero the buffer */
     bzero(buffer, MAX_RECEIVE_SIZE);

     /* Open the output file for writing */
     fp = fopen(output_file, "wb");
     long i = 0;
     int block_size = 0;

     while ((block_size = recv(newsockfd, buffer, MAX_RECEIVE_SIZE, 0)) > 0) 
	{
     		fwrite(buffer, sizeof(char), block_size, fp);
        
        	i = i + block_size;
        	printf("Bytes written = %ld\n", i);
        	bzero(buffer, MAX_RECEIVE_SIZE);
     	}        
	

     /* Close the sockets and the file */
     fclose(fp);
     close(newsockfd);
     close(sockfd);

     return 0; 
}



