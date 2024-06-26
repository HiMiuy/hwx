#include <stdio.h>
#include <stdlib.h>
#include "myhash.h"


long myHash31(long long a, long long b, long long x) {
    long long result;
    long longresult;
    result = (a*x) + b;
    result = (result >> 31) + result;
    result = result & MOD31;
    longresult = (long) result;
    return longresult;
}

long myHash61(long long a, long long b, long long x) {
    long long result;
    long longresult;
    result = (a*x) + b;
    result = (result >> 61) + result;
    result = result & MOD61;
    longresult = (long) result;
    return longresult;
}


// These variables are used in XorShift
static uint32_t xorsft_x = 123456789;
static uint32_t xorsft_y = 362436069;
static uint32_t xorsft_z = 521288629;
static uint32_t xorsft_w = 88675123;
void randInit(uint32_t seed) {
    do {
        seed = seed*1812433253 + 1; seed ^= seed<<13; seed ^= seed>>17;
        xorsft_x = 123464980 ^ seed;
        seed = seed*1812433253 + 1; seed ^= seed<<13; seed ^= seed>>17;
        xorsft_y = 3447902351 ^ seed;
        seed = seed*1812433253 + 1; seed ^= seed<<13; seed ^= seed>>17;
        xorsft_z = 2859490775 ^ seed;
        seed = seed*1812433253 + 1; seed ^= seed<<13; seed ^= seed>>17;
        xorsft_w = 47621719 ^ seed;
    } while (xorsft_x==0 && xorsft_y==0 && xorsft_z==0 && xorsft_w==0);
}
double urand(){
    uint32_t t;
    t = xorsft_x ^ (xorsft_x<<11);
    xorsft_x = xorsft_y;
    xorsft_y = xorsft_z;
    xorsft_z = xorsft_w;
    xorsft_w ^= t ^ (t>>8) ^ (xorsft_w>>19);
    return ((xorsft_x+0.5) / 4294967296.0 + xorsft_w) ;/// 4294967296.0;
}