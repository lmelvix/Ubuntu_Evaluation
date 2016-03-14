/* MEMORY OVERHEAD MEASUREMENT USING RDTSC */
#include<stdio.h>
#include<time.h>
#include<sys/time.h>
#include<unistd.h>
#include<stdint.h>
#include<sys/syscall.h>
#include<errno.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>

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

int main() {
        unsigned long jump,arr_size,i,j,k;
        ticks tick, tick1,tick0, tickh;
        int num_of_samples = 10;
        int sample, subsample;
	unsigned long  data;
        long double stat[10];
	long double overhead = 0, ram_access, ram_bw, read_bw, write_bw;
	long double bw_overhead = 0.0;
        double access_overhead = 0.0;
	long *access_array;
	long double sum = 0.0;

	/* Warm-Up */
        for(sample = 0; sample <= num_of_samples*num_of_samples; sample++){}
	
	/* RAM ACCESS MEASUREMENT */
/*	for (jump = 0; jump <= 8; jump++) {
		stride = pow(2,jump);
		for (arr_size = 9; arr_size <= 28; arr_size++) {
			unsigned long max_size = (1<<arr_size)/sizeof(long);			
			access_array = (long *)malloc(1<<arr_size);
			for (k = 0; k < max_size; k++){
				access_array[k] = (long)(&access_array[(k+stride)%max_size]);
			}
			read_array = (long *) &access_array;
			for (sample = 0; sample < num_of_samples; sample++) {
				tick = getticks();
				for (subsample = 0; subsample < 10000000; subsample++){}		
				tick1 = getticks();				
				overhead = (long double)(tick1 - tick);

				tick = getticks();
				for (subsample = 0; subsample < 10000000; subsample++){
					read_array = (long *) *read_array;
				}					
				tick1 = getticks();
				ram_access = (long double)(tick1 - tick);
				stat[sample] = (long double)((ram_access - overhead)/10000000);
			}
			printf("\n Stride : %ldb \t Array Size : %dkB \t RAM Access : %LF cycles \t Deviation : %LF"
				,(stride*8), (arr_size),mean(stat,num_of_samples), standard_deviation(stat,num_of_samples));
		}
	}
*/
	/* RAM BANDWIDTH MEASUREMENT */
	unsigned long read_size = (1<<24)/sizeof(long); // 64MB access array
	unsigned long arr_element;
	unsigned int access_element = 0;
	long stride = 1000;
	unsigned long counter = 0; 
	long *read_array = (long *)malloc(1<<24);
	for(arr_element=0;arr_element<read_size;arr_element++) {
		read_array[arr_element] = 1;
	}

	for(sample=0;sample<num_of_samples;sample++) {
		stride = (1<<22)/sizeof(long); // 2MB stride size 
		tick = getticks();
		for (arr_element=0;arr_element<read_size;arr_element++) {
			i = 0;
			/* READ 64 times by Loop unrolling */
			access_element = (access_element+ arr_element + stride*i)%read_size;
			sum += read_array[access_element];
			i++;

			access_element = (access_element+ arr_element + stride*i)%read_size;
			sum += read_array[access_element];
			i++;

			access_element = (access_element+ arr_element + stride*i)%read_size;
			sum += read_array[access_element];
			i++;

			access_element = (access_element+ arr_element + stride*i)%read_size;
			sum += read_array[access_element];
			i++;

			access_element = (access_element+ arr_element + stride*i)%read_size;
			sum += read_array[access_element];
			i++;

			access_element = (access_element+ arr_element + stride*i)%read_size;
			sum += read_array[access_element];
			i++;

			access_element = (access_element+ arr_element + stride*i)%read_size;
			sum += read_array[access_element];
			i++;

			access_element = (access_element+ arr_element + stride*i)%read_size;
			sum += read_array[access_element];
			i++;

			access_element = (access_element+ arr_element + stride*i)%read_size;
			sum += read_array[access_element];
			i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;
                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

                        access_element = (access_element+ arr_element + stride*i)%read_size;
                        sum += read_array[access_element];
                        i++;

		}      
		tick1 = getticks();
		ram_bw = (long double)(tick1 - tick);
		stat[sample] = (long double)(ram_bw);
	}
	overhead = mean(stat,num_of_samples)/64;
	data = (long double)(read_size * sizeof(long));	
	read_bw = (long double)(data/(overhead*0.47));
	printf("\n\nRAM BANDWIDTH:\n");
	printf("Average READ time\t:\t%LFns\tStd Deviation\t:\t%LF\tRead Bandwidth\t:\t%LF \n",
		(overhead * 0.47),(long double)(standard_deviation(stat,num_of_samples)*0.47/64),read_bw);	

	access_element = 0;

	for(sample=0;sample<num_of_samples;sample++) {
                 stride = (1<<22)/sizeof(long); // 2MB stride size 
                 tick = getticks();
                 for (arr_element=0;arr_element<read_size;arr_element++) {
                         i = 0;
			 access_element = (access_element + arr_element + stride*i)%read_size;
			 read_array[access_element] = 100;
			 i++;
		
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
             
                         access_element = (access_element + arr_element + stride*i)%read_size;
                         read_array[access_element] = 100;
                         i++;
		}
                tick1 = getticks();
                write_bw = (long double)(tick1 - tick);
                stat[sample] = (long double)(write_bw);
        }
        overhead = mean(stat,num_of_samples)/64;
        write_bw = (long double)((data/overhead)*2.1);
        printf("Average WRITE time\t:\t%LFns\tStd Deviation\t:\t%LF\tWrite Bandwidth\t:\t%LF \n",
                (overhead * 0.47),(long double)(standard_deviation(stat,num_of_samples)*0.47/64),write_bw);
	
	return 0;
}

