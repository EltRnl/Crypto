#include <stdio.h>
#include "mc48.h"
#include "xoshiro256starstar.h"

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

int check_collision(uint8_t m1[16], uint8_t m2[16]){


    return -1;
}

/* Searches for a collision for the compression function tcz48_dm*/
void find_col(uint8_t h[6], uint8_t m1[16], uint8_t m2[16]){
    uint8_t *map[UINT32_MAX] = {0};
    
    uint8_t th[6];
    uint64_t c = 0;
    
    copy(h,th);
    tcz48_dm(m1,th);
    if(map[(*th)%UINT32_MAX]!=0){
        if(check_collision(m1,map[(*th)%UINT32_MAX])==-1){
            copy(map[(*th)%UINT32_MAX],m2,16);
            return;
        }
        copy(m1,map[(*th)%UINT32_MAX]);
    }
    

    printf("Found collision after %ld tries.",c);
}

/* Takes as input an argument 'd' and writes on the standard output a list of 2^d colliding messages */
void attack(int d){

}

int main(){
    uint8_t m1[16];
    uint8_t m2[16];
    uint8_t h[8];
    uint8_t h2[8];
    

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

    find_col(h,m1,m2);
    tcz48_dm(m1,h);
    tcz48_dm(m2,h2);
    printf("Got messages :\n\t%s = %s\n\t%s = %s\n",m1,h,m2,h2);
    
    return 0;
}