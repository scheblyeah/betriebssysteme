#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
//#define _POSIX_SOURCE

// my solution was inspired by https://stackoverflow.com/a/60402014/13622848

int main (){

    while(1){
        int pid = fork();
        sigset_t sigs;
        sigemptyset(&sigs);
        sigaddset(&sigs, SIGINT);
        sigprocmask(SIG_BLOCK, &sigs, NULL);
        struct timespec timeout = { .tv_sec = 5 };

        if (pid < 0){
            fprintf(stderr, "Fork Failed\n");
		    return 1;
        }

        else if( pid == 0){
            printf("Child %d started.\n", getpid());
            sleep(5);
            printf("Child %d done.\n", getpid());
            exit(0);
        }

        else{
            wait(NULL);
            int sig_num = sigtimedwait(&sigs, NULL, &timeout);

            if( sig_num == SIGINT){
                printf("Received SIGINT. Exiting.\n");
                return 1;
            }
            else{
                printf("Did not receive SIGINT. Continuing.\n");
            }
        }
    }
    return 1;
}