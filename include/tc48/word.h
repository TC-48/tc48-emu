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

#define TC48_TRYTE_VALUES   (729ULL)          /* 3^6  */
#define TC48_QUARTER_VALUES (531441ULL)       /* 3^12 */
#define TC48_HALF_VALUES    (282429536481ULL) /* 3^24 */
#define TC48_WORD_VALUES    ((tc48_word)0x10E425C5ULL << 64 | 0x6DAFFABC35C1ULL) /* 3^48 */
