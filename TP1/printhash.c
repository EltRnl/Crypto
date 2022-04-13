#include <stdlib.h>
#include <stdio.h>
#include "printhash.h"

void print_piece_128(uint8_t *m){
    printf("%llu",TOLONGLONG(m));
}

void print_piece_8(uint8_t *m, int size){
    for(int i=0; i<size; i++) printf("%c",m[i]);
}

void print_piece_int(uint8_t *m, int size){
    for(int i=0; i<size; i++) printf("%d|",m[i]);
}

void print_message_choice(uint8_t ***m, int N,uint8_t *choice){
    for(int i=0; i<N; i++)
        print_piece_8(m[choice[i]][i],16);
    printf("\n");
}

void print_rec(uint8_t ***m, int N, int i, uint8_t *choice){
    if(i==N){
        print_message_choice(m,N,choice);
    }else{
        choice[i] = 0;
        print_rec(m,N,i+1,choice);
        choice[i] = 1;
        print_rec(m,N,i+1,choice);
    }
}

void print_2powN(uint8_t **m1, uint8_t **m2, int N){
    uint8_t *choice = malloc(sizeof(uint8_t)*N);
    uint8_t **m[2]; m[0]=m1; m[1]=m2;
    print_rec(m,N,0,choice);
    free(choice);
}

void print_hash(uint8_t *h){
    print_piece_int(h,6);
    printf("\n");
}