#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mc48.h"
#include "xoshiro256starstar.h"

typedef struct _{
    uint8_t *value;
    uint32_t size;
    uint8_t elem_size;
} hmap;

void copy(uint8_t * h, uint8_t * t, int size){
    for(int i=0; i<size; i++)
        t[i]=h[i];
}
int equals(uint8_t * m1, uint8_t * m2, int size){
    for(int i=0; i<size; i++){
        if(m1[i]!=m2[i])
            return -1;
    }
    return 1;
}
int check_collision(uint8_t *m, uint8_t *h, uint8_t *th){
    uint8_t nh[6];
    copy(h,nh,6);
    tcz48_dm(m,nh);
    return equals(th,nh,6);
}
void make_random(uint8_t m[16]){
    for(int i=0; i<16; i++){
        m[i] = xoshiro256starstar_random();
    }
}

/* Searches for a collision for the compression function tcz48_dm*/
void find_col(uint8_t h[6], uint8_t m1[16], uint8_t m2[16]){
    hmap *map = malloc(sizeof(hmap));
    map->elem_size = 16;
    map->size = UINT32_MAX;
    map->value = malloc(map->size*map->elem_size*sizeof(uint8_t));

    uint8_t th[6];
    uint64_t c = 0;
    //
    while(++c<16777216*6){ // 16777216 is 2^24, aka sqrt(2^48)
        copy(h,th,6);
        make_random(m1);
        tcz48_dm(m1,th);
        if(map->value[((*(uint64_t*)th)*map->elem_size)%UINT32_MAX]!=0 && equals(m1,map->value+(((*(uint64_t*)th)*map->elem_size)%UINT32_MAX),16)==-1){
            if(check_collision(map->value+(((*(uint64_t*)th)*map->elem_size)%UINT32_MAX),h,th)==1){
                copy(map->value+(((*(uint64_t*)th)*map->elem_size)%UINT32_MAX),m2,16);
                printf("Found collision after %ld tries.\n",c);
                return;
            }
        }
        copy(m1,map->value+(((*(uint64_t*)th)*map->elem_size)%UINT32_MAX),16);
    }
}

/* Takes as input an argument 'd' and writes on the standard output a list of 2^d colliding messages */
void attack(int d){

}

int main(){
    uint8_t *m1 = malloc(sizeof(uint8_t)*16);
    uint8_t *m2 = malloc(sizeof(uint8_t)*16);
    uint8_t *h = malloc(sizeof(uint8_t)*8);
    uint8_t *h2 = malloc(sizeof(uint8_t)*8);
    

    h[0] = IVB0;
    h[1] = IVB1;
    h[2] = IVB2;
    h[3] = IVB3;
    h[4] = IVB4;
    h[5] = IVB5;

    h2[0] = IVB0;
    h2[1] = IVB1;
    h2[2] = IVB2;
    h2[3] = IVB3;
    h2[4] = IVB4;
    h2[5] = IVB5;

    
    clock_t begin = clock();
    find_col(h,m1,m2);
    clock_t end = clock();
    tcz48_dm(m1,h);
    tcz48_dm(m2,h2);
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Got messages after %lf seconds :\n\t%llx = %lx\n\t%llx = %lx\n",time_spent,(unsigned long long)(*(__uint128_t*)m1 & 0xFFFFFFFFFFFFFFFF),*(__uint64_t*)h,(unsigned long long)(*(__uint128_t*)m2 & 0xFFFFFFFFFFFFFFFF),*(__uint64_t*)h2);
    
    return 0;
}