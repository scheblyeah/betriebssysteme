#include<stdlib.h>
#include<stdio.h>
#include"membench.h"

/**
The code base was inspired by: https://github.com/philippgs/os_ps_2018/blob/master/ex10/task1_solution/freelist.h
*/

#define BLOCK_SIZE 1024

typedef struct Block{
    struct Block* next;
    size_t size;
} Block;

Block* head;
Block* next_free;


void my_allocator_init(size_t size){
    head = (Block*) malloc(size / BLOCK_SIZE * BLOCK_SIZE);  // whole number division to eliminate the remainder
    unsigned int number_of_blocks = size / BLOCK_SIZE;
    head->next = head + sizeof(Block);
    head->size = 0;
    next_free = head;


}

void my_allocator_destroy(){
    free(head);
}


void* my_malloc(size_t size){

    Block* return_block = NULL;

    if(size > BLOCK_SIZE){
        return NULL;
    }

    

    if(next_free != NULL){
        return_block = next_free;
        return_block->size = size;



        if(next_free->next !=NULL){
            next_free = next_free->next; //setting the next free node
        }
        else{  // case where all blocks are full now
            next_free = NULL;
        }

        //implementing deletenode


        //To do: node that points to returnblock now has to point to (the new) next Free
        //verbuggt:
        //!
        Block* iterate = head;
        while(iterate->next != NULL){
            if(iterate->next == return_block){
                iterate->next = next_free;
            }
            iterate = iterate->next;
        }
    }

    return return_block;
}

void my_free(void* ptr){

    if(ptr != NULL){



        //setze node an 1. stelle von linked list



        Block* freed_node = (Block*) ptr;
        freed_node->size = 0;


        if(freed_node < next_free){  // check if the freed node is before the current next_free
            next_free = freed_node;
            freed_node->next = next_free;
        }
        else{
            //freed node next = was?
            //previous free node of freed node points to who?
        }
    }
}

void print_list(){
    printf("-----------------\n");
    Block* block = next_free;
    if(block != NULL){
        while(1){
            if(block->next == NULL){
                printf("Block with size: %ld\n", block->size);
                break;
            }
            else{
                printf("Block with size: %ld\n", block->size);
                block = block->next;
            }
        }
        printf("-----------------\n");
    }
}

int main(){
    
    my_allocator_init(5000);
    print_list();

    Block* block1 = my_malloc(100);
    print_list();
    printf("Test1\n");
    Block* block2 = my_malloc(100);
    print_list();
    printf("Test2\n");

    Block* block3 = my_malloc(100);
    //print_list();

    printf("Test3\n");

    my_free(block1);
    my_free(block2);
    my_free(block3);

    
    print_list();

    printf("Test4\n");


    my_allocator_destroy(); 

    printf("Test5\n");


    //run_membench_global(my_allocator_init, my_allocator_destroy, my_malloc, my_free);
    return 0;
}
