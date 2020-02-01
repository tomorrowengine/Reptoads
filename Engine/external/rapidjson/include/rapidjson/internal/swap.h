













#ifndef RAPIDJSON_INTERNAL_SWAP_H_
#define RAPIDJSON_INTERNAL_SWAP_H_

#include "../rapidjson.h"

#if defined(__clang__)
RAPIDJSON_DIAG_PUSH
RAPIDJSON_DIAG_OFF(c++98-compat)
#endif

RAPIDJSON_NAMESPACE_BEGIN
namespace internal {



template <typename T>
inline void Swap(T& a, T& b) RAPIDJSON_NOEXCEPT {
    T tmp = a;
        a = b;
        b = tmp;
}

} 
RAPIDJSON_NAMESPACE_END

#if defined(__clang__)
RAPIDJSON_DIAG_POP
#endif

#endif 
