
#include "rendering/dx/DX12UIRenderer.h"
#ifdef PLATFORM_WINDOWS
#include <rendering/dx/DX12UIPanelRenderer.h>
#include <d3dcompiler.h>
#include <glm/common.hpp>
#include <array>
#include <glm/common.hpp>

#include <rendering/dx/DX12Application.h>
#include <rendering/dx/Helpers.h>
#include <rendering/dx/CommandQueue.h>
#include <rendering/dx/DX12Renderer.h>
#include "core/Assertion.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <array>

#include <ui/UIText.h>
#include <ui/UIImage.h>

static uint32_t PackColor(float r, float g, float b, float a)
{
	uint8_t r255 = static_cast<uint8_t>(glm::clamp(r, 0.0f, 1.0f) * 255.f);
	uint8_t g255 = static_cast<uint8_t>(glm::clamp(g, 0.0f, 1.0f) * 255.f);
	uint8_t b255 = static_cast<uint8_t>(glm::clamp(b, 0.0f, 1.0f) * 255.f);
	uint8_t a255 = static_cast<uint8_t>(glm::clamp(a, 0.0f, 1.0f) * 255.f);

	return r255 | g255 << 8 | b255 << 16 | a255 << 24;
}

namespace gfx
{

	DX12UIPanelRenderer::DX12UIPanelRenderer(DX12Renderer* renderer) : renderer(renderer)
	{
	}

