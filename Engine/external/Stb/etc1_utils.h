













#ifndef __etc1_h__
#define __etc1_h__

#define ETC1_ENCODED_BLOCK_SIZE 8
#define ETC1_DECODED_BLOCK_SIZE 48

#ifndef ETC1_RGB8_OES
#define ETC1_RGB8_OES 0x8D64
#endif

typedef unsigned char etc1_byte;
typedef int etc1_bool;
typedef unsigned int etc1_uint32;

#ifdef __cplusplus
extern "C" {
#endif












void etc1_encode_block(const etc1_byte* pIn, etc1_uint32 validPixelMask, etc1_byte* pOut);









void etc1_decode_block(const etc1_byte* pIn, etc1_byte* pOut);



etc1_uint32 etc1_get_encoded_data_size(etc1_uint32 width, etc1_uint32 height);








int etc1_encode_image(const etc1_byte* pIn, etc1_uint32 width, etc1_uint32 height,
        etc1_uint32 pixelSize, etc1_uint32 stride, etc1_byte* pOut);









int etc1_decode_image(const etc1_byte* pIn, etc1_byte* pOut,
        etc1_uint32 width, etc1_uint32 height,
        etc1_uint32 pixelSize, etc1_uint32 stride);



#define ETC_PKM_HEADER_SIZE 16



void etc1_pkm_format_header(etc1_byte* pHeader, etc1_uint32 width, etc1_uint32 height);



etc1_bool etc1_pkm_is_valid(const etc1_byte* pHeader);



etc1_uint32 etc1_pkm_get_width(const etc1_byte* pHeader);



etc1_uint32 etc1_pkm_get_height(const etc1_byte* pHeader);

#ifdef __cplusplus
}
#endif

#endif
