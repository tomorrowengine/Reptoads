#pragma once




#ifdef PLATFORM_WINDOWS
#include <cstdint>
#include <functional>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h> 



inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw std::exception();
    }
}


namespace std
{
    
    template <typename T>
    inline void hash_combine(std::size_t& seed, const T& v)
    {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template<>
    struct hash<D3D12_SHADER_RESOURCE_VIEW_DESC>
    {
        std::size_t operator()(const D3D12_SHADER_RESOURCE_VIEW_DESC& srvDesc) const noexcept
        {
            std::size_t seed = 0;

            hash_combine(seed, srvDesc.Format);
            hash_combine(seed, srvDesc.ViewDimension);
            hash_combine(seed, srvDesc.Shader4ComponentMapping);

            switch (srvDesc.ViewDimension)
            {
            case D3D12_SRV_DIMENSION_BUFFER:
                hash_combine(seed, srvDesc.Buffer.FirstElement);
                hash_combine(seed, srvDesc.Buffer.NumElements);
                hash_combine(seed, srvDesc.Buffer.StructureByteStride);
                hash_combine(seed, srvDesc.Buffer.Flags);
                break;
            case D3D12_SRV_DIMENSION_TEXTURE1D:
                hash_combine(seed, srvDesc.Texture1D.MostDetailedMip);
                hash_combine(seed, srvDesc.Texture1D.MipLevels);
                hash_combine(seed, srvDesc.Texture1D.ResourceMinLODClamp);
                break;
            case D3D12_SRV_DIMENSION_TEXTURE1DARRAY:
                hash_combine(seed, srvDesc.Texture1DArray.MostDetailedMip);
                hash_combine(seed, srvDesc.Texture1DArray.MipLevels);
                hash_combine(seed, srvDesc.Texture1DArray.FirstArraySlice);
                hash_combine(seed, srvDesc.Texture1DArray.ArraySize);
                hash_combine(seed, srvDesc.Texture1DArray.ResourceMinLODClamp);
                break;
            case D3D12_SRV_DIMENSION_TEXTURE2D:
                hash_combine(seed, srvDesc.Texture2D.MostDetailedMip);
                hash_combine(seed, srvDesc.Texture2D.MipLevels);
                hash_combine(seed, srvDesc.Texture2D.PlaneSlice);
                hash_combine(seed, srvDesc.Texture2D.ResourceMinLODClamp);
                break;
            case D3D12_SRV_DIMENSION_TEXTURE2DARRAY:
                hash_combine(seed, srvDesc.Texture2DArray.MostDetailedMip);
                hash_combine(seed, srvDesc.Texture2DArray.MipLevels);
                hash_combine(seed, srvDesc.Texture2DArray.FirstArraySlice);
                hash_combine(seed, srvDesc.Texture2DArray.ArraySize);
                hash_combine(seed, srvDesc.Texture2DArray.PlaneSlice);
                hash_combine(seed, srvDesc.Texture2DArray.ResourceMinLODClamp);
                break;
            case D3D12_SRV_DIMENSION_TEXTURE2DMS:

                break;
            case D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY:
                hash_combine(seed, srvDesc.Texture2DMSArray.FirstArraySlice);
                hash_combine(seed, srvDesc.Texture2DMSArray.ArraySize);
                break;
            case D3D12_SRV_DIMENSION_TEXTURE3D:
                hash_combine(seed, srvDesc.Texture3D.MostDetailedMip);
                hash_combine(seed, srvDesc.Texture3D.MipLevels);
                hash_combine(seed, srvDesc.Texture3D.ResourceMinLODClamp);
                break;
            case D3D12_SRV_DIMENSION_TEXTURECUBE:
                hash_combine(seed, srvDesc.TextureCube.MostDetailedMip);
                hash_combine(seed, srvDesc.TextureCube.MipLevels);
                hash_combine(seed, srvDesc.TextureCube.ResourceMinLODClamp);
                break;
            case D3D12_SRV_DIMENSION_TEXTURECUBEARRAY:
                hash_combine(seed, srvDesc.TextureCubeArray.MostDetailedMip);
                hash_combine(seed, srvDesc.TextureCubeArray.MipLevels);
                hash_combine(seed, srvDesc.TextureCubeArray.First2DArrayFace);
                hash_combine(seed, srvDesc.TextureCubeArray.NumCubes);
                hash_combine(seed, srvDesc.TextureCubeArray.ResourceMinLODClamp);
                break;
            
            
            
            
            }

            return seed;
        }
    };

    template<>
    struct hash<D3D12_UNORDERED_ACCESS_VIEW_DESC>
    {
        std::size_t operator()(const D3D12_UNORDERED_ACCESS_VIEW_DESC& uavDesc) const noexcept
        {
            std::size_t seed = 0;

            hash_combine(seed, uavDesc.Format);
            hash_combine(seed, uavDesc.ViewDimension);

            switch (uavDesc.ViewDimension)
            {
            case D3D12_UAV_DIMENSION_BUFFER:
                hash_combine(seed, uavDesc.Buffer.FirstElement);
                hash_combine(seed, uavDesc.Buffer.NumElements);
                hash_combine(seed, uavDesc.Buffer.StructureByteStride);
                hash_combine(seed, uavDesc.Buffer.CounterOffsetInBytes);
                hash_combine(seed, uavDesc.Buffer.Flags);
                break;
            case D3D12_UAV_DIMENSION_TEXTURE1D:
                hash_combine(seed, uavDesc.Texture1D.MipSlice);
                break;
            case D3D12_UAV_DIMENSION_TEXTURE1DARRAY:
                hash_combine(seed, uavDesc.Texture1DArray.MipSlice);
                hash_combine(seed, uavDesc.Texture1DArray.FirstArraySlice);
                hash_combine(seed, uavDesc.Texture1DArray.ArraySize);
                break;
            case D3D12_UAV_DIMENSION_TEXTURE2D:
                hash_combine(seed, uavDesc.Texture2D.MipSlice);
                hash_combine(seed, uavDesc.Texture2D.PlaneSlice);
                break;
            case D3D12_UAV_DIMENSION_TEXTURE2DARRAY:
                hash_combine(seed, uavDesc.Texture2DArray.MipSlice);
                hash_combine(seed, uavDesc.Texture2DArray.FirstArraySlice);
                hash_combine(seed, uavDesc.Texture2DArray.ArraySize);
                hash_combine(seed, uavDesc.Texture2DArray.PlaneSlice);
                break;
            case D3D12_UAV_DIMENSION_TEXTURE3D:
                hash_combine(seed, uavDesc.Texture3D.MipSlice);
                hash_combine(seed, uavDesc.Texture3D.FirstWSlice);
                hash_combine(seed, uavDesc.Texture3D.WSize);
                break;
            }

            return seed;
        }
    };
}

namespace Math
{
    constexpr float PI = 3.1415926535897932384626433832795f;
    constexpr float _2PI = 2.0f * PI;
    
