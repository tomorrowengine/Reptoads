#pragma once
namespace ptl
{
    struct MemoryResourceOption
    {
        size_t blobSize{};
        size_t memoryAlingment{};

        explicit MemoryResourceOption(size_t blobSize, size_t alignment)
            : blobSize(blobSize),
            memoryAlingment(alignment)
        {
        }


        MemoryResourceOption(const MemoryResourceOption& other) = default;
        MemoryResourceOption(MemoryResourceOption&& other) noexcept = default;
        MemoryResourceOption& operator=(const MemoryResourceOption& other) = default;
        MemoryResourceOption& operator=(MemoryResourceOption&& other) noexcept = default;
    };
    struct PoolResourceOption : MemoryResourceOption
    {
        size_t chunckSize{};
        PoolResourceOption(size_t blobSize, size_t chunckSize, size_t chunckAlingment)
            : MemoryResourceOption(blobSize, chunckAlingment),
            chunckSize(chunckSize)
        {
        }
        

        PoolResourceOption(const MemoryResourceOption& other)
            : MemoryResourceOption(other),
            chunckSize(chunckSize)
        {
        }

        PoolResourceOption(MemoryResourceOption&& other)
            : MemoryResourceOption(other),
            chunckSize(chunckSize)
        {
        }

        PoolResourceOption(const PoolResourceOption& other) = default;
        PoolResourceOption(PoolResourceOption&& other) noexcept = default;
        PoolResourceOption& operator=(const PoolResourceOption& other) = default;
        PoolResourceOption& operator=(PoolResourceOption&& other) noexcept = default;
    };
}

