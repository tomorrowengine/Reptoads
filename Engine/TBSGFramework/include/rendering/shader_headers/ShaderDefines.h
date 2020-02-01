#ifndef H_SHADERDEFINES
#define H_SHADERDEFINES

#define GET_COLOR(rgb) ((((uint32_t)(255 * rgb.x)) << 0) + (((uint32_t)( 255 * rgb.y)) << 8) + (((uint32_t)(255 * rgb.z)) << 16))


#ifndef __cplusplus
#ifdef __PSSL__
#define M4 column_major matrix
#define SHADER_cbuffer ConstantBuffer
#define SHADER_REGISTER(x)		 : register( x )
#define SHADER_CONSTANT_REGISTER(num)	: register(c##num)
#define SHADER_UAV_REGISTER(num)	: register(t##num)
#define V4 float4
#define V3 float3
#define V2 float2
#define UINT uint
#define StructuredBuffer RegularBuffer
#define StructuredBufferRw RW_RegularBuffer
#define NUM_THREADS NUM_THREADS
#define SEM_TARGET_OUTPUT S_TARGET_OUTPUT
#define SEM_DISPATCH_THREAD_ID S_DISPATCH_THREAD_ID
#define SEM_GROUP_ID S_GROUP_ID
#define SEM_GROUP_THREAD_ID S_GROUP_THREAD_ID
#define SEM_GROUP_INDEX S_GROUP_INDEX
#define SEM_POSITION S_POSITION
#define SEM_TARGET_OUTPUT S_TARGET_OUTPUT
#define SEM_TARGET_OUTPUT_(num) S_TARGET_OUTPUT##num
#else 
#define V4 float4
#define V3 float3
#define V2 float3
#define M4 matrix
#define UINT uint
#define SHADER_cbuffer cbuffer
#define SHADER_REGISTER(x)		 : register( x )

#define SHADER_CONSTANT_REGISTER(num)	: register(b##num)

#define SHADER_UAV_REGISTER(num)	: register(u##num)

#define StructuredBufferRw RWStructuredBuffer
#define thread_group_memory groupshared
#define DataBuffer StructuredBuffer
#define AtomicMin InterlockedMin 
#define AtomicMax InterlockedMax 

#define ThreadGroupMemoryBarrierSync GroupMemoryBarrierWithGroupSync 
#define AtomicOr InterlockedOr
#define RW_Texture2D RWTexture2D
#define NUM_THREADS numthreads
#define SEM_DISPATCH_THREAD_ID SV_DispatchThreadID
#define SEM_GROUP_ID SV_GroupID

#define SEM_GROUP_THREAD_ID SV_GroupThreadID
#define SEM_GROUP_INDEX SV_GroupIndex
#define SEM_POSITION SV_POSITION
#define SEM_TARGET_OUTPUT SV_Target
#define SEM_TARGET_OUTPUT_(num) SV_Target##num
#define SampleLOD SampleLevel
#endif 
#else 
#include <cstdint>
#include <array>
#define UINT uint32_t
#define SHADER_cbuffer struct
#define SHADER_REGISTER(x)
#define uint4 std::array<uint32_t, 4>

#define thread_group_memory
#define SEM_POSITION

#define SHADER_CONSTANT_REGISTER(num)
#define NUM_THREADS NUM_THREADS
#define SEM_GROUP_ID
#define SEM_GROUP_INDEX
#define SEM_THREAD_ID
#include "ShaderStructs.h"
using namespace gfx;
#endif 

#endif 
