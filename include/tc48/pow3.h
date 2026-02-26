#pragma once

#include <stdint.h>

#define TC48_MAX_SAFE_TRITS 40
extern const uint64_t tc48_pow3[TC48_MAX_SAFE_TRITS];

#define TC48_MAX_SAFE_TRITS_U32 21
extern const uint32_t tc48_pow3_u32[TC48_MAX_SAFE_TRITS_U32];

#define TC48_MAX_SAFE_TRITS_U64 41
extern const uint64_t tc48_pow3_u64[TC48_MAX_SAFE_TRITS_U64];

#define TC48_MAX_SAFE_TRITS_U128 81
extern const __uint128_t tc48_pow3_u128[TC48_MAX_SAFE_TRITS_U128];
