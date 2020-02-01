

#ifndef HEADER_IMAGE_HELPER
#define HEADER_IMAGE_HELPER

#ifdef __cplusplus
extern "C" {
#endif


int
	up_scale_image
	(
		const unsigned char* const orig,
		int width, int height, int channels,
		unsigned char* resampled,
		int resampled_width, int resampled_height
	);


int
	mipmap_image
	(
		const unsigned char* const orig,
		int width, int height, int channels,
		unsigned char* resampled,
		int block_size_x, int block_size_y
	);


int
	scale_image_RGB_to_NTSC_safe
	(
		unsigned char* orig,
		int width, int height, int channels
	);


int
	convert_RGB_to_YCoCg
	(
		unsigned char* orig,
		int width, int height, int channels
	);


int
	convert_YCoCg_to_RGB
	(
		unsigned char* orig,
		int width, int height, int channels
	);


int
	RGBE_to_RGBdivA
	(
		unsigned char *image,
		int width, int height,
		int rescale_to_max
	);


int
	RGBE_to_RGBdivA2
	(
		unsigned char *image,
		int width, int height,
		int rescale_to_max
	);

#ifdef __cplusplus
}
#endif

#endif 
