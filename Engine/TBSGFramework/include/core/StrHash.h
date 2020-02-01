#include "memory/string.h"
#include <cstdint>

namespace tbsg{
unsigned int SimpleHash(const char *str);
unsigned int MurmurHash3_x86_32(const void * key, int len, uint32_t seed);
std::uint64_t HashString(const ptl::string& a_Str) noexcept;
std::uint64_t HashString(const ptl::string& a_Str, const ptl::string& a_Str1);
std::uint64_t HashString(const ptl::string& a_Str, const ptl::string& a_Str1, const ptl::string& a_Str2);

}