	void DX12UIPanelRenderer::Initialize()
	{
		using namespace Microsoft::WRL;

		ComPtr<ID3DBlob> errororor;

		ComPtr<ID3DBlob > particleVertexShaderBlob;
		if (FAILED(D3DCompileFromFile(L"data/shaders/UIOverlayShader.VS.hlsl", nullptr, nullptr, "main", "vs_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &particleVertexShaderBlob, &errororor))) {
			ASSERT(errororor != nullptr);
			auto* cstr = static_cast<const char*>(errororor->GetBufferPointer());
			MessageBoxA(NULL, cstr, "Failed vertex shader compile", MB_OK);
		}

		ComPtr<ID3DBlob > particlePixelShaderBlob;
		if (FAILED(D3DCompileFromFile(L"data/shaders/UIOverlayShader.PS.hlsl", nullptr, nullptr, "main", "ps_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &particlePixelShaderBlob, &errororor))) {
			ASSERT(errororor != nullptr);
			auto* cstr = static_cast<const char*>(errororor->GetBufferPointer());
			MessageBoxA(NULL, cstr, "Failed pixel shader compile", MB_OK);
		}

		ComPtr<ID3D12Device2> device = DX12Application::Get().GetDevice();

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

		CD3DX12_DESCRIPTOR_RANGE1 descriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

		const uint32_t rootParamNums = 2;
		CD3DX12_ROOT_PARAMETER1 rootParameters[rootParamNums];
		rootParameters[0].InitAsConstantBufferView(0, 0, D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY_VERTEX);
		rootParameters[1].InitAsDescriptorTable(1, &descriptorRange, D3D12_SHADER_VISIBILITY_PIXEL);


		CD3DX12_STATIC_SAMPLER_DESC linearClampSampler(0, D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP);

		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDescription;
		rootSignatureDescription.Init_1_1(rootParamNums, rootParameters, 1, &linearClampSampler, rootSignatureFlags);

		uiOverlayRootSignature.SetRootSignatureDesc(rootSignatureDescription.Desc_1_1, featureData.HighestVersion);

		
		struct PipelineStateStream
		{
			CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
			CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
			CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveTopologyType;
			CD3DX12_PIPELINE_STATE_STREAM_VS VS;
			CD3DX12_PIPELINE_STATE_STREAM_PS PS;
			CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat;
			CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
			CD3DX12_PIPELINE_STATE_STREAM_SAMPLE_DESC SampleDesc;
			CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL DepthStencil;
			CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER Rasterizer;
			CD3DX12_PIPELINE_STATE_STREAM_BLEND_DESC BlendDesc;
		} pipelineStateStream;

		CD3DX12_DEPTH_STENCIL_DESC dsd(D3D12_DEFAULT);
		dsd.DepthEnable = false;
		dsd.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

		CD3DX12_RASTERIZER_DESC rd(D3D12_DEFAULT);
		rd.CullMode = D3D12_CULL_MODE_NONE;

		
		
		
		

		
		
		 
	  
	  
	  
	  
	  
	  
	  
	  
	  

	  

	  

	  
	  
	  
		 
		 

		CD3DX12_BLEND_DESC blendDesc(D3D12_DEFAULT);
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

		
		DXGI_FORMAT backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT;

		
		DXGI_SAMPLE_DESC sampleDesc = DX12Application::Get().GetMultisampleQualityLevels(backBufferFormat, D3D12_MAX_MULTISAMPLE_SAMPLE_COUNT);

		D3D12_RT_FORMAT_ARRAY rtvFormats = {};
		rtvFormats.NumRenderTargets = 1;
		rtvFormats.RTFormats[0] = backBufferFormat;

		const uint32_t inputLayoutAmount = 2;
		D3D12_INPUT_ELEMENT_DESC inputLayout[inputLayoutAmount]{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float)*9, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
		};

		pipelineStateStream.pRootSignature = uiOverlayRootSignature.GetRootSignature().Get();
		pipelineStateStream.InputLayout = { inputLayout, inputLayoutAmount };
		pipelineStateStream.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineStateStream.VS = CD3DX12_SHADER_BYTECODE(particleVertexShaderBlob.Get());
		pipelineStateStream.PS = CD3DX12_SHADER_BYTECODE(particlePixelShaderBlob.Get());
		pipelineStateStream.DSVFormat = depthBufferFormat;
		pipelineStateStream.RTVFormats = rtvFormats;
		pipelineStateStream.SampleDesc = sampleDesc;
		pipelineStateStream.DepthStencil = dsd;
		pipelineStateStream.Rasterizer = rd;
		pipelineStateStream.BlendDesc = blendDesc;

		D3D12_PIPELINE_STATE_STREAM_DESC pipelineStateStreamDesc = {
			sizeof(PipelineStateStream), &pipelineStateStream
		};
		ThrowIfFailed(device->CreatePipelineState(&pipelineStateStreamDesc, IID_PPV_ARGS(&uiOverlayPipelineState)));

		auto commandQ = DX12Application::Get().GetCommandQueue();
		auto commandList = commandQ->GetCommandList();
		commandList->CopyIndexBuffer(indexBuffer, IUIRenderer::CachedUIImage::indices.size(), DXGI_FORMAT_R32_UINT, IUIRenderer::CachedUIImage::indices.data());
		auto fence = commandQ->ExecuteCommandList(commandList);
		commandQ->WaitForFenceValue(fence);
	}

	void DX12UIPanelRenderer::StartFrame()
	{
		auto viewport = renderer->GetViewport();
		auto screenSize = renderer->GetUIRenderer()->m_targetScreenSize;
		glm::mat4 ortho = glm::orthoLH(0.0f, screenSize.x,
		0.0f, screenSize.y,
		viewport.MinDepth, viewport.MaxDepth);
		
		

		commandQueue_ = DX12Application::Get().GetCommandQueue();
		commandList_ = commandQueue_->GetCommandList();

		commandList_->SetPipelineState(uiOverlayPipelineState);
		commandList_->SetGraphicsRootSignature(uiOverlayRootSignature);

		commandList_->SetViewport(renderer->GetViewport());
		commandList_->SetScissorRect(renderer->GetScissorsRect());

		commandList_->SetRenderTarget(renderer->GetRenderTarget());

		commandList_->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		struct uiOverlayMatricesCB
		{
			glm::mat4 OrthoViewMat;
		};
		uiOverlayMatricesCB uiMatricesCb{ ortho };

		commandList_->SetGraphicsDynamicConstantBuffer(0, sizeof(uiOverlayMatricesCB), &uiMatricesCb);
	}

	void DX12UIPanelRenderer::RenderPanel(gfx::IUIRenderer::CachedUIImage* uiPanel)
	{
		ui::UIImage* uiImage = uiPanel->uiImage;
		auto& tex = renderer->GetGfxResourceManager().GetTexture(uiImage->GetTextureId().c_str());
		commandList_->SetShaderResourceView(1, 0, tex, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

		
		commandList_->SetDynamicVertexBuffer(0, uiPanel->vertices.size(), 
			sizeof(gfx::Vertex), uiPanel->vertices.data());
		commandList_->SetIndexBuffer(indexBuffer); 
		
		
		commandList_->DrawIndexed(static_cast<uint32_t>(uiPanel->indices.size()));
	}
	void DX12UIPanelRenderer::RenderPanel(gfx::IUIRenderer::CachedUIText* uiPanel)
	{
		ui::UIText* uiImage = uiPanel->uiText;
		UITextOptionsForTexture options{
			uiImage->GetSize(),
			uiImage->GetFontSize(),
			uiImage->GetOldText(),
			static_cast<UIText::HorizontalAlignment>(uiImage->GetHorizontalAlignment()),
			static_cast<UIText::VerticalAlignment>(uiImage->GetVerticalAlignment()),
			uiImage->GetFontPath(),
			PackColor(uiImage->GetColor().x, uiImage->GetColor().y, uiImage->GetColor().z, uiImage->GetColor().w)
		};

		auto* textTexture = static_cast<DX12UIRenderer*>(renderer->GetUIRenderer())->GetUITextRenderer().GetCachedTextTexture(options);
		
		auto& tex = *textTexture->dx12Texture;
		commandList_->SetShaderResourceView(1, 0, tex, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

		
		commandList_->SetDynamicVertexBuffer(0, uiPanel->vertices.size(),
			sizeof(gfx::Vertex), uiPanel->vertices.data());
		commandList_->SetIndexBuffer(indexBuffer);
		
		
		commandList_->DrawIndexed(static_cast<uint32_t>(uiPanel->indices.size()));
	}

	void DX12UIPanelRenderer::EndFrame()
	{
		auto fenceValue = commandQueue_->ExecuteCommandList(commandList_);
		commandQueue_->WaitForFenceValue(fenceValue);

		commandQueue_.reset();
		commandList_.reset();
	}

	void DX12UIPanelRenderer::Shutdown()
	{

	}

	void DX12UIPanelRenderer::AddImage(gfx::IUIRenderer::CachedUIImage& uiImage)
	{
		UNUSED(uiImage);

		
		
		
		
		
		
		
		
		
	}

	void DX12UIPanelRenderer::UpdateImage(gfx::IUIRenderer::CachedUIImage& uiImage)
	{
		UNUSED(uiImage);

		
		
		
		
		
		
	}

	void DX12UIPanelRenderer::RemoveImage(ui::UIImage& uiImage)
	{
		UNUSED(uiImage);

		
		
		
	}

	
}
#endif
