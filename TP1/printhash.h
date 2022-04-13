#include <stdint.h>

#define TOLONGLONG(x) ((unsigned long long)(*(__uint128_t*) x & 0xFFFFFFFFFFFFFFFF))

void print_2powN(uint8_t **m1, uint8_t **m2, int N);
void print_hash(uint8_t *h);