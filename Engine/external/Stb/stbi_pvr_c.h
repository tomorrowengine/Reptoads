#include "pvr_helper.h"

static int stbi__pvr_test(stbi__context *s)
{
	
	if (stbi__get32le(s) != sizeof(PVR_Texture_Header)) {
		stbi__rewind(s);
		return 0;
	}

	
	stbi__skip(s, 10*4);

	
	if ( stbi__get32le(s) != PVRTEX_IDENTIFIER ) {
		stbi__rewind(s);
		return 0;
	}

	
	stbi__rewind(s);

	return 1;
}

#ifndef STBI_NO_STDIO

int      stbi__pvr_test_filename        		(char const *filename)
{
   int r;
   FILE *f = fopen(filename, "rb");
   if (!f) return 0;
   r = stbi__pvr_test_file(f);
   fclose(f);
   return r;
}

int      stbi__pvr_test_file        (FILE *f)
{
   stbi__context s;
   int r,n = ftell(f);
   stbi__start_file(&s,f);
   r = stbi__pvr_test(&s);
   fseek(f,n,SEEK_SET);
   return r;
}
#endif

int      stbi__pvr_test_memory      (stbi_uc const *buffer, int len)
{
   stbi__context s;
   stbi__start_mem(&s,buffer, len);
   return stbi__pvr_test(&s);
}

int      stbi__pvr_test_callbacks      (stbi_io_callbacks const *clbk, void *user)
{
   stbi__context s;
   stbi__start_callbacks(&s, (stbi_io_callbacks *) clbk, user);
   return stbi__pvr_test(&s);
}

static int stbi__pvr_info(stbi__context *s, int *x, int *y, int *comp, int * iscompressed )
{
	PVR_Texture_Header header={0};

	stbi__getn( s, (stbi_uc*)(&header), sizeof(PVR_Texture_Header) );

	
	if ( header.dwHeaderSize != sizeof(PVR_Texture_Header) ) {
		stbi__rewind( s );
		return 0;
	}

	
	if ( header.dwPVR != PVRTEX_IDENTIFIER ) {
		stbi__rewind(s);
		return 0;
	}

	*x = s->img_x = header.dwWidth;
	*y = s->img_y = header.dwHeight;
	*comp = s->img_n = ( header.dwBitCount + 7 ) / 8;

	if ( iscompressed )
		*iscompressed = 0;

	switch ( header.dwpfFlags & PVRTEX_PIXELTYPE )
	{
		case OGL_RGBA_4444:
			s->img_n = 2;
			break;
		case OGL_RGBA_5551:
			s->img_n = 2;
			break;
		case OGL_RGBA_8888:
			s->img_n = 4;
			break;
		case OGL_RGB_565:
			s->img_n = 2;
			break;
		case OGL_RGB_888:
			s->img_n = 3;
			break;
		case OGL_I_8:
			s->img_n = 1;
			break;
		case OGL_AI_88:
			s->img_n = 2;
			break;
		case OGL_PVRTC2:
			s->img_n = 4;
			if ( iscompressed )
				*iscompressed = 1;
			break;
		case OGL_PVRTC4:
			s->img_n = 4;
			if ( iscompressed )
				*iscompressed = 1;
			break;
		case OGL_RGB_555:
		default:
			stbi__rewind(s);
			return 0;
	}

	*comp = s->img_n;

	return 1;
}

int stbi__pvr_info_from_memory (stbi_uc const *buffer, int len, int *x, int *y, int *comp, int * iscompressed )
{
	stbi__context s;
	stbi__start_mem(&s,buffer, len);
	return stbi__pvr_info( &s, x, y, comp, iscompressed );
}

int stbi__pvr_info_from_callbacks (stbi_io_callbacks const *clbk, void *user, int *x, int *y, int *comp, int * iscompressed)
{
	stbi__context s;
	stbi__start_callbacks(&s, (stbi_io_callbacks *) clbk, user);
	return stbi__pvr_info( &s, x, y, comp, iscompressed );
}

#ifndef STBI_NO_STDIO
int stbi__pvr_info_from_path(char const *filename,     int *x, int *y, int *comp, int * iscompressed)
{
   int res;
   FILE *f = fopen(filename, "rb");
   if (!f) return 0;
   res = stbi__pvr_info_from_file( f, x, y, comp, iscompressed );
   fclose(f);
   return res;
}

int stbi__pvr_info_from_file(FILE *f,                  int *x, int *y, int *comp, int * iscompressed)
{
   stbi__context s;
   int res;
   long n = ftell(f);
   stbi__start_file(&s, f);
   res = stbi__pvr_info(&s, x, y, comp, iscompressed);
   fseek(f, n, SEEK_SET);
   return res;
}
#endif



