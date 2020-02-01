










#include "DirectXTexp.h"

namespace DirectX
{
    extern bool _CalculateMipLevels(_In_ size_t width, _In_ size_t height, _Inout_ size_t& mipLevels);
    extern bool _CalculateMipLevels3D(_In_ size_t width, _In_ size_t height, _In_ size_t depth, _Inout_ size_t& mipLevels);
    extern bool _IsAlphaAllOpaqueBC(_In_ const Image& cImage);
}

using namespace DirectX;




_Use_decl_annotations_
bool DirectX::_DetermineImageArray(
    const TexMetadata& metadata,
    DWORD cpFlags,
    size_t& nImages,
    size_t& pixelSize)
{
    assert(metadata.width > 0 && metadata.height > 0 && metadata.depth > 0);
    assert(metadata.arraySize > 0);
    assert(metadata.mipLevels > 0);

    uint64_t totalPixelSize = 0;
    size_t nimages = 0;

    switch (metadata.dimension)
    {
    case TEX_DIMENSION_TEXTURE1D:
    case TEX_DIMENSION_TEXTURE2D:
        for (size_t item = 0; item < metadata.arraySize; ++item)
        {
            size_t w = metadata.width;
            size_t h = metadata.height;

            for (size_t level = 0; level < metadata.mipLevels; ++level)
            {
                size_t rowPitch, slicePitch;
                if (FAILED(ComputePitch(metadata.format, w, h, rowPitch, slicePitch, cpFlags)))
                {
                    nImages = pixelSize = 0;
                    return false;
                }

                totalPixelSize += uint64_t(slicePitch);
                ++nimages;

                if (h > 1)
                    h >>= 1;

                if (w > 1)
                    w >>= 1;
            }
        }
        break;

    case TEX_DIMENSION_TEXTURE3D:
    {
        size_t w = metadata.width;
        size_t h = metadata.height;
        size_t d = metadata.depth;

        for (size_t level = 0; level < metadata.mipLevels; ++level)
        {
            size_t rowPitch, slicePitch;
            if (FAILED(ComputePitch(metadata.format, w, h, rowPitch, slicePitch, cpFlags)))
            {
                nImages = pixelSize = 0;
                return false;
            }

            for (size_t slice = 0; slice < d; ++slice)
            {
                totalPixelSize += uint64_t(slicePitch);
                ++nimages;
            }

            if (h > 1)
                h >>= 1;

            if (w > 1)
                w >>= 1;

            if (d > 1)
                d >>= 1;
        }
    }
    break;

    default:
        nImages = pixelSize = 0;
        return false;
    }

#if defined(_M_IX86) || defined(_M_ARM) || defined(_M_HYBRID_X86_ARM64)
    static_assert(sizeof(size_t) == 4, "Not a 32-bit platform!");
    if (totalPixelSize > UINT32_MAX)
    {
        nImages = pixelSize = 0;
        return false;
    }
#else
    static_assert(sizeof(size_t) == 8, "Not a 64-bit platform!");
#endif

    nImages = nimages;
    pixelSize = static_cast<size_t>(totalPixelSize);

    return true;
}





