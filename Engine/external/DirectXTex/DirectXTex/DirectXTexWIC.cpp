










#include "DirectXTexp.h"





#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP) && (WINAPI_FAMILY != WINAPI_FAMILY_PHONE_APP)

    #include <shcore.h>
    #pragma comment(lib,"shcore.lib")

#ifdef __cplusplus_winrt

    static inline HRESULT CreateMemoryStream(_Outptr_ IStream** stream)
    {
        auto randomAccessStream = ref new ::Windows::Storage::Streams::InMemoryRandomAccessStream();
        return CreateStreamOverRandomAccessStream(randomAccessStream, IID_PPV_ARGS(stream));
    }

#else

#pragma warning(push)
#pragma warning(disable : 4619 5038)
    #include <wrl\client.h>
    #include <wrl\wrappers\corewrappers.h>
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable : 4471)
    #include <windows.storage.streams.h>
#pragma warning(pop)

    static inline HRESULT CreateMemoryStream(_Outptr_ IStream** stream)
    {
        Microsoft::WRL::ComPtr<ABI::Windows::Storage::Streams::IRandomAccessStream> abiStream;
        HRESULT hr = Windows::Foundation::ActivateInstance(
            Microsoft::WRL::Wrappers::HStringReference(RuntimeClass_Windows_Storage_Streams_InMemoryRandomAccessStream).Get(),
            abiStream.GetAddressOf());

        if (SUCCEEDED(hr))
        {
            hr = CreateStreamOverRandomAccessStream(abiStream.Get(), IID_PPV_ARGS(stream));
        }
        return hr;
    }

#endif 

#else

    #pragma prefast(suppress:6387 28196, "a simple wrapper around an existing annotated function" );
    static inline HRESULT CreateMemoryStream(_Outptr_ IStream** stream)
    {
        return CreateStreamOnHGlobal(nullptr, TRUE, stream);
    }

#endif

using namespace DirectX;
using Microsoft::WRL::ComPtr;

namespace
{

    
    
    

    struct WICConvert
    {
        GUID        source;
        GUID        target;
    };

