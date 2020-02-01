#pragma once

namespace ptl
{
    namespace mem{
        constexpr size_t numberOfPools = 2;
        constexpr size_t sizeOfChunks[numberOfPools]{1024,2024 };
        constexpr size_t sizeOfPools[numberOfPools]{ 304857600,304857600};
#ifdef _WIN32
        constexpr bool enable = false;
#else
        constexpr bool enable = true;
#endif
    }
}