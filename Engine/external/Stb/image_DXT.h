

#ifndef HEADER_IMAGE_DXT
#define HEADER_IMAGE_DXT


int
save_image_as_DDS
(
    const char *filename,
    int width, int height, int channels,
    const unsigned char *const data
);


unsigned char*
convert_image_to_DXT1
(
    const unsigned char *const uncompressed,
    int width, int height, int channels,
    int *out_size
);


unsigned char*
convert_image_to_DXT5
(
    const unsigned char *const uncompressed,
    int width, int height, int channels,
    int *out_size
);


typedef struct
{
    unsigned int    dwMagic;
    unsigned int    dwSize;
    unsigned int    dwFlags;
    unsigned int    dwHeight;
    unsigned int    dwWidth;
    unsigned int    dwPitchOrLinearSize;
    unsigned int    dwDepth;
    unsigned int    dwMipMapCount;
    unsigned int    dwReserved1[ 11 ];

    
    struct
    {
        unsigned int    dwSize;
        unsigned int    dwFlags;
        unsigned int    dwFourCC;
        unsigned int    dwRGBBitCount;
        unsigned int    dwRBitMask;
        unsigned int    dwGBitMask;
        unsigned int    dwBBitMask;
        unsigned int    dwAlphaBitMask;
    }
    sPixelFormat;

    
    struct
    {
        unsigned int    dwCaps1;
        unsigned int    dwCaps2;
        unsigned int    dwDDSX;
        unsigned int    dwReserved;
    }
    sCaps;
    unsigned int    dwReserved2;
}
DDS_header ;




#define DDSD_CAPS	0x00000001
#define DDSD_HEIGHT	0x00000002
#define DDSD_WIDTH	0x00000004
#define DDSD_PITCH	0x00000008
#define DDSD_PIXELFORMAT	0x00001000
#define DDSD_MIPMAPCOUNT	0x00020000
#define DDSD_LINEARSIZE	0x00080000
#define DDSD_DEPTH	0x00800000


#define DDPF_ALPHAPIXELS	0x00000001
#define DDPF_FOURCC	0x00000004
#define DDPF_RGB	0x00000040


#define DDSCAPS_COMPLEX	0x00000008
#define DDSCAPS_TEXTURE	0x00001000
#define DDSCAPS_MIPMAP	0x00400000


#define DDSCAPS2_CUBEMAP	0x00000200
#define DDSCAPS2_CUBEMAP_POSITIVEX	0x00000400
#define DDSCAPS2_CUBEMAP_NEGATIVEX	0x00000800
#define DDSCAPS2_CUBEMAP_POSITIVEY	0x00001000
#define DDSCAPS2_CUBEMAP_NEGATIVEY	0x00002000
#define DDSCAPS2_CUBEMAP_POSITIVEZ	0x00004000
#define DDSCAPS2_CUBEMAP_NEGATIVEZ	0x00008000
#define DDSCAPS2_VOLUME	0x00200000

#endif 
