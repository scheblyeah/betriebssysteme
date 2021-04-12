#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_ARGS 8
void parse_args(char* str, char* (*result)[MAX_ARGS]) {
	memset(*result, 0, sizeof(char*) * MAX_ARGS);
	for(int i = 0; i < MAX_ARGS - 1; ++i, str = NULL) {
		(*result)[i] = strtok(str, " ");
	}
}

int main(int argc, char *argv[]){

    /** save stdout to a string
    char string[50];
    freopen("/dev/null", "a", stdout);
    setbuf(stdout, string);
    freopen ("/dev/tty", "a", stdout);

    printf("lol");


    //char* args[MAX_ARGS];
    //parse_args(argv[1], &args);
    //execvp(args[0],args);
    printf("lol");
    printf(" The output is : %s lkjölk", string);
    **/



    int pipefd[2];
    pipe(pipefd);

    if (fork() == 0)
    {
        close(pipefd[0]);    // close reading end in the child

        dup2(pipefd[1], 1);  // send stdout to the pipe
        dup2(pipefd[1], 2);  // send stderr to the pipe

        close(pipefd[1]);    // this descriptor is no longer needed

        char* args[MAX_ARGS];
        parse_args(argv[1], &args);
        execvp(args[0],args);
}
else
{
    // parent

    char buffer[1024];

    close(pipefd[1]);  // close the write end of the pipe in the parent

    while (read(pipefd[0], buffer, sizeof(buffer)) != 0)
    {
    }
    printf("The command is: \n %s\n... and so on\n", buffer);

}

    return 0;
}