_Use_decl_annotations_
bool DirectX::_SetupImageArray(
    uint8_t *pMemory,
    size_t pixelSize,
    const TexMetadata& metadata,
    DWORD cpFlags,
    Image* images,
    size_t nImages)
{
    assert(pMemory);
    assert(pixelSize > 0);
    assert(nImages > 0);

    if (!images)
        return false;

    size_t index = 0;
    uint8_t* pixels = pMemory;
    const uint8_t* pEndBits = pMemory + pixelSize;

    switch (metadata.dimension)
    {
    case TEX_DIMENSION_TEXTURE1D:
    case TEX_DIMENSION_TEXTURE2D:
        if (metadata.arraySize == 0 || metadata.mipLevels == 0)
        {
            return false;
        }

        for (size_t item = 0; item < metadata.arraySize; ++item)
        {
            size_t w = metadata.width;
            size_t h = metadata.height;

            for (size_t level = 0; level < metadata.mipLevels; ++level)
            {
                if (index >= nImages)
                {
                    return false;
                }

                size_t rowPitch, slicePitch;
                if (FAILED(ComputePitch(metadata.format, w, h, rowPitch, slicePitch, cpFlags)))
                    return false;

                images[index].width = w;
                images[index].height = h;
                images[index].format = metadata.format;
                images[index].rowPitch = rowPitch;
                images[index].slicePitch = slicePitch;
                images[index].pixels = pixels;
                ++index;

                pixels += slicePitch;
                if (pixels > pEndBits)
                {
                    return false;
                }

                if (h > 1)
                    h >>= 1;

                if (w > 1)
                    w >>= 1;
            }
        }
        return true;

    case TEX_DIMENSION_TEXTURE3D:
    {
        if (metadata.mipLevels == 0 || metadata.depth == 0)
        {
            return false;
        }

        size_t w = metadata.width;
        size_t h = metadata.height;
        size_t d = metadata.depth;

        for (size_t level = 0; level < metadata.mipLevels; ++level)
        {
            size_t rowPitch, slicePitch;
            if (FAILED(ComputePitch(metadata.format, w, h, rowPitch, slicePitch, cpFlags)))
                return false;

            for (size_t slice = 0; slice < d; ++slice)
            {
                if (index >= nImages)
                {
                    return false;
                }

                
                
                images[index].width = w;
                images[index].height = h;
                images[index].format = metadata.format;
                images[index].rowPitch = rowPitch;
                images[index].slicePitch = slicePitch;
                images[index].pixels = pixels;
                ++index;

                pixels += slicePitch;
                if (pixels > pEndBits)
                {
                    return false;
                }
            }

            if (h > 1)
                h >>= 1;

            if (w > 1)
                w >>= 1;

            if (d > 1)
                d >>= 1;
        }
    }
    return true;

    default:
        return false;
    }
}






ScratchImage& ScratchImage::operator= (ScratchImage&& moveFrom) noexcept
{
    if (this != &moveFrom)
    {
        Release();

        m_nimages = moveFrom.m_nimages;
        m_size = moveFrom.m_size;
        m_metadata = moveFrom.m_metadata;
        m_image = moveFrom.m_image;
        m_memory = moveFrom.m_memory;

        moveFrom.m_nimages = 0;
        moveFrom.m_size = 0;
        moveFrom.m_image = nullptr;
        moveFrom.m_memory = nullptr;
    }
    return *this;
}





_Use_decl_annotations_
HRESULT ScratchImage::Initialize(const TexMetadata& mdata, DWORD flags)
{
    if (!IsValid(mdata.format))
        return E_INVALIDARG;

    if (IsPalettized(mdata.format))
        return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);

    size_t mipLevels = mdata.mipLevels;

    switch (mdata.dimension)
    {
    case TEX_DIMENSION_TEXTURE1D:
        if (!mdata.width || mdata.height != 1 || mdata.depth != 1 || !mdata.arraySize)
            return E_INVALIDARG;

        if (!_CalculateMipLevels(mdata.width, 1, mipLevels))
            return E_INVALIDARG;
        break;

    case TEX_DIMENSION_TEXTURE2D:
        if (!mdata.width || !mdata.height || mdata.depth != 1 || !mdata.arraySize)
            return E_INVALIDARG;

        if (mdata.IsCubemap())
        {
            if ((mdata.arraySize % 6) != 0)
                return E_INVALIDARG;
        }

        if (!_CalculateMipLevels(mdata.width, mdata.height, mipLevels))
            return E_INVALIDARG;
        break;

    case TEX_DIMENSION_TEXTURE3D:
        if (!mdata.width || !mdata.height || !mdata.depth || mdata.arraySize != 1)
            return E_INVALIDARG;

        if (!_CalculateMipLevels3D(mdata.width, mdata.height, mdata.depth, mipLevels))
            return E_INVALIDARG;
        break;

    default:
        return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
    }

    Release();

    m_metadata.width = mdata.width;
    m_metadata.height = mdata.height;
    m_metadata.depth = mdata.depth;
    m_metadata.arraySize = mdata.arraySize;
    m_metadata.mipLevels = mipLevels;
    m_metadata.miscFlags = mdata.miscFlags;
    m_metadata.miscFlags2 = mdata.miscFlags2;
    m_metadata.format = mdata.format;
    m_metadata.dimension = mdata.dimension;

    size_t pixelSize, nimages;
    if (!_DetermineImageArray(m_metadata, flags, nimages, pixelSize))
        return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);

    m_image = new (std::nothrow) Image[nimages];
    if (!m_image)
        return E_OUTOFMEMORY;

    m_nimages = nimages;
    memset(m_image, 0, sizeof(Image) * nimages);

    m_memory = static_cast<uint8_t*>(_aligned_malloc(pixelSize, 16));
    if (!m_memory)
    {
        Release();
        return E_OUTOFMEMORY;
    }
    m_size = pixelSize;
    if (!_SetupImageArray(m_memory, pixelSize, m_metadata, flags, m_image, nimages))
    {
        Release();
        return E_FAIL;
    }

    return S_OK;
}

