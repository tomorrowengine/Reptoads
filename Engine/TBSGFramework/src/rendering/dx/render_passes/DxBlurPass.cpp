#ifdef PLATFORM_WINDOWS
#include "rendering/dx/render_passes/DxBlurPass.h"
#include "rendering/dx/DX12Application.h"
#include "rendering/dx/DX12Renderer.h"
#include "core/Defines.h"
#include "core/Config.h"
#include "rendering/ResourceIds.h"
#include "rendering/dx/DX12Shader.h"
#include "rendering/dx/CommandQueue.h"
#include "rendering/dx/CommandList.h"
#include "rendering/dx/Helpers.h"
#include "BlurShaderBuffers.h"
#include "rendering/dx/render_passes/DxMipmapPass.h"
#include "rendering/dx/ResourceStateTracker.h"


namespace gfx
{
	enum class BlurRootParameters
	{
		ConstantBlurData,
		Textures,
		NumParams
	};

	const int blurInputElementAmount = 2;
	const D3D12_INPUT_ELEMENT_DESC blurInputElements[blurInputElementAmount] =
	{
		{ "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};


	DxBlurPass::DxBlurPass(DX12Renderer* renderer, DxMipmapPass& mipmapPass)
		: renderer(renderer), mipmapPass(&mipmapPass)
	{
		auto& rm = renderer->GetGfxResourceManager();

		static ShaderId blurComputeShader = rm.CreateShader(tbsg::Config::Get().MakeShaderPath("Blur_c.cso", 1));

		DX12Shader* dxBlurComputeShader = static_cast<DX12Shader*>(&rm.GetShader(blurComputeShader));
		
		auto device = DX12Application::Get().GetDevice();


		
		D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
		if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData)))) {
			featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
		}

		
		D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

		const int amountOfDesctiptorRanges = 2;
		D3D12_DESCRIPTOR_RANGE1 blurDescriptorRanges[amountOfDesctiptorRanges]{
			CD3DX12_DESCRIPTOR_RANGE1(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0),
			CD3DX12_DESCRIPTOR_RANGE1(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0)
		};

		CD3DX12_ROOT_PARAMETER1 rootParameters[static_cast<UINT>(BlurRootParameters::NumParams)];
		rootParameters[static_cast<UINT>(BlurRootParameters::ConstantBlurData)].InitAsConstantBufferView(0, 0);
		rootParameters[static_cast<UINT>(BlurRootParameters::Textures)].InitAsDescriptorTable(amountOfDesctiptorRanges, blurDescriptorRanges);
		CD3DX12_STATIC_SAMPLER_DESC linearRepeatSampler(0, D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR);

		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDescription;
		rootSignatureDescription.Init_1_1(static_cast<UINT>(BlurRootParameters::NumParams), rootParameters, 1, &linearRepeatSampler, rootSignatureFlags);

		blurRootSignature.SetRootSignatureDesc(rootSignatureDescription.Desc_1_1, featureData.HighestVersion);

		
		struct PipelineStateStream
		{
			CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
			CD3DX12_PIPELINE_STATE_STREAM_CS CS;
		} pipelineStateStream;

		pipelineStateStream.pRootSignature = blurRootSignature.GetRootSignature().Get();
		pipelineStateStream.CS = CD3DX12_SHADER_BYTECODE(dxBlurComputeShader->getShaderBlob());

		D3D12_PIPELINE_STATE_STREAM_DESC pipelineStateStreamDesc = {
			sizeof(PipelineStateStream), &pipelineStateStream
		};
		ThrowIfFailed(device->CreatePipelineState(&pipelineStateStreamDesc, IID_PPV_ARGS(&blurPipelineState)));
	}

	void DxBlurPass::ExecutePass(Camera& camera, void* perFrameResources)
	{
		UNUSED(perFrameResources);
		UNUSED(camera);

		Texture& mippedTextureToBlur = mipmapPass->GetNonMsaaTextureToMipmap();
		mippedTextureToBlur.CreateViews();
		if(!horizontalBlurredTexture.IsValid()) {
			auto desc = mippedTextureToBlur.GetD3D12ResourceDesc();

			textureFormat = desc.Format;
			horizontalBlurredTexture = Texture(desc, nullptr, TextureUsage::Albedo, L"Horizontal blur Texture");
			horizontalBlurredTexture.CreateViews();
		}

		auto commandQueue = DX12Application::Get().GetCommandQueue(D3D12_COMMAND_LIST_TYPE_COMPUTE);
		auto commandList = commandQueue->GetCommandList();

		commandList->SetPipelineState(blurPipelineState);
		commandList->SetComputeRootSignature(blurRootSignature);

		auto texDesc = mippedTextureToBlur.GetD3D12ResourceDesc();
		auto mipLevels = texDesc.MipLevels;
		auto texWidth = texDesc.Width;
		auto texHeight = texDesc.Height;
		
		BlurData blurData{};
		blurData.blurHorizontal = true;
		mipLevels = 1;
		for(uint32_t i = 0; i < mipLevels; ++i) {
			blurData.mipLevel = i;
			int width = glm::max<uint32_t>(1, texWidth >> i);
			int height = glm::max<uint32_t>(1, texHeight >> i);
			blurData.dimensions = glm::vec2{ width, height };

			D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc;
			uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
			uavDesc.Format = textureFormat;
			uavDesc.Texture2D.MipSlice = i;
			uavDesc.Texture2D.PlaneSlice = 0;


			

			commandList->SetComputeDynamicConstantBuffer(0, blurData);
			commandList->SetShaderResourceView(1, 0, mippedTextureToBlur, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, 0, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);
			commandList->SetUnorderedAccessView(1, 1, horizontalBlurredTexture, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, 0, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, &uavDesc);
			
		}

		
		
		
		
		
		

		
		
		
		
		

		
		
		
		
		

		auto fence = commandQueue->ExecuteCommandList(commandList);
		commandQueue->WaitForFenceValue(fence);
	}
}
#endif
