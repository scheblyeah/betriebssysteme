#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>  
#include <sys/mman.h>

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Usage: %s <int N> <int B>\n", argv[0]);
        return 1;
    }

    uint64_t n = atoi(argv[1]);
    uint64_t b = atoi(argv[2]);
    const char *shm_name = "shm";
    int shm_fd;

    shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666); //file descriptor
    ftruncate(shm_fd, sizeof(n)*(b+1)); // set size of array (b+1 because the sum is saved in array[0])
    uint64_t *ptr = mmap(0, sizeof(n)*b, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0); //memory-map a file pointer to the shared memory object


    pid_t child_producer = fork();
    if(child_producer == 0 ){
        //producer child
        ptr[0] = 0; // the sum is stored in ptr[0]
        for (uint64_t i = 0; i<=n; i++){
            ptr[i%b+1] = i;
        }
    }
    else if(child_producer == -1 ){
        printf("Producer fork failed.\n");
    }
    else{
        pid_t child_consumer = fork();
        if(child_consumer == 0){
        //consumer child
            for(uint64_t j = 1; j <= b; j ++){
                ptr[0] += ptr[j]; // the sum is stored in ptr[0]
                //printf("The sum after %ld numbers: %ld\n",j ,ptr[0]); //the sum is written in array[0]
            }
        }
        else if(child_consumer == -1 ){
            printf("Consumer fork failed.\n");
        }
        else{
            //parent
            wait(NULL);
            wait(NULL); //wait for consumer and producer child
            printf("The sum is: %ld\n", ptr[0]);
            for(uint64_t k = 0; k <= b; k++){
                //printf("%ld\n", ptr[k]);
            }
        }
    }
    shm_unlink(shm_name);
}