typedef unsigned char      PVRTuint8;
typedef unsigned short     PVRTuint16;
typedef unsigned int       PVRTuint32;


#define PT_INDEX (2)	

#define BLK_Y_SIZE 	(4) 

#define BLK_X_MAX	(8)	

#define BLK_X_2BPP	(8) 
#define BLK_X_4BPP	(4)

#define WRAP_COORD(Val, Size) ((Val) & ((Size)-1))

#define POWER_OF_2(X)   util_number_is_power_2(X)


#define PVRT_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#define PVRT_MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define PVRT_CLAMP(x, l, h)      (PVRT_MIN((h), PVRT_MAX((x), (l))))

#define LIMIT_COORD(Val, Size, AssumeImageTiles) \
	  ((AssumeImageTiles)? WRAP_COORD((Val), (Size)): PVRT_CLAMP((Val), 0, (Size)-1))


typedef PVRTuint32 U32;
typedef PVRTuint8 U8;




typedef struct
{
	
	U32 PackedData[2];
}AMTC_BLOCK_STRUCT;

 
int util_number_is_power_2( unsigned  input )
{
  unsigned minus1;

  if( !input ) return 0;

  minus1 = input - 1;
  return ( (input | minus1) == (input ^ minus1) ) ? 1 : 0;
}


static void Unpack5554Colour(const AMTC_BLOCK_STRUCT *pBlock,
							 int   ABColours[2][4])
{
	U32 RawBits[2];

	int i;

	
	RawBits[0] = pBlock->PackedData[1] & (0xFFFE); 
	RawBits[1] = pBlock->PackedData[1] >> 16;	   

	
	for(i = 0; i < 2; i++)
	{
		
		if(RawBits[i] & (1<<15))
		{
			
			ABColours[i][0] = (RawBits[i] >> 10) & 0x1F;
			ABColours[i][1] = (RawBits[i] >>  5) & 0x1F;

			
			ABColours[i][2] = RawBits[i] & 0x1F;
			if(i==0)
			{
				ABColours[0][2] |= ABColours[0][2] >> 4;
			}

			
			ABColours[i][3] = 0xF;
		}
		else 
		{
			
			ABColours[i][0] = (RawBits[i] >>  (8-1)) & 0x1E;
			ABColours[i][1] = (RawBits[i] >>  (4-1)) & 0x1E;

			
			ABColours[i][0] |= ABColours[i][0] >> 4;
			ABColours[i][1] |= ABColours[i][1] >> 4;

			
			ABColours[i][2] = (RawBits[i] & 0xF) << 1;

			
			if(i==0)
			{
				ABColours[0][2] |= ABColours[0][2] >> 3;
			}
			else
			{
				ABColours[0][2] |= ABColours[0][2] >> 4;
			}

			
			ABColours[i][3] = (RawBits[i] >> 11) & 0xE;
		}
	}
}


static void	UnpackModulations(const AMTC_BLOCK_STRUCT *pBlock,
							  const int Do2bitMode,
							  int ModulationVals[8][16],
							  int ModulationModes[8][16],
							  int StartX,
							  int StartY)
{
	int BlockModMode;
	U32 ModulationBits;

	int x, y;

	BlockModMode= pBlock->PackedData[1] & 1;
	ModulationBits	= pBlock->PackedData[0];

	
	if(Do2bitMode && BlockModMode)
	{
		
		for(y = 0; y < BLK_Y_SIZE; y++)
		{
			for(x = 0; x < BLK_X_2BPP; x++)
			{
				ModulationModes[y+StartY][x+StartX] = BlockModMode;

				
				if(((x^y)&1) == 0)
				{
					ModulationVals[y+StartY][x+StartX] = ModulationBits & 3;
					ModulationBits >>= 2;
				}
			}
		}
	}
	else if(Do2bitMode) 
	{
		for(y = 0; y < BLK_Y_SIZE; y++)
		{
			for(x = 0; x < BLK_X_2BPP; x++)
			{
				ModulationModes[y+StartY][x+StartX] = BlockModMode;

				
				if(ModulationBits & 1)
				{
					ModulationVals[y+StartY][x+StartX] = 0x3;
				}
				else
				{
					ModulationVals[y+StartY][x+StartX] = 0x0;
				}
				ModulationBits >>= 1;
			}
		}
	}
	else 
	{
		for(y = 0; y < BLK_Y_SIZE; y++)
		{
			for(x = 0; x < BLK_X_4BPP; x++)
			{
				ModulationModes[y+StartY][x+StartX] = BlockModMode;

				ModulationVals[y+StartY][x+StartX] = ModulationBits & 3;
				ModulationBits >>= 2;
			}
		}
	}

	
	assert(ModulationBits==0);
}


