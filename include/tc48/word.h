#pragma once

#include <stdint.h>

#ifndef uint128_t
#  define uint128_t __uint128_t
#endif

typedef uint128_t  tc48_word;
typedef uint64_t   tc48_half;
typedef uint32_t   tc48_quarter;
typedef uint16_t   tc48_tryte;

typedef tc48_word tc48_addr;
