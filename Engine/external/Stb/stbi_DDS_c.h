



#include "image_DXT.h"

static int stbi__dds_test(stbi__context *s)
{
	
	if (stbi__get8(s) != 'D') {
		stbi__rewind(s);
		return 0;
	}

	if (stbi__get8(s) != 'D') {
		stbi__rewind(s);
		return 0;
	}

	if (stbi__get8(s) != 'S') {
		stbi__rewind(s);
		return 0;
	}

	if (stbi__get8(s) != ' ') {
		stbi__rewind(s);
		return 0;
	}

	
	if (stbi__get32le(s) != 124) {
		stbi__rewind(s);
		return 0;
	}

	
	stbi__rewind(s);

	return 1;
}
#ifndef STBI_NO_STDIO

int      stbi__dds_test_filename        		(char const *filename)
{
   int r;
   FILE *f = fopen(filename, "rb");
   if (!f) return 0;
   r = stbi__dds_test_file(f);
   fclose(f);
   return r;
}

int      stbi__dds_test_file        (FILE *f)
{
   stbi__context s;
   int r,n = ftell(f);
   stbi__start_file(&s,f);
   r = stbi__dds_test(&s);
   fseek(f,n,SEEK_SET);
   return r;
}
#endif

int      stbi__dds_test_memory      (stbi_uc const *buffer, int len)
{
   stbi__context s;
   stbi__start_mem(&s,buffer, len);
   return stbi__dds_test(&s);
}

int      stbi__dds_test_callbacks      (stbi_io_callbacks const *clbk, void *user)
{
   stbi__context s;
   stbi__start_callbacks(&s, (stbi_io_callbacks *) clbk, user);
   return stbi__dds_test(&s);
}


