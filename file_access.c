#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include "file_setup.h"


void seq_access(int fd, int size, char size_type) {

    int sample,sample_count = 10;
	long double count = 1;
    char buf[4096] __attribute__((aligned(4096)));
	ssize_t data;
    ticks start, end;
    long double overhead;
    long double seq_read_overhead;
	long double stat[10],std;
	long int blocks, itr;

    for (sample=0;sample<sample_count;sample++) {
        start = getticks();
        while(1){
			data = read(fd,buf,4096);
			if(data == 0) {
                break;
			}
			count++;
		}
		end = getticks();
		stat[sample] = (long double)(end - start);
    }
    count = count/sample_count;
    overhead = (long double)(mean(stat,sample_count));
	std = (long double)(standard_deviation(stat,sample_count)/CPU_FREQUENCY);
    seq_read_overhead = (long double)(overhead/CPU_FREQUENCY);
    printf("File Size : %d %cb\t Access Time : %LF us \t Access time per block : %LF \t Standard Deviation : %LF \n",
            size, size_type, seq_read_overhead, (long double)(seq_read_overhead/count),(std/count));
}


void rand_access(int fd, long size, int print_size, char size_type) {

	int subsample,sample, count=1;
	off_t block_location=0;
	ssize_t data;
	off_t tot_blocks;
	char buf[4096] __attribute__((aligned(4096)));
        ticks start, end;
	long double overhead=0.0;
	long double rand_read_overhead=0.0;

	tot_blocks = (size>4096) ? (long int)(size/4096) : 1 ;
	for(sample=0;sample<10;sample++) {
		overhead = 0;
		for (subsample=0;subsample<tot_blocks;subsample++){
			block_location = ((rand()%(tot_blocks)));
			start=getticks();
			lseek(fd,block_location*4096,SEEK_SET);
			data = read(fd,buf,4096);
			end=getticks();
			overhead += (long double)(end-start);
		}
		rand_read_overhead += (long double)(overhead/(tot_blocks*CPU_FREQUENCY));
	}
	printf("File Size : %d %cb\t Total Blocks : %d\t Random Access Time: %LF\t Access Time per block: %LF \n",
            print_size, size_type, (int)tot_blocks,(rand_read_overhead/10)*tot_blocks,(rand_read_overhead/10));

}


void cont_access(int num_process) {
	char *fname[10] = {"filek_1","file_2","file_4","file_8","file_16","file_32","file_64","file_128","file_256","file_512"};
	int fsize = 32;
	int fblock[10] = {1,1,1,2,4,8,16,32,64,128};
	int process,file_num;
	int i;
	int fd, fd_parent;
	int sample,key;
	int data,pid;
	void *buf, *buf_parent;
	ticks start, end;
	long double cont_overhead, cont_block;
	long int parent_itr;

	for(process=0, file_num=0;process<num_process;process++,file_num++) {
		pid = fork();
		if(pid == 0) {
			int fd;
			int child_itr = fblock[file_num];
			fd = open(fname[file_num], O_RDONLY | O_DIRECT | O_SYNC);
			posix_fadvise(fd,0,0,POSIX_FADV_RANDOM);
			sleep(10);
			while(child_itr) {
				data = read(fd, buf, 4096);
				if (data <= 0)
					break;
				child_itr--;
			}
			close(fd);
			exit(0);
		}
	}
	sleep(1);
	wait(&key);

	fd_parent = open("filem_4",O_SYNC | O_RDONLY | O_DIRECT);
	posix_fadvise(fd_parent,0,0,POSIX_FADV_RANDOM);
	start = getticks();
	for(sample = 0; sample < 1000; sample++) {
		parent_itr = 10240;
		while(parent_itr) {
			data = read(fd_parent, buf_parent, 4096);
			if (data <= 0)
				break;
			parent_itr--;
		}
	}
	end = getticks();
	cont_overhead = (long double)((end - start)/CPU_FREQUENCY*1000);
	cont_block = (long double)(cont_overhead/10240);
	close(fd_parent);
	printf("Number of Processes : %d \t File Size : 40MB \t Access Time : %LF us \t Block Access Time : %LF us \n",
		num_process, cont_overhead, cont_block);

}