    const WICConvert g_WICConvert[] =
    {
        
        

        { GUID_WICPixelFormat1bppIndexed,           GUID_WICPixelFormat32bppRGBA }, 
        { GUID_WICPixelFormat2bppIndexed,           GUID_WICPixelFormat32bppRGBA }, 
        { GUID_WICPixelFormat4bppIndexed,           GUID_WICPixelFormat32bppRGBA }, 
        { GUID_WICPixelFormat8bppIndexed,           GUID_WICPixelFormat32bppRGBA }, 

        { GUID_WICPixelFormat2bppGray,              GUID_WICPixelFormat8bppGray }, 
        { GUID_WICPixelFormat4bppGray,              GUID_WICPixelFormat8bppGray }, 

        { GUID_WICPixelFormat16bppGrayFixedPoint,   GUID_WICPixelFormat16bppGrayHalf }, 
        { GUID_WICPixelFormat32bppGrayFixedPoint,   GUID_WICPixelFormat32bppGrayFloat }, 

        { GUID_WICPixelFormat16bppBGR555,           GUID_WICPixelFormat16bppBGRA5551 }, 
        { GUID_WICPixelFormat32bppBGR101010,        GUID_WICPixelFormat32bppRGBA1010102 }, 

        { GUID_WICPixelFormat24bppBGR,              GUID_WICPixelFormat32bppRGBA }, 
        { GUID_WICPixelFormat24bppRGB,              GUID_WICPixelFormat32bppRGBA }, 
        { GUID_WICPixelFormat32bppPBGRA,            GUID_WICPixelFormat32bppRGBA }, 
        { GUID_WICPixelFormat32bppPRGBA,            GUID_WICPixelFormat32bppRGBA }, 

        { GUID_WICPixelFormat48bppRGB,              GUID_WICPixelFormat64bppRGBA }, 
        { GUID_WICPixelFormat48bppBGR,              GUID_WICPixelFormat64bppRGBA }, 
        { GUID_WICPixelFormat64bppBGRA,             GUID_WICPixelFormat64bppRGBA }, 
        { GUID_WICPixelFormat64bppPRGBA,            GUID_WICPixelFormat64bppRGBA }, 
        { GUID_WICPixelFormat64bppPBGRA,            GUID_WICPixelFormat64bppRGBA }, 

        { GUID_WICPixelFormat48bppRGBFixedPoint,    GUID_WICPixelFormat64bppRGBAHalf }, 
        { GUID_WICPixelFormat48bppBGRFixedPoint,    GUID_WICPixelFormat64bppRGBAHalf }, 
        { GUID_WICPixelFormat64bppRGBAFixedPoint,   GUID_WICPixelFormat64bppRGBAHalf }, 
        { GUID_WICPixelFormat64bppBGRAFixedPoint,   GUID_WICPixelFormat64bppRGBAHalf }, 
        { GUID_WICPixelFormat64bppRGBFixedPoint,    GUID_WICPixelFormat64bppRGBAHalf }, 
        { GUID_WICPixelFormat64bppRGBHalf,          GUID_WICPixelFormat64bppRGBAHalf }, 
        { GUID_WICPixelFormat48bppRGBHalf,          GUID_WICPixelFormat64bppRGBAHalf }, 

        { GUID_WICPixelFormat128bppPRGBAFloat,      GUID_WICPixelFormat128bppRGBAFloat }, 
        { GUID_WICPixelFormat128bppRGBFloat,        GUID_WICPixelFormat128bppRGBAFloat }, 
        { GUID_WICPixelFormat128bppRGBAFixedPoint,  GUID_WICPixelFormat128bppRGBAFloat }, 
        { GUID_WICPixelFormat128bppRGBFixedPoint,   GUID_WICPixelFormat128bppRGBAFloat }, 
        { GUID_WICPixelFormat32bppRGBE,             GUID_WICPixelFormat128bppRGBAFloat }, 

        { GUID_WICPixelFormat32bppCMYK,             GUID_WICPixelFormat32bppRGBA }, 
        { GUID_WICPixelFormat64bppCMYK,             GUID_WICPixelFormat64bppRGBA }, 
        { GUID_WICPixelFormat40bppCMYKAlpha,        GUID_WICPixelFormat32bppRGBA }, 
        { GUID_WICPixelFormat80bppCMYKAlpha,        GUID_WICPixelFormat64bppRGBA }, 

    #if (_WIN32_WINNT >= _WIN32_WINNT_WIN8) || defined(_WIN7_PLATFORM_UPDATE)
        { GUID_WICPixelFormat32bppRGB,              GUID_WICPixelFormat32bppRGBA }, 
        { GUID_WICPixelFormat64bppRGB,              GUID_WICPixelFormat64bppRGBA }, 
        { GUID_WICPixelFormat64bppPRGBAHalf,        GUID_WICPixelFormat64bppRGBAHalf }, 
    #endif

        
    };

    
    
    
    DXGI_FORMAT DetermineFormat(
        _In_ const WICPixelFormatGUID& pixelFormat,
        DWORD flags,
        bool iswic2,
        _Out_opt_ WICPixelFormatGUID* pConvert)
    {
        if (pConvert)
            memset(pConvert, 0, sizeof(WICPixelFormatGUID));

        DXGI_FORMAT format = _WICToDXGI(pixelFormat);

        if (format == DXGI_FORMAT_UNKNOWN)
        {
            if (memcmp(&GUID_WICPixelFormat96bppRGBFixedPoint, &pixelFormat, sizeof(WICPixelFormatGUID)) == 0)
            {
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8) || defined(_WIN7_PLATFORM_UPDATE)
                if (iswic2)
                {
                    if (pConvert)
                        memcpy(pConvert, &GUID_WICPixelFormat96bppRGBFloat, sizeof(WICPixelFormatGUID));
                    format = DXGI_FORMAT_R32G32B32_FLOAT;
                }
                else
#else
                UNREFERENCED_PARAMETER(iswic2);
#endif
                {
                    if (pConvert)
                        memcpy(pConvert, &GUID_WICPixelFormat128bppRGBAFloat, sizeof(WICPixelFormatGUID));
                    format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                }
            }
            else
            {
                for (size_t i = 0; i < _countof(g_WICConvert); ++i)
                {
                    if (memcmp(&g_WICConvert[i].source, &pixelFormat, sizeof(WICPixelFormatGUID)) == 0)
                    {
                        if (pConvert)
                            memcpy(pConvert, &g_WICConvert[i].target, sizeof(WICPixelFormatGUID));

                        format = _WICToDXGI(g_WICConvert[i].target);
                        assert(format != DXGI_FORMAT_UNKNOWN);
                        break;
                    }
                }
            }
        }

        
        switch (format)
        {
        case DXGI_FORMAT_B8G8R8A8_UNORM:    
        case DXGI_FORMAT_B8G8R8X8_UNORM:    
            if (flags & WIC_FLAGS_FORCE_RGB)
            {
                format = DXGI_FORMAT_R8G8B8A8_UNORM;
                if (pConvert)
                    memcpy(pConvert, &GUID_WICPixelFormat32bppRGBA, sizeof(WICPixelFormatGUID));
            }
            break;

        case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
            if (flags & WIC_FLAGS_NO_X2_BIAS)
            {
                format = DXGI_FORMAT_R10G10B10A2_UNORM;
                if (pConvert)
                    memcpy(pConvert, &GUID_WICPixelFormat32bppRGBA1010102, sizeof(WICPixelFormatGUID));
            }
            break;

        case DXGI_FORMAT_B5G5R5A1_UNORM:
        case DXGI_FORMAT_B5G6R5_UNORM:
            if (flags & WIC_FLAGS_NO_16BPP)
            {
                format = DXGI_FORMAT_R8G8B8A8_UNORM;
                if (pConvert)
                    memcpy(pConvert, &GUID_WICPixelFormat32bppRGBA, sizeof(WICPixelFormatGUID));
            }
            break;

        case DXGI_FORMAT_R1_UNORM:
            if (!(flags & WIC_FLAGS_ALLOW_MONO))
            {
                
                format = DXGI_FORMAT_R8_UNORM;
                if (pConvert)
                    memcpy(pConvert, &GUID_WICPixelFormat8bppGray, sizeof(WICPixelFormatGUID));
            }
            break;

        default:
            break;
        }

