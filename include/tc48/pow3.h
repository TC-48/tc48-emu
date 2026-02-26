#pragma once

#include <tc48/defs.h>

#define TC48_MAX_SAFE_TRITS 40
extern const tc48_u64b tc48_pow3[TC48_MAX_SAFE_TRITS];

#define TC48_MAX_SAFE_TRITS_U32 21
extern const tc48_u32b tc48_pow3_u32[TC48_MAX_SAFE_TRITS_U32];

#define TC48_MAX_SAFE_TRITS_U64 41
extern const tc48_u64b tc48_pow3_u64[TC48_MAX_SAFE_TRITS_U64];

#define TC48_MAX_SAFE_TRITS_U128 81
extern const tc48_u128b tc48_pow3_u128[TC48_MAX_SAFE_TRITS_U128];