_Use_decl_annotations_
HRESULT ScratchImage::Initialize1D(DXGI_FORMAT fmt, size_t length, size_t arraySize, size_t mipLevels, DWORD flags)
{
    if (!length || !arraySize)
        return E_INVALIDARG;

    
    HRESULT hr = Initialize2D(fmt, length, 1, arraySize, mipLevels, flags);
    if (FAILED(hr))
        return hr;

    m_metadata.dimension = TEX_DIMENSION_TEXTURE1D;

    return S_OK;
}

_Use_decl_annotations_
HRESULT ScratchImage::Initialize2D(DXGI_FORMAT fmt, size_t width, size_t height, size_t arraySize, size_t mipLevels, DWORD flags)
{
    if (!IsValid(fmt) || !width || !height || !arraySize)
        return E_INVALIDARG;

    if (IsPalettized(fmt))
        return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);

    if (!_CalculateMipLevels(width, height, mipLevels))
        return E_INVALIDARG;

    Release();

    m_metadata.width = width;
    m_metadata.height = height;
    m_metadata.depth = 1;
    m_metadata.arraySize = arraySize;
    m_metadata.mipLevels = mipLevels;
    m_metadata.miscFlags = 0;
    m_metadata.miscFlags2 = 0;
    m_metadata.format = fmt;
    m_metadata.dimension = TEX_DIMENSION_TEXTURE2D;

    size_t pixelSize, nimages;
    if (!_DetermineImageArray(m_metadata, flags, nimages, pixelSize))
        return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);

    m_image = new (std::nothrow) Image[nimages];
    if (!m_image)
        return E_OUTOFMEMORY;

    m_nimages = nimages;
    memset(m_image, 0, sizeof(Image) * nimages);

    m_memory = static_cast<uint8_t*>(_aligned_malloc(pixelSize, 16));
    if (!m_memory)
    {
        Release();
        return E_OUTOFMEMORY;
    }
    m_size = pixelSize;
    if (!_SetupImageArray(m_memory, pixelSize, m_metadata, flags, m_image, nimages))
    {
        Release();
        return E_FAIL;
    }

    return S_OK;
}

_Use_decl_annotations_
HRESULT ScratchImage::Initialize3D(DXGI_FORMAT fmt, size_t width, size_t height, size_t depth, size_t mipLevels, DWORD flags)
{
    if (!IsValid(fmt) || !width || !height || !depth)
        return E_INVALIDARG;

    if (IsPalettized(fmt))
        return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);

    if (!_CalculateMipLevels3D(width, height, depth, mipLevels))
        return E_INVALIDARG;

    Release();

    m_metadata.width = width;
    m_metadata.height = height;
    m_metadata.depth = depth;
    m_metadata.arraySize = 1;    
    m_metadata.mipLevels = mipLevels;
    m_metadata.miscFlags = 0;
    m_metadata.miscFlags2 = 0;
    m_metadata.format = fmt;
    m_metadata.dimension = TEX_DIMENSION_TEXTURE3D;

    size_t pixelSize, nimages;
    if (!_DetermineImageArray(m_metadata, flags, nimages, pixelSize))
        return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);

    m_image = new (std::nothrow) Image[nimages];
    if (!m_image)
    {
        Release();
        return E_OUTOFMEMORY;
    }
    m_nimages = nimages;
    memset(m_image, 0, sizeof(Image) * nimages);

    m_memory = static_cast<uint8_t*>(_aligned_malloc(pixelSize, 16));
    if (!m_memory)
    {
        Release();
        return E_OUTOFMEMORY;
    }
    m_size = pixelSize;

    if (!_SetupImageArray(m_memory, pixelSize, m_metadata, flags, m_image, nimages))
    {
        Release();
        return E_FAIL;
    }

    return S_OK;
}

