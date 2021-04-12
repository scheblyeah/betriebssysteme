/** The codebase is copied from the example of the 7 unix man page. A code snipped from https://stackoverflow.com/a/2605313 is also used.
 * **/

#define SOCKET_NAME "/tmp/9Lq7BNBnBycd6nba.socket"
#define BUFFER_SIZE 1024

/*
* File server.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>

//#include "connection.h"

void parse_args(char* str, char* (*result)[BUFFER_SIZE]) {
	memset(*result, 0, sizeof(char*) * BUFFER_SIZE);
	for(int i = 0; i < BUFFER_SIZE - 1; ++i, str = NULL) {
		(*result)[i] = strtok(str, " ");
	}
}

void handler(){

    unlink(SOCKET_NAME);

    exit(EXIT_SUCCESS);
}


int main() {
    struct sockaddr_un name;
    int down_flag = 0;
    int ret;
    int connection_socket;
    int data_socket;
    char result[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    char* split [BUFFER_SIZE];

    struct sigaction sa;
    sa.sa_handler = handler;

    sigaction(SIGINT, &sa, NULL);


    /* Create local socket. */

    connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (connection_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /*
    * For portability clear the whole structure, since some
    * implementations have additional (nonstandard) fields in
    * the structure.
    */

    memset(&name, 0, sizeof(name));

    /* Bind socket to socket name. */

    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

    ret = bind(connection_socket, (const struct sockaddr *) &name,
                sizeof(name));
    if (ret == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    /*
    * Prepare for accepting connections. The backlog size is set
    * to 20. So while one request is being processed other requests
    * can be waiting.
    */

    ret = listen(connection_socket, 20);
    if (ret == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* This is the main loop for handling connections. */
    
    for (;;) {
        
        //clear buffer
        memset(&buffer[0], 0, sizeof(buffer));


        /* Wait for incoming connection. */

        data_socket = accept(connection_socket, NULL, NULL);
        if (data_socket == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        memset(&result[0], 0, sizeof(result));

        for (;;) {

            /* Wait for next data packet. */

            ret = read(data_socket, buffer, sizeof(buffer));
            if (ret == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }

            /* Ensure buffer is 0-terminated. */

            buffer[sizeof(buffer) - 1] = 0;

            /* Handle commands. */

            if (!strncmp(buffer, "DOWN", sizeof(buffer))) {
                down_flag = 1;
                break;
            }

            if (!strncmp(buffer, "END", sizeof(buffer))) {
                break;
            }
            strcat(result, " ");
            strcat(result, buffer);
        }

        /* Send result. */

        parse_args(result, &split);

        int pipefd[2];
        pipe(pipefd);

        if (fork() == 0)
        {
            close(pipefd[0]);    // close reading end in the child

            dup2(pipefd[1], 1);  // send stdout to the pipe
            dup2(pipefd[1], 2);  // send stderr to the pipe

            close(pipefd[1]);    // this descriptor is no longer needed

            execvp(split[0],split);
        }
        else{
            // parent

            close(pipefd[1]);  // close the write end of the pipe in the parent

            while (read(pipefd[0], buffer, sizeof(buffer)) != 0)
            {
            }
        
        ret = write(data_socket, buffer, sizeof(buffer));
        if (ret == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        /* Close socket. */

        close(data_socket);

        /* Quit on DOWN command. */

        if (down_flag) {
            break;
        }
        }
    }

    close(connection_socket);

    /* Unlink the socket. */

    unlink(SOCKET_NAME);

    exit(EXIT_SUCCESS);
}