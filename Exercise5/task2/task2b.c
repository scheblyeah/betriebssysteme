#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>  
#include <sys/mman.h>
#include <semaphore.h>

/** 
 * Task2 only works if N = B or N = B+1, otherwise the programm stays in an endless wait loop
 * from the semaphores, but i couldn't figure out why. 
 * 
 * Small code fragments were taken from the VO slides and from 
 * http://www.cs.nott.ac.uk/~pszbsl/G52CON/Slides/08-Semaphores-II.pdf
*/

sem_t *sem_empty;
sem_t *sem_full;


#define MAX_SLEEP_MS 10
uint64_t expensive_calculation(uint64_t value) {
	usleep((rand() / (float)RAND_MAX) * MAX_SLEEP_MS);
	return value;
}


int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Usage: %s <int N> <int B>\n", argv[0]);
        return 1;
    }

    uint64_t n = atoi(argv[1]);
    uint64_t b = atoi(argv[2]);
    const char *shm_name = "shm";
    int shm_fd;
    const char *name_empty = "empty";
    const char *name_full = "full";

    sem_unlink(name_empty); //unlink sem in case of already existing semaphore with this name
    sem_unlink(name_full);

    sem_empty = sem_open(name_empty, O_CREAT, 0660, 1);
    sem_full = sem_open(name_full, O_CREAT, 0660, 0);

    shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666); //file descriptor
    ftruncate(shm_fd, sizeof(n)*(b+1)); // set size of array (b+1 because the sum is saved in array[0])
    uint64_t *ptr = mmap(0, sizeof(n)*b, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0); //memory-map a file pointer to the shared memory object


    pid_t child_producer = fork();
    if(child_producer == 0 ){
        //producer child
        ptr[0] = 0; // the sum is stored in ptr[0]
        srand(getpid());
        for (uint64_t i = 0; i<=n; i++){
            sem_wait(sem_empty);
            ptr[i%b+1] = expensive_calculation(i);
            sem_post(sem_full);
        }
    }
    else if(child_producer == -1 ){
        printf("Producer fork failed.\n");
    }
    else{
        pid_t child_consumer = fork();
        if(child_consumer == 0){
        //consumer child
            srand(getpid());
            for(uint64_t j = 1; j <= b; j ++){
                sem_wait(sem_full);
                ptr[0] += expensive_calculation(ptr[j]); // the sum is stored in ptr[0]
                //printf("The sum after %ld numbers: %ld\n",j ,ptr[0]); //the sum is written in array[0]
                sem_post(sem_empty);
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
    sem_unlink(name_empty);
    sem_unlink(name_full);
    shm_unlink(shm_name);
}