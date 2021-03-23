/**
 * This code uses the code from https://github.com/sashkoristov/osc10e/blob/master/ch3/newproc-posix.c as a basis,
 * and also uses code fragments of my last years submission of a similar exercise.
 *
 * 
 * This program forks a separate process using the fork()/exec() system calls.
 *
 * Figure 3.08
 *
 * @author Silberschatz, Galvin, and Gagne
 * Operating System Concepts  - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


double mc_pi(int64_t S) {
    int64_t in_count = 0;
    for(int64_t i = 0; i < S; ++i) {
        const double x = rand() / (double)RAND_MAX;
        const double y = rand() / (double)RAND_MAX;
        if(x*x + y*y <= 1.f) {
            in_count++;
        }
    }
    return 4 * in_count / (double)S;
}

int main(int argc, char *argv[])
{

if (argc != 3){
        printf("input error function takes exactly 3 argument\n");
        printf("correct usage: %s <number children> <number samples> \n",argv[0]);
        return 0;
    }

int n = atoi(argv[1]);
int64_t s =  atoi(argv[2]);

pid_t pid;

for( int i = 0 ; i < n; i++){
	/* fork a child process */

	pid = fork();

    srand(getpid());


	if (pid < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed\n");
		return 1;
	}
	else if (pid == 0) { /* child process */
        printf("Child %d PID = %d. mc_pi(%ld) = %f.\n",i, getpid(), s, mc_pi(s) );
		exit(0);
	}
    
    }
    for( int i = 0 ; i < n; i++){
        wait(NULL);
    }
    printf("Done.\n");
}


/**
 * Question answer:
 * I observed that the order of the children seems to always be 0,1,2 when s is low ( when the process neeeds little computation power),
 * but when s is big, and the processes take quite some time and computations, the order seems to be random, probably because of the process
 * scheduling. 
 * 
 * It does make a difference if you put the pid as the seed before or after the fork. When you put it before the fork, the seed is the pid
 * of the parent for all children, so the results are the same for all, but when you put it after the fork, the seed of every child is their
 * own pid (which is obviosly different) leading to different seeds and different results.
 * 
 * */ 