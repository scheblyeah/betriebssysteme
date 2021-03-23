#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

    //This solution was inspired by https://stackoverflow.com/questions/7171722/how-can-i-handle-sigchld/7171836#7171836

    void child_handler(int sig){
        write(STDOUT_FILENO, "Parent done.\n", 14);
        sig++;
        exit(0);
    }


int main (int argc, char* argv[]){
    if(argc != 2){
        printf("Usage: %s < number of seconds to sleep > !\n", argv[0]);
        return 0;
    }

    int seconds = atoi(argv[1]);

    pid_t pid;

	pid = fork();

    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = child_handler;

    sigaction(SIGCHLD, &sa, NULL);

    if(pid == 0){
        printf("Child %d sleeping for %d seconds...\n", getpid() , seconds);
        sleep(seconds);
        printf("Child done.\n");
        exit(1);
    }
    else{
        while(1){}
        return 1;
    }

}
