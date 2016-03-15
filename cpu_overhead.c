/* CPU TIMESTAMP OVERHED MEASUREMENT USING RDTSC */
/* Authors : Swetha, Sriram & Lenord 		 */

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
#include<sched.h>

typedef unsigned long long ticks;

static inline ticks getticks() {
	unsigned hi, lo;
  	__asm__ volatile("rdtsc" : "=a" (lo), "=d" (hi));
  	return ((ticks)hi << 32) | lo;
}

/* STANDARD DEVIATION */
double standard_deviation(unsigned std[],int num_of_samples){
	int sum = 0;
	int i;
	float avg =0.0;
	double deviation = 0.0;

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

void proc_call0() {}
void proc_call1(int one){}
void proc_call2(int one, int two){}
void proc_call3(int one, int two, int three){}
void proc_call4(int one, int two, int three, int four){}
void proc_call5(int one, int two, int three, int four, int five){}
void proc_call6(int one, int two, int three, int four, int five, int six){}
void proc_call7(int one, int two, int three, int four, int five, int six, int seven){}
void *thread_start(void *arg){}
void *thread_talk(void *arg){}

static inline void iproc_call0() {}
static inline void iproc_call1(int one){}
static inline void iproc_call2(int one, int two){}
static inline void iproc_call3(int one, int two, int three){}
static inline void iproc_call4(int one, int two, int three, int four){}
static inline void iproc_call5(int one, int two, int three, int four, int five){}
static inline void iproc_call6(int one, int two, int three, int four, int five, int six){}
static inline void iproc_call7(int one, int two, int three, int four, int five, int six, int seven){}

int main() {
	int r = 0;
	int pid;
	ticks tick, tick1, tickh;
	float time = 0.0;
	unsigned cycles = 0;
	struct sched_param process_sched;
	int count,num_of_samples = 20000;
	float timestamp_overhead;
	float loop_overhead;
	float procedure_overhead;
	float systemcall_overhead;
	float task_overhead;
	float thread_overhead;
	float context_proc_overhead;
	float context_thread_overhead;
	unsigned sum = 0;
	int sample;
	int subsample;
	int fd[2],fd1[2];
	pthread_t thread, thread1;
	unsigned stat[20000];
		
	/* Warm-Up */
	for(sample = 0; sample <= num_of_samples*num_of_samples; sample++){
		tick=getticks();
	}
	printf("\n**CPU MEASUREMENT**\n");
	
	/* TIMESTAMP READING MEASUREMENT */
	for(sample=0; sample <= num_of_samples; sample++) {
		tick=getticks();
		tick1=getticks();
		stat[sample] = (unsigned)(tick1 - tick);
		sum += (unsigned)(tick1 - tick);
	}
	timestamp_overhead = sum/num_of_samples;
	printf("\nAverage CPU Timestamp overhead \t:\t%f\tSD\t:%lf",timestamp_overhead,standard_deviation(stat,num_of_samples));
        
	/* LOOP OVERHEAD MEASUREMENT */
	tick = getticks();
	for(sample=0; sample <=num_of_samples; sample++) {
		tick = getticks();
		for(subsample=0; subsample <= num_of_samples; subsample++){
		}
		tick1 = getticks();
		stat[sample] = (tick1-tick)/num_of_samples;
		sum += (unsigned)(tick1-tick)/num_of_samples;
	} 
	loop_overhead = sum/num_of_samples;
	printf("\nAverage CPU Loop overhead \t:\t%f\tSD\t:%lf\n",loop_overhead,standard_deviation(stat,num_of_samples));

	/* PROCEDURE CALL OVERHEAD */
	printf("\n PROCEDURE CALL OVERHEAD :");
        /* WARM UP PROCEDURE CALL*/
        tick = getticks();
        for(sample=0;sample <= num_of_samples; sample++) {
                proc_call0();
        }
        tick1 = getticks();


	/* ZERO ARGUMENTS */
	sum = 0;
	for(sample=0;sample<=num_of_samples; sample++) {
		tick = getticks();
		for(subsample=0;subsample <= num_of_samples; subsample++) {
			proc_call0();
		}
		tick1 = getticks();
		stat[sample] = (tick1 - tick)/num_of_samples;
		sum += (unsigned)(tick1 - tick)/num_of_samples;
	}
	procedure_overhead = sum/num_of_samples;
	printf("\n 0 Arguments\t:\t%f \t Std deviation\t:\t %lf",procedure_overhead,standard_deviation(stat,num_of_samples));

        /* ONE ARGUMENTS */
       	sum = 0;
	for(sample=0;sample<=num_of_samples;sample++) {
	 	tick = getticks();
        	for(subsample=0;subsample <= num_of_samples; subsample++) {
                	proc_call1(1);
        	}
        	tick1 = getticks();
		stat[sample] = (tick1 - tick)/num_of_samples;
		sum += (unsigned)(tick1 - tick)/num_of_samples;
	}
        procedure_overhead = sum/num_of_samples;
        printf("\n 1 Argument\t:\t%f \t Std deviation\t:\t %lf",procedure_overhead,standard_deviation(stat,num_of_samples));	

        /* TWO ARGUMENTS */
	sum =0;
	for(sample=0;sample<= num_of_samples; sample++) {
        	tick = getticks();
        	for(subsample=0;subsample <= num_of_samples; subsample++) {
                	proc_call2(1,2);
        	}
        	tick1 = getticks();
        	stat[sample] = (tick1 - tick)/num_of_samples;
 		sum += (unsigned)(tick1 - tick)/num_of_samples;
	}
        procedure_overhead = sum/num_of_samples;
        printf("\n 2 Arguments\t:\t%f \t Std deviation\t:\t %lf",procedure_overhead,standard_deviation(stat,num_of_samples));
	
	/* THREE ARGUMENTS */
	sum=0;
	for(sample=0;sample<=num_of_samples;sample++) {
        	tick = getticks();
        	for(subsample=0;subsample <= num_of_samples; subsample++) {
                	proc_call3(1,2,3);
        	}
        	tick1 = getticks();
		stat[sample] = (tick1 - tick)/num_of_samples;	
		sum += (tick1 - tick)/num_of_samples;
	}
        procedure_overhead = sum/num_of_samples;
        printf("\n 3 Arguments\t:\t%f \t Std deviation\t:\t %lf",procedure_overhead,standard_deviation(stat,num_of_samples));

        /* FOUR ARGUMENTS */
        sum=0;
        for(sample=0;sample<=num_of_samples;sample++) {
                tick = getticks();
                for(subsample=0;subsample <= num_of_samples; subsample++) {
                        proc_call4(1,2,3,4);
                }
                tick1 = getticks();
                stat[sample] = (tick1 - tick)/num_of_samples;
                sum += (tick1 - tick)/num_of_samples;
        }
        procedure_overhead = sum/num_of_samples;
        printf("\n 4 Arguments\t:\t%f \t Std deviation\t:\t %lf",procedure_overhead,standard_deviation(stat,num_of_samples));


       /* FIVE ARGUMENTS */
        sum=0;
        for(sample=0;sample<=num_of_samples;sample++) {
                tick = getticks();
                for(subsample=0;subsample <= num_of_samples; subsample++) {
                        proc_call5(1,2,3,4,5);
                }
                tick1 = getticks();
                stat[sample] = (tick1 - tick)/num_of_samples;
                sum += (tick1 - tick)/num_of_samples;
        }
        procedure_overhead = sum/num_of_samples;
        printf("\n 5 Arguments\t:\t%f \t Std deviation\t:\t %lf",procedure_overhead,standard_deviation(stat,num_of_samples));

        /* SIX ARGUMENTS */
        sum=0;
        for(sample=0;sample<=num_of_samples;sample++) {
                tick = getticks();
                for(subsample=0;subsample <= num_of_samples; subsample++) {
                        proc_call6(1,2,3,4,5,6);
                }
                tick1 = getticks();
                stat[sample] = (tick1 - tick)/num_of_samples;
                sum += (tick1 - tick)/num_of_samples;
        }
        procedure_overhead = sum/num_of_samples;
        printf("\n 6 Arguments\t:\t%f \t Std deviation\t:\t %lf",procedure_overhead,standard_deviation(stat,num_of_samples));

        /* SEVEN ARGUMENTS */
        sum=0;
        for(sample=0;sample<=num_of_samples;sample++) {
                tick = getticks();
                for(subsample=0;subsample <= num_of_samples; subsample++) {
                        proc_call7(1,2,3,4,5,6,7);
                }
                tick1 = getticks();
                stat[sample] = (tick1 - tick)/num_of_samples;
                sum += (tick1 - tick)/num_of_samples;
        }
        procedure_overhead = sum/num_of_samples;
        printf("\n 7 Arguments\t:\t%f \t Std deviation\t:\t %lf",procedure_overhead,standard_deviation(stat,num_of_samples));

        /* INLINE PROCEDURE CALL OVERHEAD */
        printf("\n INLINE PROCEDURE CALL OVERHEAD :");
        /* ZERO ARGUMENTS */
	sum = 0;
	for(sample=0;sample<=num_of_samples;sample++) {
        	tick = getticks();
        	for(subsample=0;subsample <= num_of_samples; subsample++) {
                	iproc_call0();
        	}
        	tick1 = getticks();
		stat[sample] = (tick1 - tick)/num_of_samples;
		sum += (tick1 - tick)/num_of_samples;
	} 
        procedure_overhead = sum/num_of_samples;
        printf("\n 0 Inline Arguments\t:\t%f \t Std deviation\t:\t %lf",procedure_overhead,standard_deviation(stat,num_of_samples));

        /* ONE ARGUMENTS */
        sum=0;
        for(sample=0;sample<=num_of_samples;sample++) {
                tick = getticks();
                for(subsample=0;subsample <= num_of_samples; subsample++) {
                        iproc_call1(1);
                }
                tick1 = getticks();
                stat[sample] = (tick1 - tick)/num_of_samples;
                sum += (tick1 - tick)/num_of_samples;
        }
        procedure_overhead = sum/num_of_samples;
        printf("\n 1 Inline Argument\t:\t%f \t Std deviation\t:\t %lf",procedure_overhead,standard_deviation(stat,num_of_samples));

        /* TWO ARGUMENTS */
        sum=0;
        for(sample=0;sample<=num_of_samples;sample++) {
                tick = getticks();
                for(subsample=0;subsample <= num_of_samples; subsample++) {
                        iproc_call2(1,2);
                }
                tick1 = getticks();
                stat[sample] = (tick1 - tick)/num_of_samples;
                sum += (tick1 - tick)/num_of_samples;
        }
        procedure_overhead = sum/num_of_samples;
        printf("\n 2 Inline Arguments\t:\t%f \t Std deviation\t:\t %lf",procedure_overhead,standard_deviation(stat,num_of_samples));

        /* THREE ARGUMENTS */
        sum=0;
        for(sample=0;sample<=num_of_samples;sample++) {
                tick = getticks();
                for(subsample=0;subsample <= num_of_samples; subsample++) {
                        iproc_call3(1,2,3);
                }
                tick1 = getticks();
                stat[sample] = (tick1 - tick)/num_of_samples;
                sum += (tick1 - tick)/num_of_samples;
        }
        procedure_overhead = sum/num_of_samples;
        printf("\n 3 Inline Arguments\t:\t%f \t Std deviation\t:\t %lf",procedure_overhead,standard_deviation(stat,num_of_samples));

       /* FOUR ARGUMENTS */
        sum=0;
        for(sample=0;sample<=num_of_samples;sample++) {
                tick = getticks();
                for(subsample=0;subsample <= num_of_samples;subsample++) {
                        iproc_call4(1,2,3,4);
                }
                tick1 = getticks();
                stat[sample] = (tick1 - tick)/num_of_samples;
                sum += (tick1 - tick)/num_of_samples;
        }
        procedure_overhead = sum/num_of_samples;
        printf("\n 4 Inline Arguments\t:\t%f \t Std deviation\t:\t %lf",procedure_overhead,standard_deviation(stat,num_of_samples));

        /* FIVE ARGUMENTS */
        sum=0;
        for(sample=0;sample<=num_of_samples;sample++) {
                tick = getticks();
                for(subsample=0;subsample <= num_of_samples; subsample++) {
                        iproc_call5(1,2,3,4,5);
                }
                tick1 = getticks();
                stat[sample] = (tick1 - tick)/num_of_samples;
                sum += (tick1 - tick)/num_of_samples;
        }
        procedure_overhead = sum/num_of_samples;
        printf("\n 5 Inline Arguments\t:\t%f \t Std deviation\t:\t %lf",procedure_overhead,standard_deviation(stat,num_of_samples));

        /* SIX ARGUMENTS */
        sum=0;
        for(sample=0;sample<=num_of_samples;sample++) {
                tick = getticks();
                for(subsample=0;subsample <= num_of_samples; subsample++) {
                        iproc_call6(1,2,3,4,5,6);
                }
                tick1 = getticks();
                stat[sample] = (tick1 - tick)/num_of_samples;
                sum += (tick1 - tick)/num_of_samples;
        }
        procedure_overhead = sum/num_of_samples;
        printf("\n 6 Inline Arguments\t:\t%f \t Std deviation\t:\t %lf",procedure_overhead,standard_deviation(stat,num_of_samples));

        /* SEVEN ARGUMENTS */
        sum=0;
        for(sample=0;sample<=num_of_samples;sample++) {
                tick = getticks();
                for(subsample=0;subsample <= num_of_samples; subsample++) {
                        iproc_call7(1,2,3,4,5,6,7);
                }
                tick1 = getticks();
                stat[sample] = (tick1 - tick)/num_of_samples;
                sum += (tick1 - tick)/num_of_samples;
        }
        procedure_overhead = sum/num_of_samples;
        printf("\n 7 Inline Arguments\t:\t%f \t Std deviation\t:\t %lf",procedure_overhead,standard_deviation(stat,num_of_samples));

	/*SYSTEM CALL OVERHEAD */
	/* GETPID */
        printf("\nSystem Call Overhead \n");	
	printf("GETPID:\n");
	tick = getticks();
	getpid();
	tick1 = getticks();
	printf(" Fresh\t:\t%f\n",(float)(tick1-tick));
	tick = getticks();
	syscall(SYS_getpid);
	tick1 = getticks();
	printf(" Indirect\t:\t%f\n",(float)(tick1-tick));
	sum = 0;
	for(sample=0;sample<=num_of_samples;sample++) {	
		tick = getticks();
		for(subsample=0;subsample <= num_of_samples; subsample++) {
			getpid();
		}
		tick1 = getticks();
		stat[sample] = (tick1-tick)/num_of_samples;
		sum += (tick1 - tick)/num_of_samples;
	}
	systemcall_overhead = sum/num_of_samples;
	printf(" Cached\t:\t%f \t Std deviation\t:\t %lf\n", systemcall_overhead,standard_deviation(stat,num_of_samples));

        /* GETPPID */
        printf("GETPPID:\n");
	tick = getticks();
        getppid();
        tick1 = getticks();
	printf("GETPPID:\n");
        printf(" Fresh\t:\t%f\n",(float)(tick1-tick));
	tick = getticks();
	syscall(SYS_getppid);
	tick1 = getticks();
	printf(" Indirect\t:\t%f\n",(float)(tick1-tick));
        sum = 0;
        for(sample=0;sample<=num_of_samples;sample++) { 
                tick = getticks();
                for(subsample=0;subsample <= num_of_samples; subsample++) {
                        getppid();
                }
                tick1 = getticks();
                stat[sample] = (tick1-tick)/num_of_samples;
                sum += (tick1 - tick)/num_of_samples;
        }
        systemcall_overhead = sum/num_of_samples;
        printf(" Cached\t:\t%f Std deviation\t:\t %lf\n", systemcall_overhead,standard_deviation(stat,num_of_samples));

 	/* GETUID */
        tick = getticks();
        getuid();
        tick1 = getticks();
	printf("GETUID:\n");
        printf(" Fresh\t:\t%f\n",(float)(tick1-tick));
	tick = getticks();
	syscall(SYS_getuid);
	tick1 = getticks();
	printf(" Indirect\t:\t%f\n",(float)(tick1-tick));
        sum = 0;
        for(sample=0;sample<=num_of_samples;sample++) { 
                tick = getticks();
                for(subsample=0;subsample <= num_of_samples; subsample++) {
                        getuid();
                }
                tick1 = getticks();
                stat[sample] = (tick1-tick)/num_of_samples;
                sum += (tick1 - tick)/num_of_samples;
        }
        systemcall_overhead = sum/num_of_samples;
        printf(" Cached\t:\t%f Std deviation\t:\t %lf\n", systemcall_overhead,standard_deviation(stat,num_of_samples));       

        /* GETGID */
        tick = getticks();
        getgid();
        tick1 = getticks();
	printf("GETGID\n");
        printf(" Fresh\t:\t%f\n",(float)(tick1-tick));
	tick = getticks();
	syscall(SYS_getgid);
	tick1 = getticks();
	printf(" Indirect\t:\t%f\n",(float)(tick1-tick));
        sum = 0;
        for(sample=0;sample<=num_of_samples;sample++) { 
                tick = getticks();
                for(subsample=0;subsample <= num_of_samples; subsample++) {
                        getgid();
                }
                tick1 = getticks();
                stat[sample] = (tick1-tick)/num_of_samples;
                sum += (tick1 - tick)/num_of_samples;
        }
        systemcall_overhead = sum/num_of_samples;
        printf(" Cached\t:\t%f \t Std deviation\t:\t %lf\n", systemcall_overhead,standard_deviation(stat,num_of_samples));

	/* TASK CREATION */
	printf("\nTASK CREATION OVERHEAD: \n");
	sum = 0;
	process_sched.sched_priority = 90;
	pid = getpid();
	sched_setscheduler(pid,SCHED_FIFO,&process_sched); 
	for(sample=0; sample <= num_of_samples; sample++) {
		tick = getticks();
		pid = syscall(SYS_fork);
		if (pid < 0 ) {
			exit(0);
		}
		if (pid == 0 ) {
			exit(0);
		}
		else {
			wait(NULL);
		}
		tick1 = getticks();
		if(tick1 > tick) {
			sum += (tick1 - tick);
			stat[sample] = (tick1 - tick);
		}
	}
	task_overhead = sum/num_of_samples;
	printf(" Process Overhead\t:\t%f \t Std Deviation\t:\t%lf\n",task_overhead,standard_deviation(stat,num_of_samples)); 
	
	sum = 0;
	for(sample=0;sample<num_of_samples; sample++) {
		tick = getticks();
		pthread_create(&thread, NULL, &thread_start, NULL);
		pthread_join(thread, NULL);
		tick1 = getticks();
		sum += (tick1 - tick);
		stat[sample] = (tick1 - tick);
	}
	thread_overhead = sum/num_of_samples;
	printf(" Thread Overhead\t:\t%f \t Std Deviation\t:\t%lf\n",thread_overhead,standard_deviation(stat,num_of_samples)); 

	/* CONTEXT SWITCHING */
	sum = 0;
	count = 0;
	pipe(fd);
	printf("\nCONTEXT SWITCHING OVERHEAD\n");
	for(sample=0;sample<num_of_samples;sample++) {
		if ((pid = fork())> 0) {
			tick = getticks();
			read(fd[0], (void *)&tick1, sizeof(uint64_t));
			tick1 = getticks();
		}
		else {
			tick1 = 10;
			write(fd[1], (void *)&tick1, sizeof(uint64_t));
			exit(1);
		}
		if(tick1 > tick) {
			sum += tick1 - tick;
			stat[sample] = tick1 - tick;
			count++;
		} else {
			stat[sample] = 0;
		}
	}
	context_proc_overhead = sum/count;
	printf(" Process Switch \t:\t%f \t Std deviation\t:\t%lf\n",context_proc_overhead/2, standard_deviation(stat,count));
        
	sum = 0; 
	count = 0;
	for(sample=0; sample<num_of_samples;sample++) {
		pthread_create(&thread, NULL, thread_talk, NULL);
        	tick = getticks();
        	pthread_join(thread, NULL);
        	tick1 = getticks();
		if(tick1 > tick) {
			sum += tick1 - tick;
			stat[sample] = (tick1 - tick);
			count++;
		} else {
			stat[sample] = 0;
		}
	}
        context_thread_overhead = sum/count;
        printf(" Thread Switch \t:\t%f \t Std deviation\t:\t%lf\n\n", context_thread_overhead, standard_deviation(stat,count));
	return 0;	
}