static void InterpolateColours(const int ColourP[4],
						  const int ColourQ[4],
						  const int ColourR[4],
						  const int ColourS[4],
						  const int Do2bitMode,
						  const int x,
						  const int y,
						  int Result[4])
{
	int u, v, uscale;
	int k;

	int tmp1, tmp2;

	int P[4], Q[4], R[4], S[4];

	
	for(k = 0; k < 4; k++)
	{
		P[k] = ColourP[k];
		Q[k] = ColourQ[k];
		R[k] = ColourR[k];
		S[k] = ColourS[k];
	}

	
	v = (y & 0x3) | ((~y & 0x2) << 1);

	if(Do2bitMode)
		u = (x & 0x7) | ((~x & 0x4) << 1);
	else
		u = (x & 0x3) | ((~x & 0x2) << 1);

	
	v  = v - BLK_Y_SIZE/2;

	if(Do2bitMode)
	{
		u = u - BLK_X_2BPP/2;
		uscale = 8;
	}
	else
	{
		u = u - BLK_X_4BPP/2;
		uscale = 4;
	}

	for(k = 0; k < 4; k++)
	{
		tmp1 = P[k] * uscale + u * (Q[k] - P[k]);
		tmp2 = R[k] * uscale + u * (S[k] - R[k]);

		tmp1 = tmp1 * 4 + v * (tmp2 - tmp1);

		Result[k] = tmp1;
	}

	
	if(Do2bitMode)
	{
		
		for(k = 0; k < 3; k++)
		{
			Result[k] >>= 2;
		}

		Result[3] >>= 1;
	}
	else
	{
		
		for(k = 0; k < 3; k++)
		{
			Result[k] >>= 1;
		}
	}

	
	for(k = 0; k < 4; k++)
	{
		assert(Result[k] < 256);
	}


	
	for(k = 0; k < 3; k++)
	{
		Result[k] += Result[k] >> 5;
	}

	Result[3] += Result[3] >> 4;

	
	for(k = 0; k < 4; k++)
	{
		assert(Result[k] < 256);
	}

}


static void GetModulationValue(int x,
							   int y,
							   const int Do2bitMode,
							   const int ModulationVals[8][16],
							   const int ModulationModes[8][16],
							   int *Mod,
							   int *DoPT)
{
	static const int RepVals0[4] = {0, 3, 5, 8};
	static const int RepVals1[4] = {0, 4, 4, 8};

	int ModVal;

	
	y = (y & 0x3) | ((~y & 0x2) << 1);

	if(Do2bitMode)
		x = (x & 0x7) | ((~x & 0x4) << 1);
	else
		x = (x & 0x3) | ((~x & 0x2) << 1);

	
	*DoPT = 0;

	
	if(ModulationModes[y][x]==0)
	{
		ModVal = RepVals0[ModulationVals[y][x]];
	}
	else if(Do2bitMode)
	{
		
		if(((x^y)&1)==0)
			ModVal = RepVals0[ModulationVals[y][x]];
		else if(ModulationModes[y][x] == 1) 
		{
			ModVal = (RepVals0[ModulationVals[y-1][x]] +
					  RepVals0[ModulationVals[y+1][x]] +
					  RepVals0[ModulationVals[y][x-1]] +
					  RepVals0[ModulationVals[y][x+1]] + 2) / 4;
		}
		else if(ModulationModes[y][x] == 2) 
		{
			ModVal = (RepVals0[ModulationVals[y][x-1]] +
					  RepVals0[ModulationVals[y][x+1]] + 1) / 2;
		}
		else 
		{
			ModVal = (RepVals0[ModulationVals[y-1][x]] +
					  RepVals0[ModulationVals[y+1][x]] + 1) / 2;
		}
	}
	else 
	{
		ModVal = RepVals1[ModulationVals[y][x]];

		*DoPT = ModulationVals[y][x] == PT_INDEX;
	}

	*Mod =ModVal;
}


static int DisableTwiddlingRoutine = 0;

