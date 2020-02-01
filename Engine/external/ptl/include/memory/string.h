#pragma once

#include "memory/Allocator.h"

#include <string>
#include <sstream>
#include <locale>
#include <limits.h>
#include <cstring>

#ifndef _CSTD
#define _CSTD	::
#endif

namespace ptl{
#pragma warning( push )
#pragma warning(disable: 4455)
template<typename Char_, class _Traits = std::char_traits<Char_>, class _Alloc = ptl::MemoryAllocator<Char_>>
using basic_string = std::basic_string<Char_, _Traits, _Alloc>;

template<typename Ty_>
using generic_string = std::basic_string<Ty_, std::char_traits<Ty_>, ptl::MemoryAllocator<Ty_>>;

using string = ptl::basic_string<char>;
using wstring = ptl::basic_string<wchar_t>;
using u16string = ptl::basic_string<char16_t>;
using u32string = ptl::basic_string<char32_t>;

#ifndef DO_NOT_MAKE_USE_OF_CUSTOM_ALLOC
bool operator==(ptl::string& str1, std::string& str2);
bool operator==(std::string& str1, ptl::string& str2);
bool operator==(const std::string& str1,const ptl::string& str2);
bool operator==(const ptl::string& str1, const std::string& str2);
#endif


template<
    class CharT,
    class Traits = std::char_traits<CharT>
>
using basic_stringstream = std::basic_stringstream< CharT, Traits, ptl::MemoryAllocator<CharT>>;


using stringstream = basic_stringstream<char, std::char_traits<char>>;

#pragma warning(push)

#pragma warning(disable: 4996)
template< class Codecvt,
    class Elem = wchar_t>
    using wstring_convert = std::wstring_convert<Codecvt, Elem, ptl::MemoryAllocator<Elem>, ptl::MemoryAllocator<char>>;
#pragma warning(pop)


template <typename CharT, typename T>
ptl::basic_string<CharT> to_basic_string(T const&v) {
    ptl::basic_stringstream<CharT> sstr;
    sstr << v;
    return sstr.str();
}

template <typename T>
string to_string(T const&v) {
    return to_basic_string<char>(v);
}
template <typename T>
wstring to_wstring(T const&v) {
    return to_basic_string<wchar_t>(v);
}
template <typename T>
u16string to_u16string(T const&v) {
    return to_basic_string<char16_t>(v);
}
template <typename T>
u32string to_u32string(T const&v) {
    return to_basic_string<char32_t>(v);
}


inline namespace literals {
    inline namespace string_literals {
         inline string operator "" s(const char *_Str, size_t _Len)
        {	
            return (string(_Str, _Len));
        }

         inline wstring operator "" s(const wchar_t *_Str, size_t _Len)
        {	
            return (wstring(_Str, _Len));
        }

         inline u16string operator "" s(const char16_t *_Str, size_t _Len)
        {	
            return (u16string(_Str, _Len));
        }

         inline u32string operator "" s(const char32_t *_Str, size_t _Len)
        {	
            return (u32string(_Str, _Len));
        }
    }	
}	
#pragma warning(pop)



inline int stoi(const string& _Str, size_t *_Idx = 0,
    int _Base = 10)
{	
    const char *_Ptr = _Str.c_str();
    char *_Eptr;
    errno = 0;
    long _Ans = _CSTD strtol(_Ptr, &_Eptr, _Base);

    if (_Ptr == _Eptr)
        assert(false && "invalid stoi argument");
    if (errno == ERANGE || _Ans < INT_MIN != INT_MAX < _Ans)
        assert(false && "stoi argument out of range");
    if (_Idx != 0)
        *_Idx = (size_t)(_Eptr - _Ptr);
    return ((int)_Ans);
}

inline long stol(const string& _Str, size_t *_Idx = 0,
    int _Base = 10)
{	
    const char *_Ptr = _Str.c_str();
    char *_Eptr;
    errno = 0;
    long _Ans = _CSTD strtol(_Ptr, &_Eptr, _Base);

    if (_Ptr == _Eptr)
        assert(false && "invalid stol argument");
    if (errno == ERANGE)
        assert(false && "stol argument out of range");
    if (_Idx != 0)
        *_Idx = (size_t)(_Eptr - _Ptr);
    return (_Ans);
}

inline unsigned long stoul(const string& _Str, size_t *_Idx = 0,
    int _Base = 10)
{	
    const char *_Ptr = _Str.c_str();
    char *_Eptr;
    errno = 0;
    unsigned long _Ans = _CSTD strtoul(_Ptr, &_Eptr, _Base);

    if (_Ptr == _Eptr)
        assert(false && "invalid stoul argument");
    if (errno == ERANGE)
        assert(false && "stoul argument out of range");
    if (_Idx != 0)
        *_Idx = (size_t)(_Eptr - _Ptr);
    return (_Ans);
}

inline long long stoll(const string& _Str, size_t *_Idx = 0,
    int _Base = 10)
{	
    const char *_Ptr = _Str.c_str();
    char *_Eptr;
    errno = 0;
    long long _Ans = _CSTD strtoll(_Ptr, &_Eptr, _Base);

    if (_Ptr == _Eptr)
        assert(false && "invalid stoll argument");
    if (errno == ERANGE)
        assert(false && "stoll argument out of range");
    if (_Idx != 0)
        *_Idx = (size_t)(_Eptr - _Ptr);
    return (_Ans);
}

inline unsigned long long stoull(const string& _Str, size_t *_Idx = 0,
    int _Base = 10)
{	
    const char *_Ptr = _Str.c_str();
    errno = 0;
    char *_Eptr;
    unsigned long long _Ans = _CSTD strtoull(_Ptr, &_Eptr, _Base);

    if (_Ptr == _Eptr)
        assert(false && "invalid stoull argument");
    if (errno == ERANGE)
        assert(false && "stoull argument out of range");
    if (_Idx != 0)
        *_Idx = (size_t)(_Eptr - _Ptr);
    return (_Ans);
}

inline float stof(const string& _Str, size_t *_Idx = 0)
{	
    const char *_Ptr = _Str.c_str();
    errno = 0;
    char *_Eptr;
    float _Ans = (float)_CSTD strtof(_Ptr, &_Eptr);

    if (_Ptr == _Eptr)
        assert(false && "invalid stof argument");
    if (errno == ERANGE)
        assert(false && "stof argument out of range");
    if (_Idx != 0)
        *_Idx = (size_t)(_Eptr - _Ptr);
    return (_Ans);
}

inline double stod(const string& _Str, size_t *_Idx = 0)
{	
    const char *_Ptr = _Str.c_str();
    errno = 0;
    char *_Eptr;
    double _Ans = _CSTD strtod(_Ptr, &_Eptr);

    if (_Ptr == _Eptr)
        assert(false && "invalid stod argument");
    if (errno == ERANGE)
        assert(false && "stod argument out of range");
    if (_Idx != 0)
        *_Idx = (size_t)(_Eptr - _Ptr);
    return (_Ans);
}

inline long double stold(const string& _Str, size_t *_Idx = 0)
{	
    const char *_Ptr = _Str.c_str();
    errno = 0;
    char *_Eptr;
    long double _Ans = _CSTD strtold(_Ptr, &_Eptr);

    if (_Ptr == _Eptr)
        assert(false && "invalid stold argument");
    if (errno == ERANGE)
        assert(false && "stold argument out of range");
    if (_Idx != 0)
        *_Idx = (size_t)(_Eptr - _Ptr);
    return (_Ans);
}


inline int stoi(const wstring& _Str, size_t *_Idx = 0,
    int _Base = 10)
{	
    const wchar_t *_Ptr = _Str.c_str();
    wchar_t *_Eptr;
    errno = 0;
    long _Ans = _CSTD wcstol(_Ptr, &_Eptr, _Base);

    if (_Ptr == _Eptr)
        assert(false && "invalid stoi argument");
    if (errno == ERANGE || _Ans < INT_MIN != INT_MAX < _Ans)
        assert(false && "stoi argument out of range");
    if (_Idx != 0)
        *_Idx = (size_t)(_Eptr - _Ptr);
    return ((int)_Ans);
}

inline long stol(const wstring& _Str, size_t *_Idx = 0,
    int _Base = 10)
{	
    const wchar_t *_Ptr = _Str.c_str();
    wchar_t *_Eptr;
    errno = 0;
    long _Ans = _CSTD wcstol(_Ptr, &_Eptr, _Base);

    if (_Ptr == _Eptr)
        assert(false && "invalid stol argument");
    if (errno == ERANGE)
        assert(false && "stol argument out of range");
    if (_Idx != 0)
        *_Idx = (size_t)(_Eptr - _Ptr);
    return (_Ans);
}

inline unsigned long stoul(const wstring& _Str, size_t *_Idx = 0,
    int _Base = 10)
{	
    const wchar_t *_Ptr = _Str.c_str();
    wchar_t *_Eptr;
    errno = 0;
    unsigned long _Ans = _CSTD wcstoul(_Ptr, &_Eptr, _Base);

    if (_Ptr == _Eptr)
        assert(false && "invalid stoul argument");
    if (errno == ERANGE)
        assert(false && "stoul argument out of range");
    if (_Idx != 0)
        *_Idx = (size_t)(_Eptr - _Ptr);
    return (_Ans);
}

inline long long stoll(const wstring& _Str, size_t *_Idx = 0,
    int _Base = 10)
{	
    const wchar_t *_Ptr = _Str.c_str();
    wchar_t *_Eptr;
    errno = 0;
    long long _Ans = _CSTD wcstoll(_Ptr, &_Eptr, _Base);

    if (_Ptr == _Eptr)
        assert(false && "invalid stoll argument");
    if (errno == ERANGE)
        assert(false && "stoll argument out of range");
    if (_Idx != 0)
        *_Idx = (size_t)(_Eptr - _Ptr);
    return (_Ans);
}

inline long long stoull(const wstring& _Str, size_t *_Idx = 0,
    int _Base = 10)
{	
    const wchar_t *_Ptr = _Str.c_str();
    errno = 0;
    wchar_t *_Eptr;
    long long _Ans = _CSTD wcstoull(_Ptr, &_Eptr, _Base);

    if (_Ptr == _Eptr)
        assert(false && "invalid stoull argument");
    if (errno == ERANGE)
        assert(false && "stoull argument out of range");
    if (_Idx != 0)
        *_Idx = (size_t)(_Eptr - _Ptr);
    return (_Ans);
}

inline float stof(const wstring& _Str, size_t *_Idx = 0)
{	
    const wchar_t *_Ptr = _Str.c_str();
    errno = 0;
    wchar_t *_Eptr;
    float _Ans = (float)_CSTD wcstof(_Ptr, &_Eptr);

    if (_Ptr == _Eptr)
        assert(false && "invalid stof argument");
    if (errno == ERANGE)
        assert(false && "stof argument out of range");
    if (_Idx != 0)
        *_Idx = (size_t)(_Eptr - _Ptr);
    return (_Ans);
}

inline double stod(const wstring& _Str, size_t *_Idx = 0)
{	
    const wchar_t *_Ptr = _Str.c_str();
    errno = 0;
    wchar_t *_Eptr;
    double _Ans = _CSTD wcstod(_Ptr, &_Eptr);

    if (_Ptr == _Eptr)
        assert(false && "invalid stod argument");
    if (errno == ERANGE)
        assert(false && "stod argument out of range");
    if (_Idx != 0)
        *_Idx = (size_t)(_Eptr - _Ptr);
    return (_Ans);
}

inline long double stold(const wstring& _Str, size_t *_Idx = 0)
{	
    const wchar_t *_Ptr = _Str.c_str();
    errno = 0;
    wchar_t *_Eptr;
    long double _Ans = _CSTD wcstold(_Ptr, &_Eptr);

    if (_Ptr == _Eptr)
        assert(false && "invalid stold argument");
    if (errno == ERANGE)
        assert(false && "stold argument out of range");
    if (_Idx != 0)
        *_Idx = (size_t)(_Eptr - _Ptr);
    return (_Ans);
}









template<typename T, typename F>
inline T
transform_to(F str) noexcept
{
    
    
    if (str.empty())
        return {};
    
    
    return { std::begin(str), std::end(str) };
    
    
};


}
