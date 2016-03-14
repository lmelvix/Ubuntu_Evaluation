#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
#include<fcntl.h>
#include "file_setup.h"


void cache_access(int fd, int size) {
	
        int sample;
        char *buf[1024];
        ticks start, end;
        long double overhead, stat[10],std;
        long double cache_read_overhead, read_per_block;

        for (sample=0;sample<10;sample++) {
		start = getticks();
                while(read(fd,buf,1024) != 0);
                lseek(fd,0,SEEK_SET);
		end = getticks();
		stat[sample] = (long double)(end - start);
        }
        overhead = mean(stat,10);
	std = (long double)(standard_deviation(stat,10)/(size*1024*CPU_FREQUENCY));
        cache_read_overhead = (long double)(overhead/CPU_FREQUENCY);
	read_per_block = (long double)(cache_read_overhead/(size*1024));
        printf("Size : %d Mb\t Read Time : %LF us Read per block : %LF Standard Deviation : %LF\n",
		size, cache_read_overhead, read_per_block, std);
}


void main() {

	int fd;
	int sample;
	int size=1;
	char *buf[1024];
	ticks start, end;
	long double overhead;
	long double cache_read_overhead;
	
	printf("\n FILE CACHING: \n"); 
	fd = open("filem_1",O_RDONLY); 
	while(read(fd,buf,1024) !=0);
	cache_access(fd,1);

	fd = open("filem_2",O_RDONLY); 
        while(read(fd,buf,1024) !=0);
	cache_access(fd,10);

	fd = open("filem_3",O_RDONLY); 
        while(read(fd,buf,1024) !=0);
	cache_access(fd,20);

	fd = open("filem_4",O_RDONLY); 
        while(read(fd,buf,1024) !=0);
	cache_access(fd,40);
	
	fd = open("filem_5",O_RDONLY); 
        while(read(fd,buf,1024) !=0);
	cache_access(fd,80);	
	
	fd = open("filem_6",O_RDONLY); 
        while(read(fd,buf,1024) !=0);
	cache_access(fd,160);
	
	fd = open("filem_7",O_RDONLY);
        while(read(fd,buf,1024) !=0);
	cache_access(fd,320);	
	
	fd = open("filem_8",O_RDONLY); 
        while(read(fd,buf,1024) !=0);
	cache_access(fd,640);	
	
	fd = open("filem_9",O_RDONLY); 
        while(read(fd,buf,1024) !=0);
	cache_access(fd,1000);

	fd = open("filem_10",O_RDONLY); 
        while(read(fd,buf,1024) !=0);
	cache_access(fd,2000);	

	fd = open("file_3",O_RDONLY); 
        while(read(fd,buf,1024) !=0);
	cache_access(fd,3000);

	fd = open("file_4",O_RDONLY); 
        while(read(fd,buf,1024) !=0);
	cache_access(fd,4000);
	
	fd = open("file_5",O_RDONLY); 
        while(read(fd,buf,1024) !=0);
	cache_access(fd,5000);
	
	fd = open("file_6",O_RDONLY); 
        while(read(fd,buf,1024) !=0);
	cache_access(fd,6000);

	fd = open("file_7",O_RDONLY); 
        while(read(fd,buf,1024) !=0);
	cache_access(fd,7000);
	
	fd = open("file_8",O_RDONLY); 
        while(read(fd,buf,1024) !=0);
	cache_access(fd,8000);
}
