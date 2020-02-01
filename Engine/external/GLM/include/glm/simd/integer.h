


#pragma once

#if GLM_ARCH & GLM_ARCH_SSE2_BIT

GLM_FUNC_QUALIFIER glm_uvec4 glm_i128_interleave(glm_uvec4 x)
{
	glm_uvec4 const Mask4 = _mm_set1_epi32(0x0000FFFF);
	glm_uvec4 const Mask3 = _mm_set1_epi32(0x00FF00FF);
	glm_uvec4 const Mask2 = _mm_set1_epi32(0x0F0F0F0F);
	glm_uvec4 const Mask1 = _mm_set1_epi32(0x33333333);
	glm_uvec4 const Mask0 = _mm_set1_epi32(0x55555555);

	glm_uvec4 Reg1;
	glm_uvec4 Reg2;

	
	
	
	Reg1 = x;

	
	
	Reg2 = _mm_slli_si128(Reg1, 2);
	Reg1 = _mm_or_si128(Reg2, Reg1);
	Reg1 = _mm_and_si128(Reg1, Mask4);

	
	
	Reg2 = _mm_slli_si128(Reg1, 1);
	Reg1 = _mm_or_si128(Reg2, Reg1);
	Reg1 = _mm_and_si128(Reg1, Mask3);

	
	
	Reg2 = _mm_slli_epi32(Reg1, 4);
	Reg1 = _mm_or_si128(Reg2, Reg1);
	Reg1 = _mm_and_si128(Reg1, Mask2);

	
	
	Reg2 = _mm_slli_epi32(Reg1, 2);
	Reg1 = _mm_or_si128(Reg2, Reg1);
	Reg1 = _mm_and_si128(Reg1, Mask1);

	
	
	Reg2 = _mm_slli_epi32(Reg1, 1);
	Reg1 = _mm_or_si128(Reg2, Reg1);
	Reg1 = _mm_and_si128(Reg1, Mask0);

	
	Reg2 = _mm_slli_epi32(Reg1, 1);
	Reg2 = _mm_srli_si128(Reg2, 8);
	Reg1 = _mm_or_si128(Reg1, Reg2);

	return Reg1;
}

GLM_FUNC_QUALIFIER glm_uvec4 glm_i128_interleave2(glm_uvec4 x, glm_uvec4 y)
{
	glm_uvec4 const Mask4 = _mm_set1_epi32(0x0000FFFF);
	glm_uvec4 const Mask3 = _mm_set1_epi32(0x00FF00FF);
	glm_uvec4 const Mask2 = _mm_set1_epi32(0x0F0F0F0F);
	glm_uvec4 const Mask1 = _mm_set1_epi32(0x33333333);
	glm_uvec4 const Mask0 = _mm_set1_epi32(0x55555555);

	glm_uvec4 Reg1;
	glm_uvec4 Reg2;

	
	
	Reg1 = _mm_unpacklo_epi64(x, y);

	
	
	Reg2 = _mm_slli_si128(Reg1, 2);
	Reg1 = _mm_or_si128(Reg2, Reg1);
	Reg1 = _mm_and_si128(Reg1, Mask4);

	
	
	Reg2 = _mm_slli_si128(Reg1, 1);
	Reg1 = _mm_or_si128(Reg2, Reg1);
	Reg1 = _mm_and_si128(Reg1, Mask3);

	
	
	Reg2 = _mm_slli_epi32(Reg1, 4);
	Reg1 = _mm_or_si128(Reg2, Reg1);
	Reg1 = _mm_and_si128(Reg1, Mask2);

	
	
	Reg2 = _mm_slli_epi32(Reg1, 2);
	Reg1 = _mm_or_si128(Reg2, Reg1);
	Reg1 = _mm_and_si128(Reg1, Mask1);

	
	
	Reg2 = _mm_slli_epi32(Reg1, 1);
	Reg1 = _mm_or_si128(Reg2, Reg1);
	Reg1 = _mm_and_si128(Reg1, Mask0);

	
	Reg2 = _mm_slli_epi32(Reg1, 1);
	Reg2 = _mm_srli_si128(Reg2, 8);
	Reg1 = _mm_or_si128(Reg1, Reg2);

	return Reg1;
}

#endif