int stbi_convert_bit_range( int c, int from_bits, int to_bits )
{
	int b = (1 << (from_bits - 1)) + c * ((1 << to_bits) - 1);
	return (b + (b >> from_bits)) >> from_bits;
}
void stbi_rgb_888_from_565( unsigned int c, int *r, int *g, int *b )
{
	*r = stbi_convert_bit_range( (c >> 11) & 31, 5, 8 );
	*g = stbi_convert_bit_range( (c >> 05) & 63, 6, 8 );
	*b = stbi_convert_bit_range( (c >> 00) & 31, 5, 8 );
}
void stbi_decode_DXT1_block(
			unsigned char uncompressed[16*4],
			unsigned char compressed[8] )
{
	int next_bit = 4*8;
	int i, r, g, b;
	int c0, c1;
	unsigned char decode_colors[4*4];
	
	c0 = compressed[0] + (compressed[1] << 8);
	c1 = compressed[2] + (compressed[3] << 8);
	stbi_rgb_888_from_565( c0, &r, &g, &b );
	decode_colors[0] = r;
	decode_colors[1] = g;
	decode_colors[2] = b;
	decode_colors[3] = 255;
	stbi_rgb_888_from_565( c1, &r, &g, &b );
	decode_colors[4] = r;
	decode_colors[5] = g;
	decode_colors[6] = b;
	decode_colors[7] = 255;
	if( c0 > c1 )
	{
		
		decode_colors[8] = (2*decode_colors[0] + decode_colors[4]) / 3;
		decode_colors[9] = (2*decode_colors[1] + decode_colors[5]) / 3;
		decode_colors[10] = (2*decode_colors[2] + decode_colors[6]) / 3;
		decode_colors[11] = 255;
		decode_colors[12] = (decode_colors[0] + 2*decode_colors[4]) / 3;
		decode_colors[13] = (decode_colors[1] + 2*decode_colors[5]) / 3;
		decode_colors[14] = (decode_colors[2] + 2*decode_colors[6]) / 3;
		decode_colors[15] = 255;
	} else
	{
		
		decode_colors[8] = (decode_colors[0] + decode_colors[4]) / 2;
		decode_colors[9] = (decode_colors[1] + decode_colors[5]) / 2;
		decode_colors[10] = (decode_colors[2] + decode_colors[6]) / 2;
		decode_colors[11] = 255;
		decode_colors[12] = 0;
		decode_colors[13] = 0;
		decode_colors[14] = 0;
		decode_colors[15] = 0;
	}
	
	for( i = 0; i < 16*4; i += 4 )
	{
		int idx = ((compressed[next_bit>>3] >> (next_bit & 7)) & 3) * 4;
		next_bit += 2;
		uncompressed[i+0] = decode_colors[idx+0];
		uncompressed[i+1] = decode_colors[idx+1];
		uncompressed[i+2] = decode_colors[idx+2];
		uncompressed[i+3] = decode_colors[idx+3];
	}
	
}
void stbi_decode_DXT23_alpha_block(
			unsigned char uncompressed[16*4],
			unsigned char compressed[8] )
{
	int i, next_bit = 0;
	
	for( i = 3; i < 16*4; i += 4 )
	{
		uncompressed[i] = stbi_convert_bit_range(
				(compressed[next_bit>>3] >> (next_bit&7)) & 15,
				4, 8 );
		next_bit += 4;
	}
}
void stbi_decode_DXT45_alpha_block(
			unsigned char uncompressed[16*4],
			unsigned char compressed[8] )
{
	int i, next_bit = 8*2;
	unsigned char decode_alpha[8];
	
	decode_alpha[0] = compressed[0];
	decode_alpha[1] = compressed[1];
	if( decode_alpha[0] > decode_alpha[1] )
	{
		
		decode_alpha[2] = (6*decode_alpha[0] + 1*decode_alpha[1]) / 7;
		decode_alpha[3] = (5*decode_alpha[0] + 2*decode_alpha[1]) / 7;
		decode_alpha[4] = (4*decode_alpha[0] + 3*decode_alpha[1]) / 7;
		decode_alpha[5] = (3*decode_alpha[0] + 4*decode_alpha[1]) / 7;
		decode_alpha[6] = (2*decode_alpha[0] + 5*decode_alpha[1]) / 7;
		decode_alpha[7] = (1*decode_alpha[0] + 6*decode_alpha[1]) / 7;
	} else
	{
		
		decode_alpha[2] = (4*decode_alpha[0] + 1*decode_alpha[1]) / 5;
		decode_alpha[3] = (3*decode_alpha[0] + 2*decode_alpha[1]) / 5;
		decode_alpha[4] = (2*decode_alpha[0] + 3*decode_alpha[1]) / 5;
		decode_alpha[5] = (1*decode_alpha[0] + 4*decode_alpha[1]) / 5;
		decode_alpha[6] = 0;
		decode_alpha[7] = 255;
	}
	for( i = 3; i < 16*4; i += 4 )
	{
		int idx = 0, bit;
		bit = (compressed[next_bit>>3] >> (next_bit&7)) & 1;
		idx += bit << 0;
		++next_bit;
		bit = (compressed[next_bit>>3] >> (next_bit&7)) & 1;
		idx += bit << 1;
		++next_bit;
		bit = (compressed[next_bit>>3] >> (next_bit&7)) & 1;
		idx += bit << 2;
		++next_bit;
		uncompressed[i] = decode_alpha[idx & 7];
	}
	
}
void stbi_decode_DXT_color_block(
			unsigned char uncompressed[16*4],
			unsigned char compressed[8] )
{
	int next_bit = 4*8;
	int i, r, g, b;
	int c0, c1;
	unsigned char decode_colors[4*3];
	
	c0 = compressed[0] + (compressed[1] << 8);
	c1 = compressed[2] + (compressed[3] << 8);
	stbi_rgb_888_from_565( c0, &r, &g, &b );
	decode_colors[0] = r;
	decode_colors[1] = g;
	decode_colors[2] = b;
	stbi_rgb_888_from_565( c1, &r, &g, &b );
	decode_colors[3] = r;
	decode_colors[4] = g;
	decode_colors[5] = b;
	
	
	decode_colors[6] = (2*decode_colors[0] + decode_colors[3]) / 3;
	decode_colors[7] = (2*decode_colors[1] + decode_colors[4]) / 3;
	decode_colors[8] = (2*decode_colors[2] + decode_colors[5]) / 3;
	decode_colors[9] = (decode_colors[0] + 2*decode_colors[3]) / 3;
	decode_colors[10] = (decode_colors[1] + 2*decode_colors[4]) / 3;
	decode_colors[11] = (decode_colors[2] + 2*decode_colors[5]) / 3;
	
	for( i = 0; i < 16*4; i += 4 )
	{
		int idx = ((compressed[next_bit>>3] >> (next_bit & 7)) & 3) * 3;
		next_bit += 2;
		uncompressed[i+0] = decode_colors[idx+0];
		uncompressed[i+1] = decode_colors[idx+1];
		uncompressed[i+2] = decode_colors[idx+2];
	}
	
}

