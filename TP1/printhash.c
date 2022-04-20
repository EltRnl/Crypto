#include <stdlib.h>
#include <stdio.h>
#include "printhash.h"
#include "mc48.h"

#ifdef PRINTCHAR
void print_piece(uint8_t *m, int size){for(int i=0; i<size; i++) printf("%c",m[i]);}
#elif PRINTINT
void print_piece(uint8_t *m, int size){for(int i=0; i<size; i++) printf("%d#",m[i]);}
#elif PRINT128
void print_piece(uint8_t *m, int dull){printf("%llu",TOLONGLONG(m));}
#else
void print_piece(uint8_t *m, int size){for(int i=0; i<size; i++) printf("%x",m[i]);}
#endif

void copybis(uint8_t * h, uint8_t * t, int size){
    for(int i=0; i<size; i++)
        t[i]=h[i];
}

void print_message_choice(uint8_t ***m, int N,uint8_t *choice){
    for(int i=0; i<N; i++)
        print_piece(m[choice[i]][i],16);
    printf(" ");
    uint8_t m_tmp[N*16]; uint8_t h[6]; h[0] = IVB0;h[1] = IVB1;h[2] = IVB2;h[3] = IVB3;h[4] = IVB4;h[5] = IVB5;
    for(int i=0; i<N; i++){
        copybis(m[choice[i]][i],m_tmp+(i*16),16);
    }
    ht48(m_tmp,N*16,h);
    print_piece(h,6);
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