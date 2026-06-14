#pragma once

#include <stdio.h>  // NOLINT(misc-include-cleaner)
#include <stdlib.h> // NOLINT(misc-include-cleaner)

#if defined(__GNUC__) || defined(__clang__)
    #define TC48_UNREACHABLE(STR) do {                                                 \
        fprintf(stderr, "REACHED UNREACHABLE: %s (%s:%d)\n", STR, __FILE__, __LINE__); \
        __builtin_unreachable();                                                       \
    } while (0)
#elif defined(_MSC_VER)
    #define TC48_UNREACHABLE(STR) do {                                                 \
        fprintf(stderr, "REACHED UNREACHABLE: %s (%s:%d)\n", STR, __FILE__, __LINE__); \
        __assume(0);                                                                   \
    } while (0)
#else
    #define TC48_UNREACHABLE(STR) do {                                                 \
        fprintf(stderr, "REACHED UNREACHABLE: %s (%s:%d)\n", STR, __FILE__, __LINE__); \
        abort();                                                                       \
    } while (0)
#endif

#if defined(__GNUC__) || defined(__clang__)
    #define TC48_UNREACHABLE_ENUM_VAL(TYPE, VAL) do { \
        fprintf(stderr, "invalid or not handled value %d for enum %s (%s:%d)\n", \
            (int)(VAL), #TYPE, __FILE__, __LINE__); \
        __builtin_unreachable(); \
    } while (0)
#elif defined(_MSC_VER)
    #define TC48_UNREACHABLE_ENUM_VAL(TYPE, VAL) do {                            \
        fprintf(stderr, "invalid or not handled value %d for enum %s (%s:%d)\n", \
            (int)(VAL), #TYPE, __FILE__, __LINE__);                              \
        __assume(0);                                                             \
    } while (0)
#else
    #define TC48_UNREACHABLE_ENUM_VAL(TYPE, VAL) do {                            \
        fprintf(stderr, "invalid or not handled value %d for enum %s (%s:%d)\n", \
            (int)(VAL), #TYPE, __FILE__, __LINE__);                              \
        abort();                                                                 \
    } while (0)
#endif

#define TC48_ASSERT_ALWAYS(EXPR, MSG) do {                                        \
    if (!(EXPR)) {                                                                \
        fprintf(stderr, "ASSERTION FAILED: %s\n Message: %s\n Location: %s:%d\n", \
            #EXPR, MSG, __FILE__, __LINE__);                                      \
        abort();                                                                  \
    }                                                                             \
} while (0)

#ifdef TC48_DEBUG
    #dfine TC48_ASSERT(EXPR, MSG) TC48_ASSERT_ALWAYS(EXPR, MSG)
#else
    #define TC48_ASSERT(EXPR, MSG) ((void)0)
#endif

#define TC48_TODO(MSG) do {                                                            \
    fprintf(stderr, "TODO: %s (in %s at %s:%d)\n", MSG, __func__, __FILE__, __LINE__); \
    abort();                                                                           \
} while (0)
