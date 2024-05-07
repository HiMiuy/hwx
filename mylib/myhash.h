#ifndef _MYHASH_H_
#define _MYHASH_H_

#include "headers.h"
#define MOD31 2147483647
#define MOD61 2305843009213693952
#define PI 3.141592653589793

extern long myHash31(long long, long long, long long);
extern long myHash61(long long, long long, long long);

extern void randInit(uint32_t);
extern double urand();

#endif