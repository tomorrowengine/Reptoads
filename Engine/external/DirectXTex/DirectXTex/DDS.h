

















#pragma once


namespace DirectX
{

#pragma pack(push,1)

const uint32_t DDS_MAGIC = 0x20534444; 

struct DDS_PIXELFORMAT
{
    uint32_t    size;
    uint32_t    flags;
    uint32_t    fourCC;
    uint32_t    RGBBitCount;
    uint32_t    RBitMask;
    uint32_t    GBitMask;
    uint32_t    BBitMask;
    uint32_t    ABitMask;
};

#define DDS_FOURCC      0x00000004  
#define DDS_RGB         0x00000040  
#define DDS_RGBA        0x00000041  
#define DDS_LUMINANCE   0x00020000  
#define DDS_LUMINANCEA  0x00020001  
#define DDS_ALPHAPIXELS 0x00000001  
#define DDS_ALPHA       0x00000002  
#define DDS_PAL8        0x00000020  
#define DDS_PAL8A       0x00000021  
#define DDS_BUMPDUDV    0x00080000  

#ifndef MAKEFOURCC
    #define MAKEFOURCC(ch0, ch1, ch2, ch3) \
                (static_cast<uint32_t>(static_cast<uint8_t>(ch0)) \
                | (static_cast<uint32_t>(static_cast<uint8_t>(ch1)) << 8) \
                | (static_cast<uint32_t>(static_cast<uint8_t>(ch2)) << 16) \
                | (static_cast<uint32_t>(static_cast<uint8_t>(ch3)) << 24))
#endif 

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_DXT1 =
    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('D','X','T','1'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_DXT2 =
    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('D','X','T','2'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_DXT3 =
    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('D','X','T','3'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_DXT4 =
    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('D','X','T','4'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_DXT5 =
    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('D','X','T','5'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_BC4_UNORM =
    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('B','C','4','U'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_BC4_SNORM =
    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('B','C','4','S'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_BC5_UNORM =
    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('B','C','5','U'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_BC5_SNORM =
    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('B','C','5','S'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_R8G8_B8G8 =
    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('R','G','B','G'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_G8R8_G8B8 =
    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('G','R','G','B'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_YUY2 =
    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('Y','U','Y','2'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_A8R8G8B8 =
    { sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_X8R8G8B8 =
    { sizeof(DDS_PIXELFORMAT), DDS_RGB,  0, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_A8B8G8R8 =
    { sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_X8B8G8R8 =
    { sizeof(DDS_PIXELFORMAT), DDS_RGB,  0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_G16R16 =
    { sizeof(DDS_PIXELFORMAT), DDS_RGB,  0, 32, 0x0000ffff, 0xffff0000, 0x00000000, 0x00000000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_R5G6B5 =
    { sizeof(DDS_PIXELFORMAT), DDS_RGB, 0, 16, 0x0000f800, 0x000007e0, 0x0000001f, 0x00000000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_A1R5G5B5 =
    { sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 16, 0x00007c00, 0x000003e0, 0x0000001f, 0x00008000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_A4R4G4B4 =
    { sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 16, 0x00000f00, 0x000000f0, 0x0000000f, 0x0000f000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_R8G8B8 =
    { sizeof(DDS_PIXELFORMAT), DDS_RGB, 0, 24, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_L8 =
    { sizeof(DDS_PIXELFORMAT), DDS_LUMINANCE, 0,  8, 0xff, 0x00, 0x00, 0x00 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_L16 =
    { sizeof(DDS_PIXELFORMAT), DDS_LUMINANCE, 0, 16, 0xffff, 0x0000, 0x0000, 0x0000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_A8L8 =
    { sizeof(DDS_PIXELFORMAT), DDS_LUMINANCEA, 0, 16, 0x00ff, 0x0000, 0x0000, 0xff00 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_A8L8_ALT =
    { sizeof(DDS_PIXELFORMAT), DDS_LUMINANCEA, 0, 8, 0x00ff, 0x0000, 0x0000, 0xff00 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_A8 =
    { sizeof(DDS_PIXELFORMAT), DDS_ALPHA, 0, 8, 0x00, 0x00, 0x00, 0xff };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_V8U8 = 
    { sizeof(DDS_PIXELFORMAT), DDS_BUMPDUDV, 0, 16, 0x00ff, 0xff00, 0x0000, 0x0000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_Q8W8V8U8 = 
    { sizeof(DDS_PIXELFORMAT), DDS_BUMPDUDV, 0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_V16U16 = 
    { sizeof(DDS_PIXELFORMAT), DDS_BUMPDUDV, 0, 32, 0x0000ffff, 0xffff0000, 0x00000000, 0x00000000 };




extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_DX10 =
    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('D','X','1','0'), 0, 0, 0, 0, 0 };

#define DDS_HEADER_FLAGS_TEXTURE        0x00001007  
#define DDS_HEADER_FLAGS_MIPMAP         0x00020000  
#define DDS_HEADER_FLAGS_VOLUME         0x00800000  
#define DDS_HEADER_FLAGS_PITCH          0x00000008  
#define DDS_HEADER_FLAGS_LINEARSIZE     0x00080000  

#define DDS_HEIGHT 0x00000002 
#define DDS_WIDTH  0x00000004 

#define DDS_SURFACE_FLAGS_TEXTURE 0x00001000 
#define DDS_SURFACE_FLAGS_MIPMAP  0x00400008 
#define DDS_SURFACE_FLAGS_CUBEMAP 0x00000008 

#define DDS_CUBEMAP_POSITIVEX 0x00000600 
#define DDS_CUBEMAP_NEGATIVEX 0x00000a00 
#define DDS_CUBEMAP_POSITIVEY 0x00001200 
#define DDS_CUBEMAP_NEGATIVEY 0x00002200 
#define DDS_CUBEMAP_POSITIVEZ 0x00004200 
#define DDS_CUBEMAP_NEGATIVEZ 0x00008200 

#define DDS_CUBEMAP_ALLFACES ( DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX |\
                               DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY |\
                               DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ )

#define DDS_CUBEMAP 0x00000200 

#define DDS_FLAGS_VOLUME 0x00200000 


enum DDS_RESOURCE_DIMENSION
{
    DDS_DIMENSION_TEXTURE1D	= 2,
    DDS_DIMENSION_TEXTURE2D	= 3,
    DDS_DIMENSION_TEXTURE3D	= 4,
};


enum DDS_RESOURCE_MISC_FLAG
{
    DDS_RESOURCE_MISC_TEXTURECUBE = 0x4L,
};

enum DDS_MISC_FLAGS2
{
    DDS_MISC_FLAGS2_ALPHA_MODE_MASK = 0x7L,
};

enum DDS_ALPHA_MODE
{
    DDS_ALPHA_MODE_UNKNOWN = 0,
    DDS_ALPHA_MODE_STRAIGHT = 1,
    DDS_ALPHA_MODE_PREMULTIPLIED = 2,
    DDS_ALPHA_MODE_OPAQUE = 3,
    DDS_ALPHA_MODE_CUSTOM = 4,
};

struct DDS_HEADER
{
    uint32_t        size;
    uint32_t        flags;
    uint32_t        height;
    uint32_t        width;
    uint32_t        pitchOrLinearSize;
    uint32_t        depth; 
    uint32_t        mipMapCount;
    uint32_t        reserved1[11];
    DDS_PIXELFORMAT ddspf;
    uint32_t        caps;
    uint32_t        caps2;
    uint32_t        caps3;
    uint32_t        caps4;
    uint32_t        reserved2;
};

struct DDS_HEADER_DXT10
{
    DXGI_FORMAT     dxgiFormat;
    uint32_t        resourceDimension;
    uint32_t        miscFlag; 
    uint32_t        arraySize;
    uint32_t        miscFlags2; 
};

#pragma pack(pop)

static_assert( sizeof(DDS_HEADER) == 124, "DDS Header size mismatch" );
static_assert( sizeof(DDS_HEADER_DXT10) == 20, "DDS DX10 Extended Header size mismatch");

} 
