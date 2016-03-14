#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

typedef unsigned long long tick;
static __inline__ tick gettick (void) {
	    unsigned a, d;
	    __asm__ __volatile__("rdtsc": "=a" (a), "=d" (d) );
	    return (((tick)a) | (((tick)d) << 32));
	}

int main(int argc, char *argv[])
{
	int sockRes;
        char* sendBuf;
	struct hostent *host;
        long bufSize, arrLen;
	struct sockaddr_in serverAddress;
	long maxIterations = 1000;
	long count,i = 0;
	long long t1, t2 = 0;
        long double t;
        

        char sBuffer[2], rBuffer[2]; 
  

	host = gethostbyname("10.0.0.2");
	if ((sockRes = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "socket creation Failure!\n");
		exit(1);
	}

	serverAddress.sin_family = AF_INET;     
	serverAddress.sin_port = htons(2154);   
	serverAddress.sin_addr = *((struct in_addr *)host->h_addr_list[0]);
	bzero(&(serverAddress.sin_zero),8); 
	t1 = gettick();
	
if (connect(sockRes, (struct sockaddr *)&serverAddress,
                    sizeof(struct sockaddr)) == -1) 
	{
		fprintf(stderr, "connect failed\n");
		exit(1);
	}

	sBuffer[0] = 'b';
	sBuffer[1] = '\0';


		send(sockRes, sBuffer, 2, 0);
		recv(sockRes, rBuffer, 2, 0);
		rBuffer[1] = '\0';
        
	t2 = gettick();

	sBuffer[0] = 'q';
	send(sockRes,sBuffer,strlen(sBuffer), 0);

	long double overhead = ((long double) (t2 - t1)) / ((long double) maxIterations);
	
	printf("Connection setup = %.2LF us\n", overhead / (2.4e3));


	fflush(stdout);

	t1 = gettick();
	close(sockRes);
	t2 = gettick();


	t = ((long double) bufSize) / (((long double) (t2 - t1)) - overhead);
	printf("Connection teardown = %.2LF us\n", overhead / (2.4e3));
	//printf("Bandwidth = %.2LF MB/sec\n", t * (2.4e9/1.0e6));
	return 0;
}
