#pragma once

#define TC48_HAS_FEATURE(name) (TC48_HAS_FEATURE_##name)

#if defined(TC48_CONFIG_TVA) && TC48_CONFIG_TVA
#   define TC48_HAS_FEATURE_tva 1
#else
#   define TC48_HAS_FEATURE_tva 0
#endif
