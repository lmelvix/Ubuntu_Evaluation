#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#define KB (1024u)
#define MB (1024u * KB)
#define GB (1024u * MB)
#define SIZE_OF_MEMORY (2u* GB)  
 
typedef unsigned long long ticks;

static inline ticks getticks() {
        unsigned hi, lo;
        __asm__ volatile("rdtsc" : "=a" (lo), "=d" (hi));
        return ((ticks)hi << 32) | lo;
}

int main (int argc, char *argv[]) {

	long long unsigned int start_t , end_t ;
	long long unsigned int total_t =0;
	int  i,j, fd1, fd2;
	char *a1 , *a2 ,temp;

	// Open these individual files
    	if ((fd1 = open(argv[1], O_RDONLY)) == -1) {
        	printf("\nUnable to open File 1\n");
        	exit(EXIT_FAILURE);
    	}

    	if ((fd2 = open(argv[2], O_RDONLY)) == -1) {
        	printf("\nunable to open File 2\n");
        	exit(EXIT_FAILURE);
    	}	

    	posix_fadvise(fd1, 0, 10000000, POSIX_FADV_RANDOM);
    	posix_fadvise(fd2, 0, 10000000, POSIX_FADV_RANDOM);

    	a1 = (char *) mmap(0, SIZE_OF_MEMORY, PROT_READ, MAP_POPULATE | MAP_PRIVATE, fd1, 0);
    	if (a1 == MAP_FAILED){
        	printf("mmap 1 error");
		exit(EXIT_FAILURE);
    	}
    	a2 = (char *) mmap(0, SIZE_OF_MEMORY, PROT_READ, MAP_POPULATE | MAP_PRIVATE, fd2, 0);
    	if (a2 == MAP_FAILED){
      		printf("mmap 2 error");
		exit(EXIT_FAILURE);
    	}

	madvise(a1, 0, MADV_RANDOM);
	madvise(a2, 0, MADV_RANDOM);
    	i = 16;
	for(j=0 ; j<10; j++) {
		start_t = getticks();
		temp = a1[j+ i*8*1024];
		j = j * 2;
		end_t = getticks();
		total_t += (end_t - start_t);
	}
	munmap(a1, SIZE_OF_MEMORY);
 	munmap(a2, SIZE_OF_MEMORY);
 	printf("Time required to service a page faut is %f \n", (total_t/10)/2.1);
}