        return format;
    }


    
    
    
    HRESULT DecodeMetadata(
        DWORD flags,
        bool iswic2,
        _In_ IWICBitmapDecoder *decoder,
        _In_ IWICBitmapFrameDecode *frame,
        _Out_ TexMetadata& metadata,
        _Out_opt_ WICPixelFormatGUID* pConvert,
        _In_opt_ std::function<void(IWICMetadataQueryReader*)> getMQR)
    {
        if (!decoder || !frame)
            return E_POINTER;

        memset(&metadata, 0, sizeof(TexMetadata));
        metadata.depth = 1;
        metadata.mipLevels = 1;
        metadata.dimension = TEX_DIMENSION_TEXTURE2D;

        UINT w, h;
        HRESULT hr = frame->GetSize(&w, &h);
        if (FAILED(hr))
            return hr;

        metadata.width = w;
        metadata.height = h;

        if (flags & WIC_FLAGS_ALL_FRAMES)
        {
            UINT fcount;
            hr = decoder->GetFrameCount(&fcount);
            if (FAILED(hr))
                return hr;

            metadata.arraySize = fcount;
        }
        else
            metadata.arraySize = 1;

        WICPixelFormatGUID pixelFormat;
        hr = frame->GetPixelFormat(&pixelFormat);
        if (FAILED(hr))
            return hr;

        metadata.format = DetermineFormat(pixelFormat, flags, iswic2, pConvert);
        if (metadata.format == DXGI_FORMAT_UNKNOWN)
            return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);

        if (!(flags & WIC_FLAGS_IGNORE_SRGB))
        {
            GUID containerFormat;
            hr = decoder->GetContainerFormat(&containerFormat);
            if (FAILED(hr))
                return hr;

            ComPtr<IWICMetadataQueryReader> metareader;
            hr = frame->GetMetadataQueryReader(metareader.GetAddressOf());
            if (SUCCEEDED(hr))
            {
                
                bool sRGB = false;

                PROPVARIANT value;
                PropVariantInit(&value);

                if (memcmp(&containerFormat, &GUID_ContainerFormatPng, sizeof(GUID)) == 0)
                {
                    
                    if (SUCCEEDED(metareader->GetMetadataByName(L"/sRGB/RenderingIntent", &value)) && value.vt == VT_UI1)
                    {
                        sRGB = true;
                    }
                }
#if defined(_XBOX_ONE) && defined(_TITLE)
                else if (memcmp(&containerFormat, &GUID_ContainerFormatJpeg, sizeof(GUID)) == 0)
                {
                    if (SUCCEEDED(metareader->GetMetadataByName(L"/app1/ifd/exif/{ushort=40961}", &value)) && value.vt == VT_UI2 && value.uiVal == 1)
                    {
                        sRGB = true;
                    }
                }
                else if (memcmp(&containerFormat, &GUID_ContainerFormatTiff, sizeof(GUID)) == 0)
                {
                    if (SUCCEEDED(metareader->GetMetadataByName(L"/ifd/exif/{ushort=40961}", &value)) && value.vt == VT_UI2 && value.uiVal == 1)
                    {
                        sRGB = true;
                    }
                }
#else
                else if (SUCCEEDED(metareader->GetMetadataByName(L"System.Image.ColorSpace", &value)) && value.vt == VT_UI2 && value.uiVal == 1)
                {
                    sRGB = true;
                }
#endif

                (void)PropVariantClear(&value);

                if (sRGB)
                    metadata.format = MakeSRGB(metadata.format);
            }
            else if (hr == WINCODEC_ERR_UNSUPPORTEDOPERATION)
            {
                
                hr = S_OK;
            }
        }

        if (getMQR)
        {
            ComPtr<IWICMetadataQueryReader> metareader;
            if (SUCCEEDED(frame->GetMetadataQueryReader(metareader.GetAddressOf())))
            {
                getMQR(metareader.Get());
            }
        }

        return hr;
    }


    
    
    
    HRESULT DecodeSingleFrame(
        DWORD flags,
        const TexMetadata& metadata,
        const WICPixelFormatGUID& convertGUID,
        _In_ IWICBitmapFrameDecode *frame,
        _Inout_ ScratchImage& image)
    {
        if (!frame)
            return E_POINTER;

        HRESULT hr = image.Initialize2D(metadata.format, metadata.width, metadata.height, 1, 1);
        if (FAILED(hr))
            return hr;

        const Image *img = image.GetImage(0, 0, 0);
        if (!img)
            return E_POINTER;

        bool iswic2 = false;
        IWICImagingFactory* pWIC = GetWICFactory(iswic2);
        if (!pWIC)
            return E_NOINTERFACE;

        if (img->rowPitch > UINT32_MAX || img->slicePitch > UINT32_MAX)
            return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);

        if (memcmp(&convertGUID, &GUID_NULL, sizeof(GUID)) == 0)
        {
            hr = frame->CopyPixels(nullptr, static_cast<UINT>(img->rowPitch), static_cast<UINT>(img->slicePitch), img->pixels);
            if (FAILED(hr))
                return hr;
        }
        else
        {
            ComPtr<IWICFormatConverter> FC;
            hr = pWIC->CreateFormatConverter(FC.GetAddressOf());
            if (FAILED(hr))
                return hr;

            WICPixelFormatGUID pixelFormat;
            hr = frame->GetPixelFormat(&pixelFormat);
            if (FAILED(hr))
                return hr;

            BOOL canConvert = FALSE;
            hr = FC->CanConvert(pixelFormat, convertGUID, &canConvert);
            if (FAILED(hr) || !canConvert)
            {
                return E_UNEXPECTED;
            }

            hr = FC->Initialize(frame, convertGUID, _GetWICDither(flags), nullptr, 0, WICBitmapPaletteTypeMedianCut);
            if (FAILED(hr))
                return hr;

            hr = FC->CopyPixels(nullptr, static_cast<UINT>(img->rowPitch), static_cast<UINT>(img->slicePitch), img->pixels);
            if (FAILED(hr))
                return hr;
        }

        return S_OK;
    }


    
    
    
    HRESULT DecodeMultiframe(
        DWORD flags,
        const TexMetadata& metadata,
        _In_ IWICBitmapDecoder *decoder,
        _Inout_ ScratchImage& image)
    {
        if (!decoder)
            return E_POINTER;

        HRESULT hr = image.Initialize2D(metadata.format, metadata.width, metadata.height, metadata.arraySize, 1);
        if (FAILED(hr))
            return hr;

        bool iswic2 = false;
        IWICImagingFactory* pWIC = GetWICFactory(iswic2);
        if (!pWIC)
            return E_NOINTERFACE;

        WICPixelFormatGUID sourceGUID;
        if (!_DXGIToWIC(metadata.format, sourceGUID))
            return E_FAIL;

        for (size_t index = 0; index < metadata.arraySize; ++index)
        {
            const Image* img = image.GetImage(0, index, 0);
            if (!img)
                return E_POINTER;

            if (img->rowPitch > UINT32_MAX || img->slicePitch > UINT32_MAX)
                return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);

            ComPtr<IWICBitmapFrameDecode> frame;
            hr = decoder->GetFrame(static_cast<UINT>(index), frame.GetAddressOf());
            if (FAILED(hr))
                return hr;

            WICPixelFormatGUID pfGuid;
            hr = frame->GetPixelFormat(&pfGuid);
            if (FAILED(hr))
                return hr;

            UINT w, h;
            hr = frame->GetSize(&w, &h);
            if (FAILED(hr))
                return hr;

            if (w == metadata.width && h == metadata.height)
            {
                
                if (memcmp(&pfGuid, &sourceGUID, sizeof(WICPixelFormatGUID)) == 0)
                {
                    hr = frame->CopyPixels(nullptr, static_cast<UINT>(img->rowPitch), static_cast<UINT>(img->slicePitch), img->pixels);
                    if (FAILED(hr))
                        return hr;
                }
                else
                {
                    ComPtr<IWICFormatConverter> FC;
                    hr = pWIC->CreateFormatConverter(FC.GetAddressOf());
                    if (FAILED(hr))
                        return hr;

                    BOOL canConvert = FALSE;
                    hr = FC->CanConvert(pfGuid, sourceGUID, &canConvert);
                    if (FAILED(hr) || !canConvert)
                    {
                        return E_UNEXPECTED;
                    }

                    hr = FC->Initialize(frame.Get(), sourceGUID, _GetWICDither(flags), nullptr, 0, WICBitmapPaletteTypeMedianCut);
                    if (FAILED(hr))
                        return hr;

                    hr = FC->CopyPixels(nullptr, static_cast<UINT>(img->rowPitch), static_cast<UINT>(img->slicePitch), img->pixels);
                    if (FAILED(hr))
                        return hr;
                }
            }
            else
            {
                
                ComPtr<IWICBitmapScaler> scaler;
                hr = pWIC->CreateBitmapScaler(scaler.GetAddressOf());
                if (FAILED(hr))
                    return hr;

                hr = scaler->Initialize(frame.Get(), static_cast<UINT>(metadata.width), static_cast<UINT>(metadata.height), _GetWICInterp(flags));
                if (FAILED(hr))
                    return hr;

                WICPixelFormatGUID pfScaler;
                hr = scaler->GetPixelFormat(&pfScaler);
                if (FAILED(hr))
                    return hr;

                if (memcmp(&pfScaler, &sourceGUID, sizeof(WICPixelFormatGUID)) == 0)
                {
                    hr = scaler->CopyPixels(nullptr, static_cast<UINT>(img->rowPitch), static_cast<UINT>(img->slicePitch), img->pixels);
                    if (FAILED(hr))
                        return hr;
                }
                else
                {
                    
                    
                    ComPtr<IWICFormatConverter> FC;
                    hr = pWIC->CreateFormatConverter(FC.GetAddressOf());
                    if (FAILED(hr))
                        return hr;

                    BOOL canConvert = FALSE;
                    hr = FC->CanConvert(pfScaler, sourceGUID, &canConvert);
                    if (FAILED(hr) || !canConvert)
                    {
                        return E_UNEXPECTED;
                    }

                    hr = FC->Initialize(scaler.Get(), sourceGUID, _GetWICDither(flags), nullptr, 0, WICBitmapPaletteTypeMedianCut);
                    if (FAILED(hr))
                        return hr;

                    hr = FC->CopyPixels(nullptr, static_cast<UINT>(img->rowPitch), static_cast<UINT>(img->slicePitch), img->pixels);
                    if (FAILED(hr))
                        return hr;
                }
            }
        }

        return S_OK;
    }


    
    
    
    HRESULT EncodeMetadata(
        _In_ IWICBitmapFrameEncode* frame,
        const GUID& containerFormat,
        DXGI_FORMAT format)
    {
        if (!frame)
            return E_POINTER;

        ComPtr<IWICMetadataQueryWriter> metawriter;
        HRESULT hr = frame->GetMetadataQueryWriter(metawriter.GetAddressOf());
        if (SUCCEEDED(hr))
        {
            PROPVARIANT value;
            PropVariantInit(&value);

            bool sRGB = IsSRGB(format);

            value.vt = VT_LPSTR;
            value.pszVal = const_cast<char*>("DirectXTex");

            if (memcmp(&containerFormat, &GUID_ContainerFormatPng, sizeof(GUID)) == 0)
            {
                
                (void)metawriter->SetMetadataByName(L"/tEXt/{str=Software}", &value);

                
                if (sRGB)
                {
                    value.vt = VT_UI1;
                    value.bVal = 0;
                    (void)metawriter->SetMetadataByName(L"/sRGB/RenderingIntent", &value);
                }
                else
                {
                    
                    value.vt = VT_UI4;
                    value.uintVal = 100000; 
                    (void)metawriter->SetMetadataByName(L"/gAMA/ImageGamma", &value);

                    
                    (void)metawriter->RemoveMetadataByName(L"/sRGB/RenderingIntent");
                }
            }
#if defined(_XBOX_ONE) && defined(_TITLE)
            else if (memcmp(&containerFormat, &GUID_ContainerFormatJpeg, sizeof(GUID)) == 0)
            {
                
                (void)metawriter->SetMetadataByName(L"/app1/ifd/{ushort=305}", &value);

                if (sRGB)
                {
                    
                    value.vt = VT_UI2;
                    value.uiVal = 1;
                    (void)metawriter->SetMetadataByName(L"/app1/ifd/exif/{ushort=40961}", &value);
                }
            }
            else if (memcmp(&containerFormat, &GUID_ContainerFormatTiff, sizeof(GUID)) == 0)
            {
                
                (void)metawriter->SetMetadataByName(L"/ifd/{ushort=305}", &value);

                if (sRGB)
                {
                    
                    value.vt = VT_UI2;
                    value.uiVal = 1;
                    (void)metawriter->SetMetadataByName(L"/ifd/exif/{ushort=40961}", &value);
                }
            }
#else
            else
            {
                
                (void)metawriter->SetMetadataByName(L"System.ApplicationName", &value);

                if (sRGB)
                {
                    
                    value.vt = VT_UI2;
                    value.uiVal = 1;
                    (void)metawriter->SetMetadataByName(L"System.Image.ColorSpace", &value);
                }
            }
#endif
        }
        else if (hr == WINCODEC_ERR_UNSUPPORTEDOPERATION)
        {
            
            hr = S_OK;
        }

        return hr;
    }


    
    
    
    HRESULT EncodeImage(
        const Image& image,
        DWORD flags,
        _In_ REFGUID containerFormat,
        _In_ IWICBitmapFrameEncode* frame,
        _In_opt_ IPropertyBag2* props,
        _In_opt_ const GUID* targetFormat)
    {
        if (!frame)
            return E_INVALIDARG;

        if (!image.pixels)
            return E_POINTER;

        WICPixelFormatGUID pfGuid;
        if (!_DXGIToWIC(image.format, pfGuid))
            return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);

        HRESULT hr = frame->Initialize(props);
        if (FAILED(hr))
            return hr;

        if ((image.width > UINT32_MAX) || (image.height > UINT32_MAX))
            return E_INVALIDARG;

        if (image.rowPitch > UINT32_MAX || image.slicePitch > UINT32_MAX)
            return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);

        hr = frame->SetSize(static_cast<UINT>(image.width), static_cast<UINT>(image.height));
        if (FAILED(hr))
            return hr;

        hr = frame->SetResolution(72, 72);
        if (FAILED(hr))
            return hr;

        WICPixelFormatGUID targetGuid = (targetFormat) ? (*targetFormat) : pfGuid;
        hr = frame->SetPixelFormat(&targetGuid);
        if (FAILED(hr))
            return hr;

        if (targetFormat && memcmp(targetFormat, &targetGuid, sizeof(WICPixelFormatGUID)) != 0)
        {
            
            return E_FAIL;
        }

        hr = EncodeMetadata(frame, containerFormat, image.format);
        if (FAILED(hr))
            return hr;

        if (memcmp(&targetGuid, &pfGuid, sizeof(WICPixelFormatGUID)) != 0)
        {
            
            bool iswic2 = false;
            IWICImagingFactory* pWIC = GetWICFactory(iswic2);
            if (!pWIC)
                return E_NOINTERFACE;

            ComPtr<IWICBitmap> source;
            hr = pWIC->CreateBitmapFromMemory(static_cast<UINT>(image.width), static_cast<UINT>(image.height), pfGuid,
                static_cast<UINT>(image.rowPitch), static_cast<UINT>(image.slicePitch),
                image.pixels, source.GetAddressOf());
            if (FAILED(hr))
                return hr;

            ComPtr<IWICFormatConverter> FC;
            hr = pWIC->CreateFormatConverter(FC.GetAddressOf());
            if (FAILED(hr))
                return hr;

            BOOL canConvert = FALSE;
            hr = FC->CanConvert(pfGuid, targetGuid, &canConvert);
            if (FAILED(hr) || !canConvert)
            {
                return E_UNEXPECTED;
            }

            hr = FC->Initialize(source.Get(), targetGuid, _GetWICDither(flags), nullptr, 0, WICBitmapPaletteTypeMedianCut);
            if (FAILED(hr))
                return hr;

            WICRect rect = { 0, 0, static_cast<INT>(image.width), static_cast<INT>(image.height) };
            hr = frame->WriteSource(FC.Get(), &rect);
            if (FAILED(hr))
                return hr;
        }
        else
        {
            
            hr = frame->WritePixels(static_cast<UINT>(image.height), static_cast<UINT>(image.rowPitch), static_cast<UINT>(image.slicePitch),
                reinterpret_cast<uint8_t*>(image.pixels));
            if (FAILED(hr))
                return hr;
        }

        hr = frame->Commit();
        if (FAILED(hr))
            return hr;

        return S_OK;
    }

    HRESULT EncodeSingleFrame(
        const Image& image,
        DWORD flags,
        _In_ REFGUID containerFormat,
        _Inout_ IStream* stream,
        _In_opt_ const GUID* targetFormat,
        _In_opt_ std::function<void(IPropertyBag2*)> setCustomProps)
    {
        if (!stream)
            return E_INVALIDARG;

        
        bool iswic2 = false;
        IWICImagingFactory* pWIC = GetWICFactory(iswic2);
        if (!pWIC)
            return E_NOINTERFACE;

        ComPtr<IWICBitmapEncoder> encoder;
        HRESULT hr = pWIC->CreateEncoder(containerFormat, nullptr, encoder.GetAddressOf());
        if (FAILED(hr))
            return hr;

        hr = encoder->Initialize(stream, WICBitmapEncoderNoCache);
        if (FAILED(hr))
            return hr;

        ComPtr<IWICBitmapFrameEncode> frame;
        ComPtr<IPropertyBag2> props;
        hr = encoder->CreateNewFrame(frame.GetAddressOf(), props.GetAddressOf());
        if (FAILED(hr))
            return hr;

        if (memcmp(&containerFormat, &GUID_ContainerFormatBmp, sizeof(WICPixelFormatGUID)) == 0 && iswic2)
        {
            
            PROPBAG2 option = {};
            option.pstrName = const_cast<wchar_t*>(L"EnableV5Header32bppBGRA");

            VARIANT varValue;
            varValue.vt = VT_BOOL;
            varValue.boolVal = VARIANT_TRUE;
            (void)props->Write(1, &option, &varValue);
        }

        if (setCustomProps)
        {
            setCustomProps(props.Get());
        }

        hr = EncodeImage(image, flags, containerFormat, frame.Get(), props.Get(), targetFormat);
        if (FAILED(hr))
            return hr;

        hr = encoder->Commit();
        if (FAILED(hr))
            return hr;

        return S_OK;
    }


    
    
    
    HRESULT EncodeMultiframe(
        _In_reads_(nimages) const Image* images,
        size_t nimages,
        DWORD flags,
        _In_ REFGUID containerFormat,
        _Inout_ IStream* stream,
        _In_opt_ const GUID* targetFormat,
        _In_opt_ std::function<void(IPropertyBag2*)> setCustomProps)
    {
        if (!stream || nimages < 2)
            return E_INVALIDARG;

        if (!images)
            return E_POINTER;

        
        bool iswic2 = false;
        IWICImagingFactory* pWIC = GetWICFactory(iswic2);
        if (!pWIC)
            return E_NOINTERFACE;

        ComPtr<IWICBitmapEncoder> encoder;
        HRESULT hr = pWIC->CreateEncoder(containerFormat, nullptr, encoder.GetAddressOf());
        if (FAILED(hr))
            return hr;

        ComPtr<IWICBitmapEncoderInfo> einfo;
        hr = encoder->GetEncoderInfo(einfo.GetAddressOf());
        if (FAILED(hr))
            return hr;

        BOOL mframe = FALSE;
        hr = einfo->DoesSupportMultiframe(&mframe);
        if (FAILED(hr))
            return hr;

        if (!mframe)
            return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);

        hr = encoder->Initialize(stream, WICBitmapEncoderNoCache);
        if (FAILED(hr))
            return hr;

        for (size_t index = 0; index < nimages; ++index)
        {
            ComPtr<IWICBitmapFrameEncode> frame;
            ComPtr<IPropertyBag2> props;
            hr = encoder->CreateNewFrame(frame.GetAddressOf(), props.GetAddressOf());
            if (FAILED(hr))
                return hr;

            if (setCustomProps)
            {
                setCustomProps(props.Get());
            }

            hr = EncodeImage(images[index], flags, containerFormat, frame.Get(), props.Get(), targetFormat);
            if (FAILED(hr))
                return hr;
        }

        hr = encoder->Commit();
        if (FAILED(hr))
            return hr;

        return S_OK;
    }
}