void main() {

    int fd;
	int sample;
    char buf[4096] __attribute__((aligned(4096)));
	long int warm_itr=10240;
    ssize_t data;
	long int kB = 1024;
	long int Mb = kB*1000;
	long int Gb = Mb*1000;

	// WARM UP //
	for (sample = 0; sample <= NUM_SAMPLES; sample++) {
		fd = open("filem_4",O_RDONLY | O_SYNC);
                lseek(fd,0,SEEK_SET);
                while(warm_itr){
                        data = read(fd,buf,4096);
			if(data ==  0)
                       		 break;
			warm_itr--;
                }
		close(fd);
	}

	printf("\n FILE ACCESS CONTENTION: \n");
	cont_access(0);
	cont_access(1);
	cont_access(2);
	cont_access(3);
	cont_access(4);
	cont_access(5);
	cont_access(6);
	cont_access(7);
	cont_access(8);
	cont_access(9);
	cont_access(10);

    printf("\nRANDOM FILE ACCESS: \n");
    fd = open("filek_1",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,(1*kB),1,'k'); close(fd);
	fd = open("filek_2",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,(2*kB),2,'k'); close(fd);
	fd = open("filek_4",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,4*kB,4,'k'); close(fd);
	fd = open("filek_8",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,8*kB,8,'k'); close(fd);
	fd = open("filek_16",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,16*kB,16,'k'); close(fd);
	fd = open("filek_32",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,32*kB,32,'k'); close(fd);
	fd = open("filek_64",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,64*kB,64,'k'); close(fd);
	fd = open("filek_128",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,128*kB,128,'k'); close(fd);
	fd = open("filek_256",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,256*kB,256,'k'); close(fd);
	fd = open("filek_512",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,512*kB,512,'k'); close(fd);
	fd = open("filem_1",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,1*Mb,1,'M'); close(fd);
    fd = open("filem_2",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,10*Mb,10,'M'); close(fd);
    fd = open("filem_3",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,20*Mb,20,'M'); close(fd);
    fd = open("filem_4",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,40*Mb,40,'M'); close(fd);
    fd = open("filem_5",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,80*Mb,80,'M'); close(fd);
    fd = open("filem_6",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,160*Mb,160,'M'); close(fd);
    fd = open("filem_7",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,320*Mb,320,'M'); close(fd);
    fd = open("filem_8",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,640*Mb,640,'M'); close(fd);
    fd = open("filem_9",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,1*Gb,1,'G'); close(fd);
    fd = open("filem_10",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,2*Gb,2,'G'); close(fd);
    fd = open("file_3",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,3*Gb,3,'G'); close(fd);
    fd = open("file_4",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,4*Gb,4,'G'); close(fd);
    fd = open("file_5",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,5*Gb,5,'G'); close(fd);
    fd = open("file_6",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,6*Gb,6,'G'); close(fd);
    fd = open("file_7",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,7*Gb,7,'G'); close(fd);
    fd = open("file_8",O_RDONLY | O_SYNC | O_DIRECT); rand_access(fd,8*Gb,8,'G'); close(fd);

	printf("\n SEQUENTIAL FILE ACCESS :\n");
	fd = open("filek_1",O_RDONLY | O_SYNC | O_DIRECT); seq_access(fd,1,'k'); close(fd);
	fd = open("filek_2",O_RDONLY | O_SYNC | O_DIRECT); seq_access(fd,2,'k'); close(fd);
	fd = open("filek_4",O_RDONLY | O_SYNC | O_DIRECT); seq_access(fd,4,'k'); close(fd);
	fd = open("filek_8",O_RDONLY | O_SYNC | O_DIRECT); seq_access(fd,8,'k'); close(fd);
	fd = open("filek_16",O_RDONLY | O_SYNC | O_DIRECT); seq_access(fd,16,'k'); close(fd);
	fd = open("filek_32",O_RDONLY | O_SYNC | O_DIRECT); seq_access(fd,32,'k'); close(fd);
	fd = open("filek_64",O_RDONLY | O_SYNC | O_DIRECT); seq_access(fd,64,'k'); close(fd);
	fd = open("filek_128",O_RDONLY | O_SYNC | O_DIRECT); seq_access(fd,128,'k'); close(fd);
	fd = open("filek_256",O_RDONLY | O_SYNC | O_DIRECT); seq_access(fd,256,'k'); close(fd);
	fd = open("filek_512",O_RDONLY | O_SYNC | O_DIRECT); seq_access(fd,512,'k'); close(fd);
	fd = open("filem_1",O_RDONLY | O_SYNC | O_DIRECT); seq_access(fd,1,'M'); close(fd);
    fd = open("filem_2",O_RDONLY | O_SYNC | O_DIRECT); seq_access(fd,10,'M'); close(fd);
    fd = open("filem_3",O_RDONLY | O_SYNC | O_DIRECT); seq_access(fd,20,'M'); close(fd);
    fd = open("filem_4",O_RDONLY | O_SYNC | O_DIRECT); seq_access(fd,40,'M'); close(fd);
    fd = open("filem_5",O_RDONLY | O_SYNC | O_DIRECT); seq_access(fd,80,'M'); close(fd);
    fd = open("filem_6",O_RDONLY | O_SYNC | O_DIRECT); seq_access(fd,160,'M'); close(fd);
    fd = open("filem_7",O_RDONLY | O_SYNC | O_DIRECT); seq_access(fd,320,'M'); close(fd);
    fd = open("filem_8",O_RDONLY | O_SYNC | O_DIRECT); seq_access(fd,640, 'M'); close(fd);
    fd = open("filem_9",O_RDONLY | O_SYNC | O_DIRECT); seq_access(fd,1,'G'); close(fd);
    fd = open("filem_10",O_RDONLY | O_SYNC | O_DIRECT); seq_access(fd,2,'G'); close(fd);
    fd = open("file_3",O_RDONLY | O_SYNC | O_DIRECT); seq_access(fd,3,'G'); close(fd);
    fd = open("file_4",O_RDONLY | O_SYNC | O_DIRECT ); seq_access(fd,4,'G'); close(fd);
    fd = open("file_5",O_RDONLY | O_SYNC | O_DIRECT ); seq_access(fd,5,'G'); close(fd);
    fd = open("file_6",O_RDONLY | O_SYNC | O_DIRECT ); seq_access(fd,6,'G'); close(fd);
    fd = open("file_7",O_RDONLY | O_SYNC | O_DIRECT ); seq_access(fd,7,'G'); close(fd);
    fd = open("file_8",O_RDONLY | O_SYNC | O_DIRECT ); seq_access(fd,8,'G'); close(fd);
}