static U32 TwiddleUV(U32 YSize, U32 XSize, U32 YPos, U32 XPos)
{
	U32 Twiddled;

	U32 MinDimension;
	U32 MaxValue;

	U32 SrcBitPos;
	U32 DstBitPos;

	int ShiftCount;

	assert(YPos < YSize);
	assert(XPos < XSize);

	assert(POWER_OF_2(YSize));
	assert(POWER_OF_2(XSize));

	if(YSize < XSize)
	{
		MinDimension = YSize;
		MaxValue	 = XPos;
	}
	else
	{
		MinDimension = XSize;
		MaxValue	 = YPos;
	}

	
	if(DisableTwiddlingRoutine)
		return (YPos* XSize + XPos);

	
	SrcBitPos = 1;
	DstBitPos = 1;
	Twiddled  = 0;
	ShiftCount = 0;

	while(SrcBitPos < MinDimension)
	{
		if(YPos & SrcBitPos)
		{
			Twiddled |= DstBitPos;
		}

		if(XPos & SrcBitPos)
		{
			Twiddled |= (DstBitPos << 1);
		}


		SrcBitPos <<= 1;
		DstBitPos <<= 2;
		ShiftCount += 1;

	}

	
	MaxValue >>= ShiftCount;

	Twiddled |=  (MaxValue << (2*ShiftCount));

	return Twiddled;
}





static void Decompress(AMTC_BLOCK_STRUCT *pCompressedData,
					   const int Do2bitMode,
					   const int XDim,
					   const int YDim,
					   const int AssumeImageTiles,
					   unsigned char* pResultImage)
{
	int x, y;
	int i, j;

	int BlkX, BlkY;
	int BlkXp1, BlkYp1;
	int XBlockSize;
	int BlkXDim, BlkYDim;

	int StartX, StartY;

	int ModulationVals[8][16];
	int ModulationModes[8][16];

	int Mod, DoPT;

	unsigned int uPosition;

	
	AMTC_BLOCK_STRUCT *pBlocks[2][2];

	AMTC_BLOCK_STRUCT *pPrevious[2][2] = {{NULL, NULL}, {NULL, NULL}};

	
	struct
	{
		int Reps[2][4];
	}Colours5554[2][2];

	
	int ASig[4], BSig[4];

	int Result[4];

	if(Do2bitMode)
	{
		XBlockSize = BLK_X_2BPP;
	}
	else
	{
		XBlockSize = BLK_X_4BPP;
	}


	
	BlkXDim = PVRT_MAX(2, XDim / XBlockSize);
	BlkYDim = PVRT_MAX(2, YDim / BLK_Y_SIZE);

	
	for(y = 0; y < YDim; y++)
	{
		for(x = 0; x < XDim; x++)
		{
			
			BlkX = (x - XBlockSize/2);
			BlkY = (y - BLK_Y_SIZE/2);

			BlkX = LIMIT_COORD(BlkX, XDim, AssumeImageTiles);
			BlkY = LIMIT_COORD(BlkY, YDim, AssumeImageTiles);


			BlkX /= XBlockSize;
			BlkY /= BLK_Y_SIZE;

			
			


			
			BlkXp1 = LIMIT_COORD(BlkX+1, BlkXDim, AssumeImageTiles);
			BlkYp1 = LIMIT_COORD(BlkY+1, BlkYDim, AssumeImageTiles);

			
			pBlocks[0][0] = pCompressedData +TwiddleUV(BlkYDim, BlkXDim, BlkY, BlkX);
			pBlocks[0][1] = pCompressedData +TwiddleUV(BlkYDim, BlkXDim, BlkY, BlkXp1);
			pBlocks[1][0] = pCompressedData +TwiddleUV(BlkYDim, BlkXDim, BlkYp1, BlkX);
			pBlocks[1][1] = pCompressedData +TwiddleUV(BlkYDim, BlkXDim, BlkYp1, BlkXp1);


			
			if(memcmp(pPrevious, pBlocks, 4*sizeof(void*)) != 0)
			{
				StartY = 0;
				for(i = 0; i < 2; i++)
				{
					StartX = 0;
					for(j = 0; j < 2; j++)
					{
						Unpack5554Colour(pBlocks[i][j], Colours5554[i][j].Reps);

						UnpackModulations(pBlocks[i][j],
							Do2bitMode,
							ModulationVals,
							ModulationModes,
							StartX, StartY);

						StartX += XBlockSize;
					}

					StartY += BLK_Y_SIZE;
				}

				
				memcpy(pPrevious, pBlocks, 4*sizeof(void*));
			}


			
			InterpolateColours(Colours5554[0][0].Reps[0],
				Colours5554[0][1].Reps[0],
				Colours5554[1][0].Reps[0],
				Colours5554[1][1].Reps[0],
				Do2bitMode, x, y,
				ASig);

			InterpolateColours(Colours5554[0][0].Reps[1],
				Colours5554[0][1].Reps[1],
				Colours5554[1][0].Reps[1],
				Colours5554[1][1].Reps[1],
				Do2bitMode, x, y,
				BSig);

			GetModulationValue(x,y, Do2bitMode, (const int (*)[16])ModulationVals, (const int (*)[16])ModulationModes,
				&Mod, &DoPT);

			
			for(i = 0; i < 4; i++)
			{
				Result[i] = ASig[i] * 8 + Mod * (BSig[i] - ASig[i]);
				Result[i] >>= 3;
			}
			if(DoPT)
			{
				Result[3] = 0;
			}

			
			uPosition = (x+y*XDim)<<2;
			pResultImage[uPosition+0] = (unsigned char)Result[0];
			pResultImage[uPosition+1] = (unsigned char)Result[1];
			pResultImage[uPosition+2] = (unsigned char)Result[2];
			pResultImage[uPosition+3] = (unsigned char)Result[3];

		}
	}

}