_Use_decl_annotations_
HRESULT DirectX::GetMetadataFromWICMemory(
    const void* pSource,
    size_t size,
    DWORD flags,
    TexMetadata& metadata,
    std::function<void(IWICMetadataQueryReader*)> getMQR)
{
    if (!pSource || size == 0)
        return E_INVALIDARG;

    if (size > UINT32_MAX)
        return HRESULT_FROM_WIN32(ERROR_FILE_TOO_LARGE);

    bool iswic2 = false;
    IWICImagingFactory* pWIC = GetWICFactory(iswic2);
    if (!pWIC)
        return E_NOINTERFACE;

    
    ComPtr<IWICStream> stream;
    HRESULT hr = pWIC->CreateStream(stream.GetAddressOf());
    if (FAILED(hr))
        return hr;

    hr = stream->InitializeFromMemory(static_cast<BYTE*>(const_cast<void*>(pSource)),
        static_cast<UINT>(size));
    if (FAILED(hr))
        return hr;

    
    ComPtr<IWICBitmapDecoder> decoder;
    hr = pWIC->CreateDecoderFromStream(stream.Get(), nullptr, WICDecodeMetadataCacheOnDemand, decoder.GetAddressOf());
    if (FAILED(hr))
        return hr;

    ComPtr<IWICBitmapFrameDecode> frame;
    hr = decoder->GetFrame(0, frame.GetAddressOf());
    if (FAILED(hr))
        return hr;

    
    hr = DecodeMetadata(flags, iswic2, decoder.Get(), frame.Get(), metadata, nullptr, getMQR);
    if (FAILED(hr))
        return hr;

    return S_OK;
}





