













#ifndef RAPIDJSON_INTERNAL_STRFUNC_H_
#define RAPIDJSON_INTERNAL_STRFUNC_H_

#include "../stream.h"
#include <cwchar>

RAPIDJSON_NAMESPACE_BEGIN
namespace internal {



template <typename Ch>
inline SizeType StrLen(const Ch* s) {
    RAPIDJSON_ASSERT(s != 0);
    const Ch* p = s;
    while (*p) ++p;
    return SizeType(p - s);
}

template <>
inline SizeType StrLen(const char* s) {
    return SizeType(std::strlen(s));
}

template <>
inline SizeType StrLen(const wchar_t* s) {
    return SizeType(std::wcslen(s));
}


template<typename Encoding>
bool CountStringCodePoint(const typename Encoding::Ch* s, SizeType length, SizeType* outCount) {
    RAPIDJSON_ASSERT(s != 0);
    RAPIDJSON_ASSERT(outCount != 0);
    GenericStringStream<Encoding> is(s);
    const typename Encoding::Ch* end = s + length;
    SizeType count = 0;
    while (is.src_ < end) {
        unsigned codepoint;
        if (!Encoding::Decode(is, &codepoint))
            return false;
        count++;
    }
    *outCount = count;
    return true;
}

} 
RAPIDJSON_NAMESPACE_END

#endif 