static void * stbi__pvr_load(stbi__context *s, int *x, int *y, int *comp, int req_comp)
{
	stbi_uc *pvr_data = NULL;
	stbi_uc *pvr_res_data = NULL;
	PVR_Texture_Header header={0};
	int iscompressed = 0;
	int bitmode = 0;
	unsigned int levelSize = 0;

	stbi__getn( s, (stbi_uc*)(&header), sizeof(PVR_Texture_Header) );

	
	if ( header.dwHeaderSize != sizeof(PVR_Texture_Header) ) {
		return NULL;
	}

	
	if ( header.dwPVR != PVRTEX_IDENTIFIER ) {
		return NULL;
	}

	*x = s->img_x = header.dwWidth;
	*y = s->img_y = header.dwHeight;

	
	switch ( header.dwpfFlags & PVRTEX_PIXELTYPE )
	{
		case OGL_RGBA_4444:
			s->img_n = 2;
			break;
		case OGL_RGBA_5551:
			s->img_n = 2;
			break;
		case OGL_RGBA_8888:
			s->img_n = 4;
			break;
		case OGL_RGB_565:
			s->img_n = 2;
			break;
		case OGL_RGB_888:
			s->img_n = 3;
			break;
		case OGL_I_8:
			s->img_n = 1;
			break;
		case OGL_AI_88:
			s->img_n = 2;
			break;
		case OGL_PVRTC2:
			bitmode = 1;
			s->img_n = 4;
			iscompressed = 1;
			break;
		case OGL_PVRTC4:
			s->img_n = 4;
			iscompressed = 1;
			break;
		case OGL_RGB_555:
		default:
			return NULL;
	}

	*comp = s->img_n;

	
	levelSize = (s->img_x * s->img_y * header.dwBitCount + 7) / 8;

	
	pvr_data = (stbi_uc *)malloc( levelSize );
	stbi__getn( s, pvr_data, levelSize );

	
	if ( iscompressed ) {
		pvr_res_data = (stbi_uc *)malloc( s->img_x * s->img_y * 4 );
		Decompress( (AMTC_BLOCK_STRUCT*)pvr_data, bitmode, s->img_x, s->img_y, 1, (unsigned char*)pvr_res_data );
		free( pvr_data );
	} else {
		
		pvr_res_data = pvr_data;
	}

	if( (req_comp <= 4) && (req_comp >= 1) ) {
		
		if( req_comp != s->img_n ) {
			pvr_res_data = stbi__convert_format( pvr_res_data, s->img_n, req_comp, s->img_x, s->img_y );
			*comp = req_comp;
		}
	}

	return pvr_res_data;
}

#ifndef STBI_NO_STDIO
void *stbi__pvr_load_from_file   (FILE *f,                  int *x, int *y, int *comp, int req_comp)
{
	stbi__context s;
	stbi__start_file(&s,f);
	return stbi__pvr_load(&s,x,y,comp,req_comp);
}

void *stbi__pvr_load_from_path             (char const*filename,           int *x, int *y, int *comp, int req_comp)
{
   void *data;
   FILE *f = fopen(filename, "rb");
   if (!f) return NULL;
   data = stbi__pvr_load_from_file(f,x,y,comp,req_comp);
   fclose(f);
   return data;
}
#endif

void *stbi__pvr_load_from_memory (stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp)
{
   stbi__context s;
   stbi__start_mem(&s,buffer, len);
   return stbi__pvr_load(&s,x,y,comp,req_comp);
}

void *stbi__pvr_load_from_callbacks (stbi_io_callbacks const *clbk, void *user, int *x, int *y, int *comp, int req_comp)
{
	stbi__context s;
   stbi__start_callbacks(&s, (stbi_io_callbacks *) clbk, user);
   return stbi__pvr_load(&s,x,y,comp,req_comp);
}