_Use_decl_annotations_
HRESULT DirectX::GetMetadataFromWICFile(
    const wchar_t* szFile,
    DWORD flags,
    TexMetadata& metadata,
    std::function<void(IWICMetadataQueryReader*)> getMQR)
{
    if (!szFile)
        return E_INVALIDARG;

    bool iswic2 = false;
    IWICImagingFactory* pWIC = GetWICFactory(iswic2);
    if (!pWIC)
        return E_NOINTERFACE;

    
    ComPtr<IWICBitmapDecoder> decoder;
    HRESULT hr = pWIC->CreateDecoderFromFilename(szFile, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, decoder.GetAddressOf());
    if (FAILED(hr))
        return hr;

    ComPtr<IWICBitmapFrameDecode> frame;
    hr = decoder->GetFrame(0, frame.GetAddressOf());
    if (FAILED(hr))
        return hr;

    
    hr = DecodeMetadata(flags, iswic2, decoder.Get(), frame.Get(), metadata, nullptr, getMQR);
    if (FAILED(hr))
        return hr;

    return S_OK;
}





_Use_decl_annotations_
HRESULT DirectX::LoadFromWICMemory(
    const void* pSource,
    size_t size,
    DWORD flags,
    TexMetadata* metadata,
    ScratchImage& image,
    std::function<void(IWICMetadataQueryReader*)> getMQR)
{
    if (!pSource || size == 0)
        return E_INVALIDARG;

    if (size > UINT32_MAX)
        return HRESULT_FROM_WIN32(ERROR_FILE_TOO_LARGE);

    bool iswic2 = false;
    IWICImagingFactory* pWIC = GetWICFactory(iswic2);
    if (!pWIC)
        return E_NOINTERFACE;

    image.Release();

    
    ComPtr<IWICStream> stream;
    HRESULT hr = pWIC->CreateStream(stream.GetAddressOf());
    if (FAILED(hr))
        return hr;

    hr = stream->InitializeFromMemory(static_cast<uint8_t*>(const_cast<void*>(pSource)), static_cast<DWORD>(size));
    if (FAILED(hr))
        return hr;

    
    ComPtr<IWICBitmapDecoder> decoder;
    hr = pWIC->CreateDecoderFromStream(stream.Get(), nullptr, WICDecodeMetadataCacheOnDemand, decoder.GetAddressOf());
    if (FAILED(hr))
        return hr;

    ComPtr<IWICBitmapFrameDecode> frame;
    hr = decoder->GetFrame(0, frame.GetAddressOf());
    if (FAILED(hr))
        return hr;

    
    TexMetadata mdata;
    WICPixelFormatGUID convertGUID = {};
    hr = DecodeMetadata(flags, iswic2, decoder.Get(), frame.Get(), mdata, &convertGUID, getMQR);
    if (FAILED(hr))
        return hr;

    if ((mdata.arraySize > 1) && (flags & WIC_FLAGS_ALL_FRAMES))
    {
        hr = DecodeMultiframe(flags, mdata, decoder.Get(), image);
    }
    else
    {
        hr = DecodeSingleFrame(flags, mdata, convertGUID, frame.Get(), image);
    }

    if (FAILED(hr))
    {
        image.Release();
        return hr;
    }

    if (metadata)
        memcpy(metadata, &mdata, sizeof(TexMetadata));

    return S_OK;
}