    constexpr float Degrees(const float radians)
    {
        return radians * (180.0f / PI);
    }

    
    constexpr float Radians(const float degrees)
    {
        return degrees * (PI / 180.0f);
    }

    template<typename T>
    inline T Deadzone(T val, T deadzone)
    {
        if (std::abs(val) < deadzone)
        {
            return T(0);
        }

        return val;
    }

    
    template<typename T, typename U>
    inline T NormalizeRange(U x, U min, U max)
    {
        return T(x - min) / T(max - min);
    }

    
    template<typename T, typename U>
    inline T ShiftBias(U x, U shift, U bias)
    {
        return T(x * bias) + T(shift);
    }

    
    template <typename T>
    inline T AlignUpWithMask(T value, size_t mask)
    {
        return (T)(((size_t)value + mask) & ~mask);
    }

    template <typename T>
    inline T AlignDownWithMask(T value, size_t mask)
    {
        return (T)((size_t)value & ~mask);
    }

    template <typename T>
    inline T AlignUp(T value, size_t alignment)
    {
        return AlignUpWithMask(value, alignment - 1);
    }

    template <typename T>
    inline T AlignDown(T value, size_t alignment)
    {
        return AlignDownWithMask(value, alignment - 1);
    }

    template <typename T>
    inline bool IsAligned(T value, size_t alignment)
    {
        return 0 == ((size_t)value & (alignment - 1));
    }

    template <typename T>
    inline T DivideByMultiple(T value, size_t alignment)
    {
        return (T)((value + alignment - 1) / alignment);
    }
    

    
    inline uint32_t NextHighestPow2(uint32_t v)
    {
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v++;

        return v;
    }

    
    inline uint64_t NextHighestPow2(uint64_t v)
    {
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v |= v >> 32;
        v++;

        return v;
    }
}

#define STR1(x) #x
#define STR(x) STR1(x)
#define WSTR1(x) L##x
#define WSTR(x) WSTR1(x)
#define NAME_D3D12_OBJECT(x) x->SetName( WSTR(__FILE__ "(" STR(__LINE__) "): " L#x) )
#endif
