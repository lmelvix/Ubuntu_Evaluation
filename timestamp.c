/* CPU TIMESTAMP OVERHED MEASUREMENT USING RDTSC */
#include<stdio.h>
#include<time.h>
#include<sys/time.h>
#include<unistd.h>
#include<sys/syscall.h>
#include<errno.h>
#include<stdlib.h>

typedef unsigned long long ticks;

static inline ticks getticks() {
	unsigned hi, lo;
  	__asm__ volatile("rdtsc" : "=a" (lo), "=d" (hi));
  	return ((ticks)hi << 32) | lo;
}

void proc_call0() {}
void proc_call1(int one){}
void proc_call2(int one, int two){}
void proc_call3(int one, int two, int three){}
void proc_call4(int one, int two, int three, int four){}
void proc_call5(int one, int two, int three, int four, int five){}
void proc_call6(int one, int two, int three, int four, int five, int six){}
void proc_call7(int one, int two, int three, int four, int five, int six, int seven){}

int main() {
	int r = 0;
	int pid;
	ticks tick, tick1, tickh;
	float time = 0.0;
	unsigned cycles = 0;
	int num_of_samples = 20000;
	float timestamp_overhead;
	float loop_overhead;
	float procedure_overhead;
	float systemcall_overhead;
	float task_overhead;
	int sum = 0;
	int sample;
	int subsample;
	

		
	/* Warm-Up */
	for(sample = 0; sample <= num_of_samples; sample++){
		tick=getticks();
	}
 
	printf("\n**CPU MEASUREMENT**\n");
	
	/* TIMESTAMP READING MEASUREMENT */
	for(sample=0; sample <= num_of_samples; sample++) {
		tick=getticks();
		tick1=getticks();
		sum += (unsigned)(tick1 - tick);
	}
	timestamp_overhead = sum/num_of_samples;
	printf("\nAverage CPU Timestamp overhead \t:\t%f",timestamp_overhead);
        
	/* LOOP OVERHEAD MEASUREMENT */
	tick = getticks();
	for(subsample=0; subsample <= num_of_samples; subsample++){
	}
	tick1 = getticks();
	loop_overhead = (tick1-tick)/num_of_samples;
	printf("\nAverage CPU Loop overhead \t:\t%f\n",loop_overhead);

	/* PROCEDURE CALL OVERHEAD */
	printf("\n PROCEDURE CALL OVERHEAD :");
	/* ZERO ARGUMENTS */
	tick = getticks();
	for(sample=0;sample <= num_of_samples; sample++) {
		proc_call0();
	}
	tick1 = getticks();
	procedure_overhead = (tick1 - tick)/num_of_samples;
	printf("\n 0 Arguments\t:\t%f",procedure_overhead);

        /* ONE ARGUMENTS */
        tick = getticks();
        for(sample=0;sample <= num_of_samples; sample++) {
                proc_call1(1);
        }
        tick1 = getticks();
        procedure_overhead = (tick1 - tick)/num_of_samples;
        printf("\n 1 Argument\t:\t%f",procedure_overhead);	

        /* TWO ARGUMENTS */
        tick = getticks();
        for(sample=0;sample <= num_of_samples; sample++) {
                proc_call2(1,2);
        }
        tick1 = getticks();
        procedure_overhead = (tick1 - tick)/num_of_samples;
        printf("\n 2 Arguments\t:\t%f",procedure_overhead);
	
	/* THREE ARGUMENTS */
        tick = getticks();
        for(sample=0;sample <= num_of_samples; sample++) {
                proc_call3(1,2,3);
        }
        tick1 = getticks();
        procedure_overhead = (tick1 - tick)/num_of_samples;
        printf("\n 3 Arguments\t:\t%f",procedure_overhead);

        /* FOUR ARGUMENTS */
        tick = getticks();
        for(sample=0;sample <= num_of_samples; sample++) {
                proc_call4(1,2,3,4);
        }
        tick1 = getticks();
        procedure_overhead = (tick1 - tick)/num_of_samples;
        printf("\n 4 Arguments\t:\t%f",procedure_overhead);

        /* FIVE ARGUMENTS */
        tick = getticks();
        for(sample=0;sample <= num_of_samples; sample++) {
                proc_call5(1,2,3,4,5);
        }
        tick1 = getticks();
        procedure_overhead = (tick1 - tick)/num_of_samples;
        printf("\n 5 Arguments\t:\t%f",procedure_overhead);

        /* SIX ARGUMENTS */
        tick = getticks();
        for(sample=0;sample <= num_of_samples; sample++) {
                proc_call6(1,2,3,4,5,6);
        }
        tick1 = getticks();
        procedure_overhead = (tick1 - tick)/num_of_samples;
        printf("\n 6 Arguments\t:\t%f",procedure_overhead);

        /* SEVEN ARGUMENTS */
        tick = getticks();
        for(sample=0;sample <= num_of_samples; sample++) {
                proc_call7(1,2,3,4,5,6,7);
        }
        tick1 = getticks();
        procedure_overhead = (tick1 - tick)/num_of_samples;
        printf("\n 7 Arguments\t:\t%f\n",procedure_overhead);
	
	/*SYSTEM CALL OVERHEAD */
	/* GETPID */
        printf("\n System Call Overhead \n");	
	tick = getticks();
	for(sample=0;sample <= num_of_samples; sample++) {
		getpid();
	}
	tick1 = getticks();
	systemcall_overhead = (tick1 - tick)/num_of_samples;
	printf(" GetPID \t:\t%f\n", systemcall_overhead);

        /* GETPPID */
        tick = getticks();
        for(sample=0;sample <= num_of_samples; sample++) {
                getppid();
        }
        tick1 = getticks();
        systemcall_overhead = (tick1 - tick)/num_of_samples;
        printf(" GetPPID \t:\t%f\n", systemcall_overhead);

	/* GETUID */
        tick = getticks();
        for(sample=0;sample <= num_of_samples; sample++) {
                getuid();
        }
        tick1 = getticks();
        systemcall_overhead = (tick1 - tick)/num_of_samples;
        printf(" GetUID \t:\t%f\n", systemcall_overhead);
	
        /* GETGID */
        tick = getticks();
        for(sample=0;sample <= num_of_samples; sample++) {
                getgid();
        }
        tick1 = getticks();
        systemcall_overhead = (tick1 - tick)/num_of_samples;
        printf(" GetGID \t:\t%f\n", systemcall_overhead);

	/* TASK CREATION */
	printf("\nTASK CREATION OVERHEAD: \n");
	sum = 0;
	for(sample=0; sample <= num_of_samples; sample++) {
		tick = getticks();
		pid = fork();
		if (pid < 0 ) {
			exit(0);
		}
		if (pid == 0 ) {
			exit(1);
		}
		else wait(NULL);
		tick1 = getticks();
		sum += (tick1 - tick);
	}
	task_overhead = sum/num_of_samples;
	printf(" Process Measurement\t:\t%f\n",task_overhead); 

	return 0;
}