_Use_decl_annotations_
HRESULT DirectX::LoadFromWICFile(
    const wchar_t* szFile,
    DWORD flags,
    TexMetadata* metadata,
    ScratchImage& image,
    std::function<void(IWICMetadataQueryReader*)> getMQR)
{
    if (!szFile)
        return E_INVALIDARG;

    bool iswic2 = false;
    IWICImagingFactory* pWIC = GetWICFactory(iswic2);
    if (!pWIC)
        return E_NOINTERFACE;

    image.Release();

    
    ComPtr<IWICBitmapDecoder> decoder;
    HRESULT hr = pWIC->CreateDecoderFromFilename(szFile, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, decoder.GetAddressOf());
    if (FAILED(hr))
        return hr;

    ComPtr<IWICBitmapFrameDecode> frame;
    hr = decoder->GetFrame(0, frame.GetAddressOf());
    if (FAILED(hr))
        return hr;

    
    TexMetadata mdata;
    WICPixelFormatGUID convertGUID = {};
    hr = DecodeMetadata(flags, iswic2, decoder.Get(), frame.Get(), mdata, &convertGUID, getMQR);
    if (FAILED(hr))
        return hr;

    if ((mdata.arraySize > 1) && (flags & WIC_FLAGS_ALL_FRAMES))
    {
        hr = DecodeMultiframe(flags, mdata, decoder.Get(), image);
    }
    else
    {
        hr = DecodeSingleFrame(flags, mdata, convertGUID, frame.Get(), image);
    }

    if (FAILED(hr))
    {
        image.Release();
        return hr;
    }

    if (metadata)
        memcpy(metadata, &mdata, sizeof(TexMetadata));

    return S_OK;
}





