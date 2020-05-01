#include <ADTPriorityQueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* create_int(int value){
    int* a = malloc(sizeof(int));
    *a = value;
    return a;
}

int compare_ints(Pointer a, Pointer b) {
	return *(int*)a - *(int*)b;
}

void real();
void amortized();

int main(int argc, char **argv){
    if(argc < 2){
        printf("Wrong usage.\npqueue_insert_steps <type>, <type> = real | amortized\n");
        return 1;
    }
    if(strcmp(argv[1], "real") == 0){
        real();
    }else if(strcmp(argv[1], "amortized") == 0){
        amortized();
    }else{
        printf("Wrong parameter. Use real | amortized\n");
        return 1;
    }
    return 0;
}

void real(){
    PriorityQueue pqueue = pqueue_create(compare_ints, free, NULL);
    pqueue_insert(pqueue, create_int(0)); // Πρέπει να υπάρχει ένα στοιχείο ήδη όταν γίνει pqueue_insert στην for
    for(int n = 1; n <= 10000; n++){
        pqueue_insert(pqueue, create_int(n));
        printf("%d,%d\n", n, pqueue_steps(pqueue));
    }
    pqueue_destroy(pqueue);
}

void amortized(){
    int steps = 0;
    PriorityQueue pqueue = pqueue_create(compare_ints, free, NULL);
    for(int n = 1; n <= 10000; n++){
        pqueue_insert(pqueue, create_int(n));
        steps += pqueue_steps(pqueue);
        printf("%d,%f\n", n, (float)steps/n);
    }
    pqueue_destroy(pqueue);
}