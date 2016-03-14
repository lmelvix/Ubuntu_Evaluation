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


long double standard_deviation(long double std[],int num_of_samples){
        long double sum = 0;
        int i;
        long double avg =0.0;
        long double deviation = 0.0;

        for(i=0;i<num_of_samples;i++) {
                sum += std[i];
        }
        avg = sum/num_of_samples;
        for(i=0;i<num_of_samples;i++) {
                deviation += ((float)std[i] - avg)*((float)std[i] - avg);
        }
        deviation=deviation/num_of_samples;
        deviation= sqrt (deviation);
        return deviation;
}

long double mean(long double std[], int num_of_samples){
	long double sum = 0;
	int i;
	long double mean = 0.0;

	for(i=0;i<num_of_samples; i++) {
		sum += std[i];
	}
	mean = sum/num_of_samples;
	return (mean);
}

int main (int argc, char *argv[]) {

	long long unsigned int start_t , end_t ;
	long long unsigned int total_t =0;
	int num_of_samples = 20000;
	long double stat[20000];
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
	for(j=0 ; j<num_of_samples; j++) {
		start_t = getticks();
		temp = a1[j+ i*8*1024];
		j = j * 2;
		end_t = getticks();
		total_t += (end_t - start_t);
		stat[j] = (long double)(end_t - start_t);
	}
	munmap(a1, SIZE_OF_MEMORY);
 	munmap(a2, SIZE_OF_MEMORY);
 	printf("\nPage FAULT\n Average\t:\t%LF ns \t Standard Deviation\t:\t%LF\n", (mean(stat,num_of_samples)/2.1),standard_deviation(stat,num_of_samples));
}
