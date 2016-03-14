#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


int main(int argc, char *argv[])
{
        int sockRes = 1;
	int connectRes = 1;
	int bytesgot = 1;
	int sockOption = 1;
        long bufSize,arrLen,i; 
        char* recBuf; 
        char sendBuffer[50], receiveBuffer[50];       

	struct sockaddr_in servaddr,cliaddr;    

	if ((sockRes = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, " socket creation fail\n");
		exit(1);
	}

	if (setsockopt(sockRes,SOL_SOCKET,SO_REUSEADDR,&sockOption,sizeof(int)) == -1) {
		fprintf(stderr, "Could not set socket options!\n");
		exit(1);
	}

	servaddr.sin_family = AF_INET;         
	servaddr.sin_port = htons(2154);     
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	bzero(&(servaddr.sin_zero),8); 

	if (bind(sockRes, (struct sockaddr *)&servaddr, sizeof(struct sockaddr)) == -1) {
		fprintf(stderr, "bind() fail\n");
		exit(1);
	}

	if (listen(sockRes, 3) == -1) {
		fprintf(stderr, "listen fail\n");
		exit(1);
	}
		
	printf("\nWaiting \n");
	fflush(stdout);
	sendBuffer[0] = 'q';
	sendBuffer[1] = '\0';
	int _size = sizeof(struct sockaddr_in);

	connectRes = accept(sockRes, (struct sockaddr *)&cliaddr, (socklen_t*) &_size);

	printf("connection request received from (%s , %d) \n", inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
	fflush(stdout);

	for(;;)
	{
		bytesgot = recv(connectRes,receiveBuffer,2,0);
		receiveBuffer[1] = '\0';

		if (receiveBuffer[0] == 'q' || receiveBuffer[0] == 'Q')
		{
			printf("Recieved quit\n");
			fflush(stdout);
			break;
		} else {
			send(connectRes, receiveBuffer, 2, 0);
		}
	}
	
       bufSize = 1073741824; // 1GB
	arrLen = bufSize / ((long) sizeof(char));
       recBuf = (char*) malloc(bufSize);

	printf("\nWaiting or the 1GB array\n");
	fflush(stdout);

	bytesgot = recv(connectRes,recBuf,arrLen,0);
	while (bytesgot < bufSize) {
		bytesgot += recv(connectRes,recBuf,arrLen,0);
	}
	send(connectRes, sendBuffer, 2, 0);

	 i = 0;
	while ( recBuf[i] == 'a' ) {
		i++;
	}

	printf("recieved %li a's\n", i);

	close(connectRes);
	free(recBuf);
	close(sockRes);
	return 0;
}
