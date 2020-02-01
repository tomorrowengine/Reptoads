
#ifdef PLATFORM_WINDOWS
#include <rendering/dx/render_passes/DxForwardPass.h>
#include <brofiler/Brofiler.h>
#include "rendering/dx/DX12Mesh.h"
#include "rendering/dx/DX12Application.h"
#include <rendering/dx/CommandQueue.h>
#include <rendering/dx/CommandList.h>
#include <rendering/dx/Helpers.h>
#include <rendering/dx/DX12Renderer.h>
#include "rendering/dx/DX12Shader.h"
#include "DefaultShaderBuffers.h"

#include "core/Config.h"

namespace gfx
{
	
	
	
	enum ForwardRootParameters
	{
		PerFrameConstants,	
		PerObjectConstants,	
		Material,			
		Textures,				
								
								
		NumRootParameters
	};

	const int GfxVertexInputElementAmount = 3;
	const D3D12_INPUT_ELEMENT_DESC GfxVertexInputElements[GfxVertexInputElementAmount] =
	{
		{ "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 6 * 4, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	DxForwardPass::DxForwardPass(DX12Renderer* renderer, ptl::vector<IShader*> shaders)
		: renderer(renderer)
	{
		int scrWidth = tbsg::Config::Get().width;
		int scrHeight = tbsg::Config::Get().height;

		
		DXGI_SAMPLE_DESC sampleDesc = DX12Application::Get().GetMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, D3D12_MAX_MULTISAMPLE_SAMPLE_COUNT);

		D3D12_RESOURCE_DESC rwBloomDesc = CD3DX12_RESOURCE_DESC::Tex2D(bloomTextureFormat, scrWidth, scrHeight, 1, 1, sampleDesc.Count, sampleDesc.Quality);
		rwBloomDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		rwBloomTexture = Texture{ rwBloomDesc, nullptr, TextureUsage::RenderTarget, L"RW Bloom Texture" };

		for (IShader* shader : shaders) {
			shaders_.insert(std::pair<ShaderType, IShader*>(shader->type, shader));
		}
		
		auto vs_it = shaders_.find(ShaderType::VS);
		ASSERT(vs_it != shaders_.end());
		Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderBlob = static_cast<DX12Shader*>(vs_it->second)->getShaderBlob();
		
		auto ps_it = shaders_.find(ShaderType::PS);
		ASSERT(ps_it != shaders_.end());
		Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderBlob = static_cast<DX12Shader*>(ps_it->second)->getShaderBlob();
	

		auto device = DX12Application::Get().GetDevice();
		auto commandQueue = DX12Application::Get().GetCommandQueue(D3D12_COMMAND_LIST_TYPE_COPY);
		auto commandList = commandQueue->GetCommandList();

		
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

		const int amountOfDesctiptorRanges = 1;
		CD3DX12_DESCRIPTOR_RANGE1 forwardDescriptorRanges[amountOfDesctiptorRanges]{
			CD3DX12_DESCRIPTOR_RANGE1(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 0),
			
		};
		
		CD3DX12_ROOT_PARAMETER1 rootParameters[ForwardRootParameters::NumRootParameters];
		rootParameters[ForwardRootParameters::PerFrameConstants].InitAsConstantBufferView(0);
		rootParameters[ForwardRootParameters::PerObjectConstants].InitAsConstantBufferView(1);
		rootParameters[ForwardRootParameters::Material].InitAsConstantBufferView(2);
		rootParameters[ForwardRootParameters::Textures].InitAsDescriptorTable(amountOfDesctiptorRanges, forwardDescriptorRanges, D3D12_SHADER_VISIBILITY_PIXEL);

		CD3DX12_STATIC_SAMPLER_DESC linearRepeatSampler(0, D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR);

		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDescription;
		rootSignatureDescription.Init_1_1(ForwardRootParameters::NumRootParameters, rootParameters, 1, &linearRepeatSampler, rootSignatureFlags);

		rootSignature_.SetRootSignatureDesc(rootSignatureDescription.Desc_1_1, featureData.HighestVersion);
		
		
		struct PipelineStateStream
		{
			CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
			CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
			CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveTopologyType;
			CD3DX12_PIPELINE_STATE_STREAM_VS VS;
			CD3DX12_PIPELINE_STATE_STREAM_PS PS;
			CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat;
			CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER Rasterizer;
			CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
			CD3DX12_PIPELINE_STATE_STREAM_SAMPLE_DESC SampleDesc;
			CD3DX12_PIPELINE_STATE_STREAM_BLEND_DESC BlendDesc;
		} pipelineStateStream;

		
		DXGI_FORMAT backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT;


		D3D12_RT_FORMAT_ARRAY rtvFormats = {};
		rtvFormats.NumRenderTargets = 2;
		rtvFormats.RTFormats[0] = backBufferFormat;
		rtvFormats.RTFormats[1] = bloomTextureFormat;

		auto rasterizerDesc = CD3DX12_RASTERIZER_DESC{ D3D12_DEFAULT };
		rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;

		pipelineStateStream.pRootSignature = rootSignature_.GetRootSignature().Get();
		pipelineStateStream.InputLayout = { GfxVertexInputElements, GfxVertexInputElementAmount };
		pipelineStateStream.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineStateStream.VS = CD3DX12_SHADER_BYTECODE(vertexShaderBlob.Get());
		pipelineStateStream.PS = CD3DX12_SHADER_BYTECODE(pixelShaderBlob.Get());
		pipelineStateStream.DSVFormat = depthBufferFormat;
		pipelineStateStream.RTVFormats = rtvFormats;
		pipelineStateStream.Rasterizer = rasterizerDesc;
		pipelineStateStream.SampleDesc = sampleDesc;

		auto alphaBlend = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		alphaBlend.AlphaToCoverageEnable = true;
		alphaBlend.IndependentBlendEnable = FALSE;
		alphaBlend.RenderTarget[0].BlendEnable = FALSE;
		alphaBlend.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		alphaBlend.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		alphaBlend.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		alphaBlend.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;
		alphaBlend.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
		alphaBlend.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;

		
		alphaBlend.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		
		pipelineStateStream.BlendDesc = alphaBlend;

		D3D12_PIPELINE_STATE_STREAM_DESC pipelineStateStreamDesc = {
			sizeof(PipelineStateStream), &pipelineStateStream
		};
		ThrowIfFailed(device->CreatePipelineState(&pipelineStateStreamDesc, IID_PPV_ARGS(&pipelineState_)));
	}

	static DirectX::XMVECTOR ConvertVector(glm::vec4 vec)
	{
		return DirectX::XMVectorSet(vec.x, vec.y, vec.z, vec.w);
	}

	static void ComputeMatrices(Camera& camera, const glm::mat4& model, const glm::mat4& view, const glm::mat4& viewProjection, 
		PerFrameConstantsVs& perFrame, PerObjectConstantsVs& perObject)
	{
		perFrame.camPosWs = glm::vec4{ camera.get_Translation(), 1.0f };
		perFrame.viewMatrix = view;
		perFrame.vpMatrix = viewProjection;
		perObject.modelMatrix = model;
		perObject.normalMatrix = glm::transpose(glm::inverse(model));
		
		
		
		
	}

	void DxForwardPass::ExecutePass(Camera& camera, void* perFrameResources)
	{ BROFILER_CATEGORY("DxForwardPass::ExecutePass", Profiler::Color::Cyan)

		UNUSED(perFrameResources);

		if(renderer->justResizedDuringFrame) {
			rwBloomTexture.Resize(renderer->windowClientWidth_, renderer->windowClientHeight_);
		}

		auto commandQueue = DX12Application::Get().GetCommandQueue();
		auto commandList = commandQueue->GetCommandList();

		float emptyBlack[4]{ 0.0f, 0.0f, 0.0f, 0.0f };
		commandList->ClearTexture(rwBloomTexture, emptyBlack);

		PrepareForRendering(*commandList);


		auto& resourceManager = renderer->GetGfxResourceManager();
		glm::mat4 camViewMatrix = camera.get_ViewMatrix();
		glm::mat4 camProjMatrix = camera.get_ProjectionMatrix();
		glm::mat4 camViewProjMatrix = camProjMatrix * camViewMatrix;

		for (auto& renderable : renderer->renderables) {

			glm::mat4 modelMat = renderable.transform->GetWorldModelMatrix();

			PerObjectConstantsVs perObject;
			PerFrameConstantsVs perFrame;
			BROFILER_INLINE_EVENT("Renderable: ComputeMatrices", ComputeMatrices(camera, modelMat, camViewMatrix, camViewProjMatrix, perFrame, perObject); );

			auto& material = resourceManager.GetMaterial(renderable.materialId);
			auto& shaderMaterial = material.GetShaderMaterialData();
			Texture diffuseTexture = resourceManager.GetTexture(material.diffuseTextureId);
			Texture emissiveTexture = resourceManager.GetTexture(material.emissiveTextureId);

			commandList->SetShaderResourceView(ForwardRootParameters::Textures, 0, diffuseTexture, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
			commandList->SetShaderResourceView(ForwardRootParameters::Textures, 1, emissiveTexture, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
			
			commandList->SetGraphicsDynamicConstantBuffer(ForwardRootParameters::PerFrameConstants, perFrame);
			commandList->SetGraphicsDynamicConstantBuffer(ForwardRootParameters::PerObjectConstants, perObject);
			commandList->SetGraphicsDynamicConstantBuffer(ForwardRootParameters::Material, shaderMaterial);
			

			auto& mesh = resourceManager.GetMesh(renderable.meshId);
			auto* dxMesh = static_cast<DX12Mesh*>(&mesh);
			commandList->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			BROFILER_INLINE_EVENT("SetVertexBuffer Mesh", commandList->SetVertexBuffer(0, *dxMesh->GetVertexBuffer()); );
			BROFILER_INLINE_EVENT("SetIndexBuffer Mesh", commandList->SetIndexBuffer(*dxMesh->GetIndexBuffer()); );
			BROFILER_INLINE_EVENT("DrawIndexed", commandList->DrawIndexed(dxMesh->GetIdxCount()); );
		}

		uint64_t fence;
		BROFILER_INLINE_EVENT("ExecuteCommandList", fence = commandQueue->ExecuteCommandList(commandList); );
		BROFILER_INLINE_EVENT("WaitForFenceValue", commandQueue->WaitForFenceValue(fence); );
	}

	void DxForwardPass::PrepareForRendering(CommandList& commandList)
	{
		commandList.SetPipelineState(pipelineState_);
		commandList.SetGraphicsRootSignature(rootSignature_);

		commandList.SetViewport(renderer->GetViewport());
		commandList.SetScissorRect(renderer->GetScissorsRect());

		RenderTarget mrtRenderTarget = renderer->GetRenderTarget();
		mrtRenderTarget.AttachTexture(AttachmentPoint::Color1, rwBloomTexture);
		commandList.SetRenderTarget(mrtRenderTarget);
	}
}
#endif
