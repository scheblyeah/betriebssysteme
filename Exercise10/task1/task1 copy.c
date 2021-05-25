#include<stdlib.h>
#include<stdio.h>
#include <pthread.h>
#include"membench.h"

/**
The code base was inspired by: https://github.com/philippgs/os_ps_2018/blob/master/ex10/task1_solution/freelist.h
*/

#define BLOCK_SIZE 1024

pthread_mutex_t lock;

typedef struct Block{
    struct Block* next;
    size_t size;
} Block;

Block* head = NULL;
Block* initial_head;

void my_allocator_init(size_t size){
    if(size>BLOCK_SIZE){
        head = malloc(8*size / BLOCK_SIZE * BLOCK_SIZE *sizeof(Block *) );  // whole number division to eliminate the remainder
        unsigned int number_of_blocks = size / BLOCK_SIZE;
        head->next = NULL;
        head->size = 0;
        initial_head = head;

        if(number_of_blocks>1){
            head->next = head + sizeof(Block);
            Block* iterate = head->next;
            

            for(unsigned int i = 1; i< number_of_blocks; i++){
                if(i == number_of_blocks -1){
                    iterate->next = NULL;
                }
                else{
                    iterate->next = iterate + BLOCK_SIZE;
                }
                iterate->size = 0;

                iterate = (Block*) iterate->next;
            }
        }
    }
    else{printf("Size must be bigger than the Block size!\n");}
}

void* my_malloc(size_t size){
    if(size > BLOCK_SIZE){
        return NULL;
    }

    Block* return_block = NULL;
    pthread_mutex_lock(&lock);
    if(head != NULL){

        return_block = head;
        return_block->size = size;

        if(head->next != NULL){
            head = head->next;
        }
        else{
            head = NULL;
        }
    }
    pthread_mutex_unlock(&lock);
    return return_block;
}

void my_allocator_destroy(){
    pthread_mutex_lock(&lock);
    free(initial_head);
    head = NULL;
    pthread_mutex_unlock(&lock);
}

void my_free(void* ptr){
    pthread_mutex_lock(&lock);
    if(ptr != NULL){
        Block* freed_node = (Block*) ptr;
        if(freed_node->size != 0){
            freed_node->size = 1;
            freed_node->next = head;
            head = freed_node;
        }
    }
    pthread_mutex_unlock(&lock);
}

void printList(){
    Block* block = head;
    if(block!=NULL){
        while(1){
            if(block->next == NULL){
                printf("address: %p, size: %ld\n", block,block->size);
                break;
            }
            else{
                printf("address: %p, size: %ld\n", block,block->size);
            }
            block = block->next;
        }
    }
    else{printf("Empty list!\n");}
    printf("----------------------\n");
}

int main(){
    // Block* block1; Block* block2; Block* block3;
    // my_allocator_init(3420);
    // printf("Full list:\n");
    // printList();

    // block1 = my_malloc(12);
    // block2 = my_malloc(12);
    // block3 = my_malloc(12);
    // printList();

    // my_free(block1); my_free(block2); my_free(block3);
    // printf("Full list:\n");
    // printList();

    // printf("Head is %p,  %p before malloc\n", head, head->next);
    // block1 = malloc(18);
    // printf("Block1 is: %p after malloc again\n", block1);
    // printf("Head is %p,  %p after malloc\n", head, head->next);
    // printf("1 Block alloced:\n");
    // printList();

    // my_allocator_destroy(head);
    // printList();

    run_membench_global(my_allocator_init, my_allocator_destroy, my_malloc, my_free);
    return 0;
}