_Use_decl_annotations_
HRESULT ScratchImage::InitializeCube(DXGI_FORMAT fmt, size_t width, size_t height, size_t nCubes, size_t mipLevels, DWORD flags)
{
    if (!width || !height || !nCubes)
        return E_INVALIDARG;

    
    HRESULT hr = Initialize2D(fmt, width, height, nCubes * 6, mipLevels, flags);
    if (FAILED(hr))
        return hr;

    m_metadata.miscFlags |= TEX_MISC_TEXTURECUBE;

    return S_OK;
}

_Use_decl_annotations_
HRESULT ScratchImage::InitializeFromImage(const Image& srcImage, bool allow1D, DWORD flags)
{
    HRESULT hr = (srcImage.height > 1 || !allow1D)
        ? Initialize2D(srcImage.format, srcImage.width, srcImage.height, 1, 1, flags)
        : Initialize1D(srcImage.format, srcImage.width, 1, 1, flags);

    if (FAILED(hr))
        return hr;

    size_t rowCount = ComputeScanlines(srcImage.format, srcImage.height);
    if (!rowCount)
        return E_UNEXPECTED;

    const uint8_t* sptr = srcImage.pixels;
    if (!sptr)
        return E_POINTER;

    uint8_t* dptr = m_image[0].pixels;
    if (!dptr)
        return E_POINTER;

    size_t spitch = srcImage.rowPitch;
    size_t dpitch = m_image[0].rowPitch;

    size_t size = std::min<size_t>(dpitch, spitch);

    for (size_t y = 0; y < rowCount; ++y)
    {
        memcpy_s(dptr, dpitch, sptr, size);
        sptr += spitch;
        dptr += dpitch;
    }

    return S_OK;
}

_Use_decl_annotations_
HRESULT ScratchImage::InitializeArrayFromImages(const Image* images, size_t nImages, bool allow1D, DWORD flags)
{
    if (!images || !nImages)
        return E_INVALIDARG;

    DXGI_FORMAT format = images[0].format;
    size_t width = images[0].width;
    size_t height = images[0].height;

    for (size_t index = 0; index < nImages; ++index)
    {
        if (!images[index].pixels)
            return E_POINTER;

        if (images[index].format != format || images[index].width != width || images[index].height != height)
        {
            
            return E_FAIL;
        }
    }

    HRESULT hr = (height > 1 || !allow1D)
        ? Initialize2D(format, width, height, nImages, 1, flags)
        : Initialize1D(format, width, nImages, 1, flags);

    if (FAILED(hr))
        return hr;

    size_t rowCount = ComputeScanlines(format, height);
    if (!rowCount)
        return E_UNEXPECTED;

    for (size_t index = 0; index < nImages; ++index)
    {
        const uint8_t* sptr = images[index].pixels;
        if (!sptr)
            return E_POINTER;

        assert(index < m_nimages);
        uint8_t* dptr = m_image[index].pixels;
        if (!dptr)
            return E_POINTER;

        size_t spitch = images[index].rowPitch;
        size_t dpitch = m_image[index].rowPitch;

        size_t size = std::min<size_t>(dpitch, spitch);

        for (size_t y = 0; y < rowCount; ++y)
        {
            memcpy_s(dptr, dpitch, sptr, size);
            sptr += spitch;
            dptr += dpitch;
        }
    }

    return S_OK;
}

_Use_decl_annotations_
HRESULT ScratchImage::InitializeCubeFromImages(const Image* images, size_t nImages, DWORD flags)
{
    if (!images || !nImages)
        return E_INVALIDARG;

    
    if ((nImages % 6) != 0)
        return E_INVALIDARG;

    HRESULT hr = InitializeArrayFromImages(images, nImages, false, flags);
    if (FAILED(hr))
        return hr;

    m_metadata.miscFlags |= TEX_MISC_TEXTURECUBE;

    return S_OK;
}