static int stbi__dds_info( stbi__context *s, int *x, int *y, int *comp, int *iscompressed ) {
	int flags,is_compressed,has_alpha;
	DDS_header header={0};

	if( sizeof( DDS_header ) != 128 )
	{
		return 0;
	}

	stbi__getn( s, (stbi_uc*)(&header), 128 );

	if( header.dwMagic != (('D' << 0) | ('D' << 8) | ('S' << 16) | (' ' << 24)) ) {
	   stbi__rewind( s );
	   return 0;
	}
	if( header.dwSize != 124 ) {
	   stbi__rewind( s );
	   return 0;
	}
	flags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	if( (header.dwFlags & flags) != flags ) {
	   stbi__rewind( s );
	   return 0;
	}
	if( header.sPixelFormat.dwSize != 32 ) {
	   stbi__rewind( s );
	   return 0;
	}
	flags = DDPF_FOURCC | DDPF_RGB;
	if( (header.sPixelFormat.dwFlags & flags) == 0 ) {
	   stbi__rewind( s );
	   return 0;
	}
	if( (header.sCaps.dwCaps1 & DDSCAPS_TEXTURE) == 0 ) {
	   stbi__rewind( s );
	   return 0;
	}

	is_compressed = (header.sPixelFormat.dwFlags & DDPF_FOURCC) / DDPF_FOURCC;
	has_alpha = (header.sPixelFormat.dwFlags & DDPF_ALPHAPIXELS) / DDPF_ALPHAPIXELS;

	*x = header.dwWidth;
	*y = header.dwHeight;

	if ( !is_compressed ) {
		*comp = 3;

		if ( has_alpha )
			*comp = 4;
	}
	else
		*comp = 4;

	if ( iscompressed )
		*iscompressed = is_compressed;

	return 1;
}

int stbi__dds_info_from_memory (stbi_uc const *buffer, int len, int *x, int *y, int *comp, int *iscompressed)
{
	stbi__context s;
	stbi__start_mem(&s,buffer, len);
	return stbi__dds_info( &s, x, y, comp, iscompressed );
}

int stbi__dds_info_from_callbacks (stbi_io_callbacks const *clbk, void *user, int *x, int *y, int *comp, int *iscompressed)
{
	stbi__context s;
	stbi__start_callbacks(&s, (stbi_io_callbacks *) clbk, user);
	return stbi__dds_info( &s, x, y, comp, iscompressed );
}

#ifndef STBI_NO_STDIO
int stbi__dds_info_from_path(char const *filename,     int *x, int *y, int *comp, int *iscompressed)
{
   int res;
   FILE *f = fopen(filename, "rb");
   if (!f) return 0;
   res = stbi__dds_info_from_file( f, x, y, comp, iscompressed );
   fclose(f);
   return res;
}

int stbi__dds_info_from_file(FILE *f,                  int *x, int *y, int *comp, int *iscompressed)
{
   stbi__context s;
   int res;
   long n = ftell(f);
   stbi__start_file(&s, f);
   res = stbi__dds_info(&s, x, y, comp, iscompressed);
   fseek(f, n, SEEK_SET);
   return res;
}
#endif