_Use_decl_annotations_
HRESULT DirectX::SaveToWICMemory(
    const Image& image,
    DWORD flags,
    REFGUID containerFormat,
    Blob& blob,
    const GUID* targetFormat,
    std::function<void(IPropertyBag2*)> setCustomProps)
{
    if (!image.pixels)
        return E_POINTER;

    blob.Release();

    ComPtr<IStream> stream;
    HRESULT hr = CreateMemoryStream(stream.GetAddressOf());
    if (FAILED(hr))
        return hr;

    hr = EncodeSingleFrame(image, flags, containerFormat, stream.Get(), targetFormat, setCustomProps);
    if (FAILED(hr))
        return hr;

    
    STATSTG stat;
    hr = stream->Stat(&stat, STATFLAG_NONAME);
    if (FAILED(hr))
        return hr;

    if (stat.cbSize.HighPart > 0)
        return HRESULT_FROM_WIN32(ERROR_FILE_TOO_LARGE);

    hr = blob.Initialize(stat.cbSize.LowPart);
    if (FAILED(hr))
        return hr;

    LARGE_INTEGER li = {};
    hr = stream->Seek(li, STREAM_SEEK_SET, nullptr);
    if (FAILED(hr))
        return hr;

    DWORD bytesRead;
    hr = stream->Read(blob.GetBufferPointer(), static_cast<ULONG>(blob.GetBufferSize()), &bytesRead);
    if (FAILED(hr))
        return hr;

    if (bytesRead != blob.GetBufferSize())
        return E_FAIL;

    return S_OK;
}