_Use_decl_annotations_
HRESULT ScratchImage::Initialize3DFromImages(const Image* images, size_t depth, DWORD flags)
{
    if (!images || !depth)
        return E_INVALIDARG;

    DXGI_FORMAT format = images[0].format;
    size_t width = images[0].width;
    size_t height = images[0].height;

    for (size_t slice = 0; slice < depth; ++slice)
    {
        if (!images[slice].pixels)
            return E_POINTER;

        if (images[slice].format != format || images[slice].width != width || images[slice].height != height)
        {
            
            return E_FAIL;
        }
    }

    HRESULT hr = Initialize3D(format, width, height, depth, 1, flags);
    if (FAILED(hr))
        return hr;

    size_t rowCount = ComputeScanlines(format, height);
    if (!rowCount)
        return E_UNEXPECTED;

    for (size_t slice = 0; slice < depth; ++slice)
    {
        const uint8_t* sptr = images[slice].pixels;
        if (!sptr)
            return E_POINTER;

        assert(slice < m_nimages);
        uint8_t* dptr = m_image[slice].pixels;
        if (!dptr)
            return E_POINTER;

        size_t spitch = images[slice].rowPitch;
        size_t dpitch = m_image[slice].rowPitch;

        size_t size = std::min<size_t>(dpitch, spitch);

        for (size_t y = 0; y < rowCount; ++y)
        {
            memcpy_s(dptr, dpitch, sptr, size);
            sptr += spitch;
            dptr += dpitch;
        }
    }

    return S_OK;
}

void ScratchImage::Release()
{
    m_nimages = 0;
    m_size = 0;

    if (m_image)
    {
        delete[] m_image;
        m_image = nullptr;
    }

    if (m_memory)
    {
        _aligned_free(m_memory);
        m_memory = nullptr;
    }

    memset(&m_metadata, 0, sizeof(m_metadata));
}

_Use_decl_annotations_
bool ScratchImage::OverrideFormat(DXGI_FORMAT f)
{
    if (!m_image)
        return false;

    if (!IsValid(f) || IsPlanar(f) || IsPalettized(f))
        return false;

    for (size_t index = 0; index < m_nimages; ++index)
    {
        m_image[index].format = f;
    }

    m_metadata.format = f;

    return true;
}

_Use_decl_annotations_
const Image* ScratchImage::GetImage(size_t mip, size_t item, size_t slice) const
{
    if (mip >= m_metadata.mipLevels)
        return nullptr;

    size_t index = 0;

    switch (m_metadata.dimension)
    {
    case TEX_DIMENSION_TEXTURE1D:
    case TEX_DIMENSION_TEXTURE2D:
        if (slice > 0)
            return nullptr;

        if (item >= m_metadata.arraySize)
            return nullptr;

        index = item*(m_metadata.mipLevels) + mip;
        break;

    case TEX_DIMENSION_TEXTURE3D:
        if (item > 0)
        {
            
            return nullptr;
        }
        else
        {
            size_t d = m_metadata.depth;

            for (size_t level = 0; level < mip; ++level)
            {
                index += d;
                if (d > 1)
                    d >>= 1;
            }

            if (slice >= d)
                return nullptr;

            index += slice;
        }
        break;

    default:
        return nullptr;
    }

    return &m_image[index];
}

bool ScratchImage::IsAlphaAllOpaque() const
{
    if (!m_image)
        return false;

    if (!HasAlpha(m_metadata.format))
        return true;

    if (IsCompressed(m_metadata.format))
    {
        for (size_t index = 0; index < m_nimages; ++index)
        {
            if (!_IsAlphaAllOpaqueBC(m_image[index]))
                return false;
        }
    }
    else
    {
        ScopedAlignedArrayXMVECTOR scanline(static_cast<XMVECTOR*>(_aligned_malloc((sizeof(XMVECTOR)*m_metadata.width), 16)));
        if (!scanline)
            return false;

        static const XMVECTORF32 threshold = { { { 0.997f, 0.997f, 0.997f, 0.997f } } };

        for (size_t index = 0; index < m_nimages; ++index)
        {
#pragma warning( suppress : 6011 )
            const Image& img = m_image[index];

            const uint8_t *pPixels = img.pixels;
            assert(pPixels);

            for (size_t h = 0; h < img.height; ++h)
            {
                if (!_LoadScanline(scanline.get(), img.width, pPixels, img.rowPitch, img.format))
                    return false;

                const XMVECTOR* ptr = scanline.get();
                for (size_t w = 0; w < img.width; ++w)
                {
                    XMVECTOR alpha = XMVectorSplatW(*ptr);
                    if (XMVector4Less(alpha, threshold))
                        return false;
                    ++ptr;
                }

                pPixels += img.rowPitch;
            }
        }
    }

    return true;
}