static void * stbi__dds_load(stbi__context *s, int *x, int *y, int *comp, int req_comp)
{
	
	stbi_uc *dds_data = NULL;
	stbi_uc block[16*4];
	stbi_uc compressed[8];
	int flags, DXT_family;
	int has_alpha, has_mipmap;
	int is_compressed, cubemap_faces;
	int block_pitch, num_blocks;
	DDS_header header={0};
	int i, sz, cf;
	
	if( sizeof( DDS_header ) != 128 )
	{
		return NULL;
	}
	stbi__getn( s, (stbi_uc*)(&header), 128 );
	
	if( header.dwMagic != (('D' << 0) | ('D' << 8) | ('S' << 16) | (' ' << 24)) ) return NULL;
	if( header.dwSize != 124 ) return NULL;
	flags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	if( (header.dwFlags & flags) != flags ) return NULL;
	
	if( header.sPixelFormat.dwSize != 32 ) return NULL;
	flags = DDPF_FOURCC | DDPF_RGB;
	if( (header.sPixelFormat.dwFlags & flags) == 0 ) return NULL;
	if( (header.sCaps.dwCaps1 & DDSCAPS_TEXTURE) == 0 ) return NULL;
	
	s->img_x = header.dwWidth;
	s->img_y = header.dwHeight;
	s->img_n = 4;
	is_compressed = (header.sPixelFormat.dwFlags & DDPF_FOURCC) / DDPF_FOURCC;
	has_alpha = (header.sPixelFormat.dwFlags & DDPF_ALPHAPIXELS) / DDPF_ALPHAPIXELS;
	has_mipmap = (header.sCaps.dwCaps1 & DDSCAPS_MIPMAP) && (header.dwMipMapCount > 1);
	cubemap_faces = (header.sCaps.dwCaps2 & DDSCAPS2_CUBEMAP) / DDSCAPS2_CUBEMAP;
	
	cubemap_faces &= (s->img_x == s->img_y);
	cubemap_faces *= 5;
	cubemap_faces += 1;
	block_pitch = (s->img_x+3) >> 2;
	num_blocks = block_pitch * ((s->img_y+3) >> 2);
	
	*x = s->img_x;
	*y = s->img_y;
	*comp = s->img_n;
	
	if( is_compressed )
	{
		
		
		DXT_family = 1 + (header.sPixelFormat.dwFourCC >> 24) - '1';
		if( (DXT_family < 1) || (DXT_family > 5) ) return NULL;
		
		
		sz = (s->img_x)*(s->img_y)*4*cubemap_faces;
		dds_data = (unsigned char*)malloc( sz );
		
		for( cf = 0; cf < cubemap_faces; ++ cf )
		{
			
			for( i = 0; i < num_blocks; ++i )
			{
				
				int bx, by, bw=4, bh=4;
				int ref_x = 4 * (i % block_pitch);
				int ref_y = 4 * (i / block_pitch);
				
				if( DXT_family == 1 )
				{
					
					stbi__getn( s, compressed, 8 );
					stbi_decode_DXT1_block( block, compressed );
				} else if( DXT_family < 4 )
				{
					
					stbi__getn( s, compressed, 8 );
					stbi_decode_DXT23_alpha_block ( block, compressed );
					stbi__getn( s, compressed, 8 );
					stbi_decode_DXT_color_block ( block, compressed );
				} else
				{
					
					stbi__getn( s, compressed, 8 );
					stbi_decode_DXT45_alpha_block ( block, compressed );
					stbi__getn( s, compressed, 8 );
					stbi_decode_DXT_color_block ( block, compressed );
				}
				
				if( ref_x + 4 > (int)s->img_x )
				{
					bw = s->img_x - ref_x;
				}
				if( ref_y + 4 > (int)s->img_y )
				{
					bh = s->img_y - ref_y;
				}
				
				for( by = 0; by < bh; ++by )
				{
					int idx = 4*((ref_y+by+cf*s->img_x)*s->img_x + ref_x);
					for( bx = 0; bx < bw*4; ++bx )
					{

						dds_data[idx+bx] = block[by*16+bx];
					}
				}
			}
			
			if( has_mipmap )
			{
				int block_size = 16;
				if( DXT_family == 1 )
				{
					block_size = 8;
				}
				for( i = 1; i < (int)header.dwMipMapCount; ++i )
				{
					int mx = s->img_x >> (i + 2);
					int my = s->img_y >> (i + 2);
					if( mx < 1 )
					{
						mx = 1;
					}
					if( my < 1 )
					{
						my = 1;
					}
					stbi__skip( s, mx*my*block_size );
				}
			}
		}
	} else
	{
		
		DXT_family = 0;
		s->img_n = 3;
		if( has_alpha )
		{
			s->img_n = 4;
		}
		*comp = s->img_n;
		sz = s->img_x*s->img_y*s->img_n*cubemap_faces;
		dds_data = (unsigned char*)malloc( sz );
		
		for( cf = 0; cf < cubemap_faces; ++ cf )
		{
			
			stbi__getn( s, &dds_data[cf*s->img_x*s->img_y*s->img_n], s->img_x*s->img_y*s->img_n );
			
			if( has_mipmap )
			{
				for( i = 1; i < (int)header.dwMipMapCount; ++i )
				{
					int mx = s->img_x >> i;
					int my = s->img_y >> i;
					if( mx < 1 )
					{
						mx = 1;
					}
					if( my < 1 )
					{
						my = 1;
					}
					stbi__skip( s, mx*my*s->img_n );
				}
			}
		}
		
		for( i = 0; i < sz; i += s->img_n )
		{
			unsigned char temp = dds_data[i];
			dds_data[i] = dds_data[i+2];
			dds_data[i+2] = temp;
		}
	}
	
	s->img_y *= cubemap_faces;
	*y = s->img_y;
	
	
	has_alpha = 0;
	if( s->img_n == 4)
	{
		for( i = 3; (i < sz) && (has_alpha == 0); i += 4 )
		{
			has_alpha |= (dds_data[i] < 255);
		}
	}
	if( (req_comp <= 4) && (req_comp >= 1) )
	{
		
		if( req_comp != s->img_n )
		{
			dds_data = stbi__convert_format( dds_data, s->img_n, req_comp, s->img_x, s->img_y );
			*comp = req_comp;
		}
	} else
	{
		
		if( (has_alpha == 0) && (s->img_n == 4) )
		{
			dds_data = stbi__convert_format( dds_data, 4, 3, s->img_x, s->img_y );
			*comp = 3;
		}
	}
	
	return dds_data;
}

#ifndef STBI_NO_STDIO
void *stbi__dds_load_from_file   (FILE *f,                  int *x, int *y, int *comp, int req_comp)
{
	stbi__context s;
	stbi__start_file(&s,f);
	return stbi__dds_load(&s,x,y,comp,req_comp);
}

void *stbi__dds_load_from_path             (const char *filename,           int *x, int *y, int *comp, int req_comp)
{
   void *data;
   FILE *f = fopen(filename, "rb");
   if (!f) return NULL;
   data = stbi__dds_load_from_file(f,x,y,comp,req_comp);
   fclose(f);
   return data;
}
#endif

void *stbi__dds_load_from_memory (stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp)
{
	stbi__context s;
   stbi__start_mem(&s,buffer, len);
   return stbi__dds_load(&s,x,y,comp,req_comp);
}

void *stbi__dds_load_from_callbacks (stbi_io_callbacks const *clbk, void *user, int *x, int *y, int *comp, int req_comp)
{
	stbi__context s;
   stbi__start_callbacks(&s, (stbi_io_callbacks *) clbk, user);
   return stbi__dds_load(&s,x,y,comp,req_comp);
}

#include "image_DXT_c.h"
