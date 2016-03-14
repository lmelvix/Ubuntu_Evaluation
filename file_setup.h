#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<time.h>
#include<sys/time.h>
#include<unistd.h>
#include<stdint.h>
#include<sys/syscall.h>
#include<errno.h>
#include<pthread.h>
#include<math.h>


#define UCSD_IP "127.0.0.1"
#define PORT 8000
#define NUM_SAMPLES 20000
#define CPU_FREQUENCY 2100
typedef unsigned long long ticks;

static inline ticks getticks() {
        unsigned hi, lo;
        __asm__ volatile("rdtsc" : "=a" (lo), "=d" (hi));
        return ((ticks)hi << 32) | lo;
}

long double standard_deviation(long double std[],int num_of_samples) {
        long double sum = 0.0;
        int i;
        long double avg =0.0;
        long double deviation = 0.0;

        for(i=0;i<num_of_samples;i++) {
                sum += std[i];
        }
        avg = sum/num_of_samples;
        for(i=0;i<num_of_samples;i++) {
                deviation += ((long double)std[i] - avg)*((long double)std[i] - avg);
        }
        deviation= deviation/num_of_samples;
        deviation= sqrt (deviation);
        return deviation;
}

long double mean(long double std[], int num_of_samples){
        long double sum = 0.0;
        int i;
   long double mean = 0.0;

        for(i=0;i<num_of_samples; i++) {
                sum += std[i];
        }
        mean = sum/num_of_samples;
        return (mean);
}