_Use_decl_annotations_
HRESULT DirectX::SaveToWICMemory(
    const Image* images,
    size_t nimages,
    DWORD flags,
    REFGUID containerFormat,
    Blob& blob,
    const GUID* targetFormat,
    std::function<void(IPropertyBag2*)> setCustomProps)
{
    if (!images || nimages == 0)
        return E_INVALIDARG;

    blob.Release();

    ComPtr<IStream> stream;
    HRESULT hr = CreateMemoryStream(stream.GetAddressOf());
    if (FAILED(hr))
        return hr;

    if (nimages > 1)
        hr = EncodeMultiframe(images, nimages, flags, containerFormat, stream.Get(), targetFormat, setCustomProps);
    else
        hr = EncodeSingleFrame(images[0], flags, containerFormat, stream.Get(), targetFormat, setCustomProps);

    if (FAILED(hr))
        return hr;

    
    STATSTG stat;
    hr = stream->Stat(&stat, STATFLAG_NONAME);
    if (FAILED(hr))
        return hr;

    if (stat.cbSize.HighPart > 0)
        return HRESULT_FROM_WIN32(ERROR_FILE_TOO_LARGE);

    hr = blob.Initialize(stat.cbSize.LowPart);
    if (FAILED(hr))
        return hr;

    LARGE_INTEGER li = {};
    hr = stream->Seek(li, STREAM_SEEK_SET, nullptr);
    if (FAILED(hr))
        return hr;

    DWORD bytesRead;
    hr = stream->Read(blob.GetBufferPointer(), static_cast<ULONG>(blob.GetBufferSize()), &bytesRead);
    if (FAILED(hr))
        return hr;

    if (bytesRead != blob.GetBufferSize())
        return E_FAIL;

    return S_OK;
}





_Use_decl_annotations_
HRESULT DirectX::SaveToWICFile(
    const Image& image,
    DWORD flags,
    REFGUID containerFormat,
    const wchar_t* szFile,
    const GUID* targetFormat,
    std::function<void(IPropertyBag2*)> setCustomProps)
{
    if (!szFile)
        return E_INVALIDARG;

    if (!image.pixels)
        return E_POINTER;

    bool iswic2 = false;
    IWICImagingFactory* pWIC = GetWICFactory(iswic2);
    if (!pWIC)
        return E_NOINTERFACE;

    ComPtr<IWICStream> stream;
    HRESULT hr = pWIC->CreateStream(stream.GetAddressOf());
    if (FAILED(hr))
        return hr;

    hr = stream->InitializeFromFilename(szFile, GENERIC_WRITE);
    if (FAILED(hr))
        return hr;

    hr = EncodeSingleFrame(image, flags, containerFormat, stream.Get(), targetFormat, setCustomProps);
    if (FAILED(hr))
    {
        stream.Reset();
        DeleteFileW(szFile);
        return hr;
    }

    return S_OK;
}

_Use_decl_annotations_
HRESULT DirectX::SaveToWICFile(
    const Image* images,
    size_t nimages,
    DWORD flags,
    REFGUID containerFormat,
    const wchar_t* szFile,
    const GUID* targetFormat,
    std::function<void(IPropertyBag2*)> setCustomProps)
{
    if (!szFile || !images || nimages == 0)
        return E_INVALIDARG;

    bool iswic2 = false;
    IWICImagingFactory* pWIC = GetWICFactory(iswic2);
    if (!pWIC)
        return E_NOINTERFACE;

    ComPtr<IWICStream> stream;
    HRESULT hr = pWIC->CreateStream(stream.GetAddressOf());
    if (FAILED(hr))
        return hr;

    hr = stream->InitializeFromFilename(szFile, GENERIC_WRITE);
    if (FAILED(hr))
        return hr;

    if (nimages > 1)
        hr = EncodeMultiframe(images, nimages, flags, containerFormat, stream.Get(), targetFormat, setCustomProps);
    else
        hr = EncodeSingleFrame(images[0], flags, containerFormat, stream.Get(), targetFormat, setCustomProps);

    if (FAILED(hr))
    {
        stream.Reset();
        DeleteFileW(szFile);
        return hr;
    }

    return S_OK;
}
