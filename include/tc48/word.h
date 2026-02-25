#pragma once

#include <stdint.h>

typedef struct tc48_doublet    { uint8_t  l, h; } tc48_doublet;
typedef struct tc48_triplet    { uint8_t  l, h; } tc48_triplet;
typedef struct tc48_quadruplet { uint8_t  l, h; } tc48_quadruplet;
typedef struct tc48_tryte      { uint8_t  l, h; } tc48_tryte;
typedef struct tc48_quarter    { uint16_t l, h; } tc48_quarter;
typedef struct tc48_half       { uint32_t l, h; } tc48_half;
typedef struct tc48_word       { uint64_t l, h; } tc48_word;

typedef tc48_word tc48_addr;

/// Dual-Bit Representation: 1 bit per component (l/h) per trit
#define TC48_TRIT_BITS 1

#define TC48_DOUBLET_BITS    2
#define TC48_TRIPLET_BITS    3
#define TC48_QUADRUPLET_BITS 4
#define TC48_TRYTE_BITS      6
#define TC48_QUARTER_BITS    12
#define TC48_HALF_BITS       24
#define TC48_WORD_BITS       48

#define TC48_TRYTE_MASK      0x3F
#define TC48_QUARTER_MASK    0xFFF
#define TC48_HALF_MASK       0xFFFFFF
#define TC48_WORD_MASK       0xFFFFFFFFFFFFULL

#define TC48_DOUBLET_VALUES    (9ULL)            /* 3^2  */
#define TC48_TRIPLET_VALUES    (27ULL)           /* 3^3  */
#define TC48_QUADRUPLET_VALUES (81ULL)           /* 3^4  */
#define TC48_TRYTE_VALUES      (729ULL)          /* 3^6  */
#define TC48_QUARTER_VALUES    (531441ULL)       /* 3^12 */
#define TC48_HALF_VALUES       (282429536481ULL) /* 3^24 */
#define TC48_WORD_VALUES       ((tc48_word){0x25C56DAFFABC35C1ULL, 0x10E4ULL}) /* 3^48 */
