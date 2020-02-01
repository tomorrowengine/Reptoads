#include "core/StrHash.h"
#include <cstdint>
#include "memory/String.h"
#include <cassert>
#include <cstdint>

namespace
{
    
    inline uint64_t rotl64(uint64_t val, int shift)
    {
        
        shift &= 63;
        if (!shift)
            return val;
        return (val >> (64 - shift)) | (val << shift);
    }
    
    inline uint32_t rotl32c(uint32_t x, uint32_t n)
    {
        assert(n < 32);
        if (!n) return x;
        return (x << n) | (x >> (32 - n));
    }



#ifdef WIN32
#define FORCE_INLINE    __forceinline
#else 
#define FORCE_INLINE    inline
#endif
#define ROTL32(x,y)     rotl32c(x,y)
#define ROTL64(x,y)     rotl64(x,y)

#define BIG_CONSTANT(x) (x)

    
    
    

    FORCE_INLINE uint32_t getblock(const uint32_t * p, int i)
    {
        return p[i];
    }

    FORCE_INLINE uint64_t getblock(const uint64_t * p, int i)
    {
        return p[i];
    }

    
    

    FORCE_INLINE uint32_t fmix(uint32_t h)
    {
        h ^= h >> 16;
        h *= 0x85ebca6b;
        h ^= h >> 13;
        h *= 0xc2b2ae35;
        h ^= h >> 16;

        return h;
    }

    

    FORCE_INLINE uint64_t fmix(uint64_t k)
    {
        k ^= k >> 33;
        k *= BIG_CONSTANT(0xff51afd7ed558ccd);
        k ^= k >> 33;
        k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
        k ^= k >> 33;

        return k;
    }


}







namespace tbsg{

    unsigned int SimpleHash(const char *str)
    {
        unsigned int key = MurmurHash3_x86_32(str, (unsigned)strlen(str), 0);
        return key;
    };
    std::uint64_t HashString(const ptl::string& a_Str) noexcept {
        uint64_t hash = 0;
        int counter;
        const char* cstr = a_Str.c_str();
        while ((counter = *cstr++))
            hash = counter + (hash << 6) + (hash << 16) - hash;
        return hash;
    }

    uint64_t HashString(const ptl::string& a_Str, const ptl::string& a_Str1) {
        return HashString(a_Str + "\n" + a_Str1);
    }

    uint64_t HashString(const ptl::string& a_Str, const ptl::string& a_Str1, const ptl::string& a_Str2) {
        return HashString(a_Str + "\n" + a_Str1 + "\n" + a_Str2);
    }
    
    
    
    unsigned int MurmurHash3_x86_32(const void * key, int len, uint32_t seed)
    {
        const uint8_t * data = (const uint8_t*)key;
        const int nblocks = len / 4;

        uint32_t h1 = seed;

        uint32_t c1 = 0xcc9e2d51;
        uint32_t c2 = 0x1b873593;

        
        

        const uint32_t * blocks = (const uint32_t *)(data + nblocks * 4);

        for (int i = -nblocks; i; i++)
        {
            uint32_t k1 = getblock(blocks, i);

            k1 *= c1;
            k1 = ROTL32(k1, 15);
            k1 *= c2;

            h1 ^= k1;
            h1 = ROTL32(h1, 13);
            h1 = h1 * 5 + 0xe6546b64;
        }

        
        

        const uint8_t * tail = (const uint8_t*)(data + nblocks * 4);

        uint32_t k1 = 0;

        switch (len & 3)
        {
        case 3: k1 ^= tail[2] << 16;
        case 2: k1 ^= tail[1] << 8;
        case 1: k1 ^= tail[0];
            k1 *= c1; k1 = ROTL32(k1, 15); k1 *= c2; h1 ^= k1;
        };

        
        

        h1 ^= len;

        h1 = fmix(h1);

        return h1;
    }

}
