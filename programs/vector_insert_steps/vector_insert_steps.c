#include <stdio.h>
#include <ADTVector.h>
#include <stdlib.h>
#include <string.h>

int* create_int(int value){
    int* a = malloc(sizeof(int));
    *a = value;
    return a;
}

void real();
void amortized();

int main(int argc, char **argv){
    if(strcmp(argv[1], "real") == 0){
        real();
    }else if(strcmp(argv[1], "amortized") == 0){
        amortized();
    }else{
        printf("Wrong usage.\nvector_insert_steps <type>, type> = real | amortized");
        return 1;
    }
    return 0;
}

void real(){
    Vector test_vec = vector_create(0, free);
    vector_insert_last(test_vec, create_int(0)); //there must be one element on first insertion
    for(int n = 1; n <= 10000; n++){
        vector_insert_last(test_vec, create_int(n));
        printf("%d,%d\n", n, vector_steps(test_vec));
    }
    vector_destroy(test_vec);
}

void amortized(){
    int steps = 0;
    Vector test_vec = vector_create(0, free);
    for(int n = 1; n <= 10000; n++){
        vector_insert_last(test_vec, create_int(n));
        steps += vector_steps(test_vec);
        printf("%d,%f\n", n, (float)steps/n);
    }
    vector_destroy(test_vec);
}