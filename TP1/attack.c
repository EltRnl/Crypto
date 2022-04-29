#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mc48.h"
#include "xoshiro256starstar.h"
#include "printhash.h"

typedef struct _{
    uint8_t *value;
    uint32_t size;
    uint8_t elem_size;
} hmap;

/* Usage functions */
void copy(uint8_t * h, uint8_t * t, int size){ for(int i=0; i<size; i++) t[i]=h[i];}
void make_random(uint8_t m[16]){ for(int i=0; i<16; i++) m[i] = xoshiro256starstar_random();}

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

/* Searches for a collision for the compression function tcz48_dm*/
void find_col(uint8_t h[6], uint8_t m1[16], uint8_t m2[16]){
    hmap *map = malloc(sizeof(hmap));
    map->elem_size = 16;
    map->size = UINT32_MAX;
    map->value = malloc(map->size*map->elem_size*sizeof(uint8_t));

    uint8_t th[6];
    uint64_t c = 0;
    while(++c<16777216*3){ // 16777216 is 2^24, aka sqrt(2^48)
        copy(h,th,6);
        make_random(m1);
        tcz48_dm(m1,th);
        if(map->value[((*(uint64_t*)th)*map->elem_size)%UINT32_MAX]!=0 && equals(m1,map->value+(((*(uint64_t*)th)*map->elem_size)%UINT32_MAX),16)==-1 && check_collision(map->value+(((*(uint64_t*)th)*map->elem_size)%UINT32_MAX),h,th)==1){
            copy(map->value+(((*(uint64_t*)th)*map->elem_size)%UINT32_MAX),m2,16);
            copy(th,h,6);
            break;
        }
        copy(m1,map->value+(((*(uint64_t*)th)*map->elem_size)%UINT32_MAX),16);
        #ifdef VERBOSE
        if(c%16777216==0) fprintf(stderr,"Reached %ld steps.\n",c);
        #endif
    }
    #ifdef VERBOSE
    fprintf(stderr,"Reached %ld steps.\n",c);
    #endif
    free(map->value);
    free(map);
}

/* Takes as input an argument 'd' and writes on the standard output a list of 2^d colliding messages */
void attack(int d){
    uint8_t **m1 = (uint8_t **)malloc(d*sizeof(uint8_t*));
    uint8_t **m2 = (uint8_t **)malloc(d*sizeof(uint8_t*));
    uint8_t *h = malloc(sizeof(uint8_t)*6);  
    h[0] = IVB0;h[1] = IVB1;h[2] = IVB2;h[3] = IVB3;h[4] = IVB4;h[5] = IVB5;

    for(int i=0; i<d; i++){
        m1[i] = malloc(sizeof(uint8_t)*16);
        m2[i] = malloc(sizeof(uint8_t)*16);
        find_col(h,m1[i],m2[i]);
        #ifdef VERBOSE
        fprintf(stderr,"Collision #%d found.\n",i+1);
        #endif
    }
    print_2powN(m1,m2,d);
    for(int i=0; i<d; i++){
        free(m1[i]);
        free(m2[i]);
    }
    free(m1);
    free(m2);
}

/* Main function to call the attack */
int main(int argc, char ** argv){
    // uint64_t seed[4] = {1,1,1,1};
    // xoshiro256starstar_random_set(seed);
    
    if(argc<2) return -1;
    clock_t begin = clock();
    attack(atoi(argv[1]));
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    fprintf(stderr,"Got messages after %lf seconds\n",time_spent);
    return 0;
}