#ifdef PLATFORM_WINDOWS
#include "rendering/dx/DxParticleRenderer.h"
#include "rendering/dx/DX12Renderer.h"
#include "rendering/dx/DX12Application.h"
#include "rendering/dx/DX12Shader.h"
#include "rendering/dx/Helpers.h"
#include "rendering/dx/CommandQueue.h"
#include "rendering/dx/CommandList.h"

#include "core/Config.h"


namespace gfx
{
	using namespace Microsoft::WRL;

	DxParticleRenderer::DxParticleRenderer(DX12Renderer* renderer, Texture& bloomTexture, DXGI_FORMAT bloomTextureFormat) : renderer(renderer)
	, bloomTex(&bloomTexture), bloomTexFormat(bloomTextureFormat)
	{
		ComPtr<ID3DBlob> errororor;

		auto& rm = renderer->GetGfxResourceManager();
		auto vertexShaderId = rm.CreateShader(tbsg::Config::Get().MakeShaderPath("ParticleShader_v.cso", 1));
		auto pixelShaderId = rm.CreateShader(tbsg::Config::Get().MakeShaderPath("ParticleShader_p.cso", 1));

		auto& vertexShader = rm.GetShader(vertexShaderId);
		auto& pixelShader = rm.GetShader(pixelShaderId);

		auto& dxVertexShader = static_cast<DX12Shader&>(vertexShader);
		auto& dxPixelShader = static_cast<DX12Shader&>(pixelShader);

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

		CD3DX12_DESCRIPTOR_RANGE1 descriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 0);

		const uint32_t rootParamNums = 2;
		CD3DX12_ROOT_PARAMETER1 rootParameters[rootParamNums];
		rootParameters[0].InitAsConstantBufferView(0, 0, D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY_VERTEX);
		rootParameters[1].InitAsDescriptorTable(1, &descriptorRange, D3D12_SHADER_VISIBILITY_PIXEL);


		CD3DX12_STATIC_SAMPLER_DESC linearRepeatSampler(0, D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR);
		CD3DX12_STATIC_SAMPLER_DESC anisotropicSampler(0, D3D12_FILTER_ANISOTROPIC);

		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDescription;
		rootSignatureDescription.Init_1_1(rootParamNums, rootParameters, 1, &linearRepeatSampler, rootSignatureFlags);

		particleRootSignature.SetRootSignatureDesc(rootSignatureDescription.Desc_1_1, featureData.HighestVersion);

		
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
			CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER RasterizerDesc;
			CD3DX12_PIPELINE_STATE_STREAM_BLEND_DESC BlendDesc;
			CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL DsDesc;
		} pipelineStateStream;

		CD3DX12_BLEND_DESC blendDesc(D3D12_DEFAULT);
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		


		CD3DX12_DEPTH_STENCIL_DESC dsd(D3D12_DEFAULT);
		dsd.DepthEnable = true;
		dsd.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

		CD3DX12_RASTERIZER_DESC rd(D3D12_DEFAULT);
		rd.CullMode = D3D12_CULL_MODE_NONE;

		
		DXGI_FORMAT backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT;

		
		DXGI_SAMPLE_DESC sampleDesc = DX12Application::Get().GetMultisampleQualityLevels(backBufferFormat, D3D12_MAX_MULTISAMPLE_SAMPLE_COUNT);

		D3D12_RT_FORMAT_ARRAY rtvFormats = {};
		rtvFormats.NumRenderTargets = 2;
		rtvFormats.RTFormats[0] = backBufferFormat;
		rtvFormats.RTFormats[1] = bloomTexFormat;

		D3D12_INPUT_ELEMENT_DESC inputLayout[2]{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
		};

		pipelineStateStream.pRootSignature = particleRootSignature.GetRootSignature().Get();
		pipelineStateStream.InputLayout = { inputLayout, 2 };
		pipelineStateStream.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineStateStream.VS = CD3DX12_SHADER_BYTECODE(dxVertexShader.getShaderBlob());
		pipelineStateStream.PS = CD3DX12_SHADER_BYTECODE(dxPixelShader.getShaderBlob());
		pipelineStateStream.DSVFormat = depthBufferFormat;
		pipelineStateStream.RTVFormats = rtvFormats;
		pipelineStateStream.SampleDesc = sampleDesc;
		pipelineStateStream.RasterizerDesc = rd;
		pipelineStateStream.BlendDesc = blendDesc;
		pipelineStateStream.DsDesc = dsd;

		D3D12_PIPELINE_STATE_STREAM_DESC pipelineStateStreamDesc = {
			sizeof(PipelineStateStream), &pipelineStateStream
		};
		ThrowIfFailed(device->CreatePipelineState(&pipelineStateStreamDesc, IID_PPV_ARGS(&particlePipelineState)));


		PipelineStateStream backgroundPipelineStream = pipelineStateStream;

		D3D12_PIPELINE_STATE_STREAM_DESC backgroundPipelineStateStreamDesc = {
		sizeof(PipelineStateStream), &backgroundPipelineStream
		};
		ThrowIfFailed(device->CreatePipelineState(&backgroundPipelineStateStreamDesc, IID_PPV_ARGS(&backgroundParticlePipelineState)));
	}

	void DxParticleRenderer::RenderParticles(Camera& camera)
	{
		using core::Transform;

		SortParticles(camera);

		for (auto& punctualParticleEmitter : particleEmitters) {
			auto& transform = punctualParticleEmitter.transform;
			auto& particleEmitter = punctualParticleEmitter.particleEmitter;

			if (!particleEmitter.startParameters.renderLikeSkybox)
				RenderParticleEmitter(transform, particleEmitter, camera);
		}
	}

	void DxParticleRenderer::RenderParticleEmitter(core::Transform& transform, ParticleEmitter& emitter, Camera& camera)
	{
		using DirectX::XMMATRIX;

		if (emitter._particles.empty())
			return;


		struct ConstantBufferMatrices  
		{
			glm::mat4 ModelMatrix;
			glm::mat4 ViewMatrix;
			glm::mat4 ProjectionMatrix;
			glm::mat4 MVPMatrix;
		} cbMats;

		cbMats.ViewMatrix = camera.get_ViewMatrix();
		cbMats.ProjectionMatrix = camera.get_ProjectionMatrix();

		
		if (emitter.startParameters.simulationSpace == SimulationSpace::Local) {
			cbMats.ModelMatrix = transform.GetWorldModelMatrix();
		} else if (emitter.startParameters.simulationSpace == SimulationSpace::LocalOnlyPosition) {
			glm::mat4 modelMat = transform.GetWorldModelMatrix();
			glm::vec4 translation = modelMat[3];
			cbMats.ModelMatrix = glm::translate(glm::mat4{ 1.0f }, glm::vec3{ translation });
		} else {
			cbMats.ModelMatrix = glm::mat4{ 1.0f };
		}

		
		cbMats.MVPMatrix = cbMats.ProjectionMatrix * cbMats.ViewMatrix * cbMats.ModelMatrix;

		auto commandQueue = DX12Application::Get().GetCommandQueue();
		auto commandList = commandQueue->GetCommandList();

		if (emitter.startParameters.renderLikeSkybox)
			commandList->SetPipelineState(backgroundParticlePipelineState);
		else
			commandList->SetPipelineState(particlePipelineState);

		commandList->SetGraphicsRootSignature(particleRootSignature);

		commandList->SetViewport(renderer->GetViewport());
		commandList->SetScissorRect(renderer->GetScissorsRect());

		RenderTarget mrtRenderTarget = renderer->GetRenderTarget();
		mrtRenderTarget.AttachTexture(AttachmentPoint::Color1, *bloomTex);
		commandList->SetRenderTarget(mrtRenderTarget);

		commandList->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		commandList->SetGraphicsDynamicConstantBuffer(0, cbMats);
		commandList->SetShaderResourceView(1, 0, *emitter.diffuseTexture, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		commandList->SetShaderResourceView(1, 1, *emitter.emissiveTexture, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

		std::vector<ParticleVertex> vertices;
		vertices.reserve(emitter._particles.size() * 4);
		std::vector<uint16_t> indices;
		indices.reserve(emitter._particles.size() * 6);

		for (uint32_t i = 0; i < static_cast<uint32_t>(emitter._particles.size()); ++i) {
			auto& particle = emitter._particles[i];
			const static glm::vec3 dirs[4]{
				{-1.f, 1.f, 0.f},
				{1.f, 1.f, 0.f},
				{-1.f, -1.f, 0.f},
				{1.f, -1.f, 0.f},
			};
			float billboardSize = emitter.startParameters.initialUniformScale.Value();
			for (uint32_t j = 0; j < 4; ++j) {
				ParticleVertex vert;
				glm::vec3 vertpos{};
				if (emitter.startParameters.billboardType == BillboardType::WorldspaceQuad) {
					vertpos = particle.position + dirs[j] * billboardSize;
				} else if (emitter.startParameters.billboardType == BillboardType::QuadLookTowardsCamera) {
					
					glm::vec3 CameraRight_worldspace{ cbMats.ViewMatrix[0][0], cbMats.ViewMatrix[1][0], cbMats.ViewMatrix[2][0] };
					glm::vec3 CameraUp_worldspace{ cbMats.ViewMatrix[0][1], cbMats.ViewMatrix[1][1], cbMats.ViewMatrix[2][1] };

					glm::vec3 rightOffset = CameraRight_worldspace * dirs[j].x * billboardSize;
					glm::vec3 upOffset = CameraUp_worldspace * dirs[j].y * billboardSize;
					vertpos = particle.position
						+ rightOffset
						+ upOffset;
				}
				
				glm::vec2 vertuv = (glm::vec2{ dirs[j] } *glm::vec2{ 0.5f } +glm::vec2{ 0.5f }) * glm::vec2(1.f, -1.f);
				vert.Position = vertpos;
				vert.TexCoord = vertuv;
				vertices.push_back(std::move(vert));
			}

			uint16_t start = static_cast<uint16_t>(i) * 4;
			indices.push_back(start + 0);
			indices.push_back(start + 1);
			indices.push_back(start + 2);
			indices.push_back(start + 1);
			indices.push_back(start + 3);
			indices.push_back(start + 2);
		}

		commandList->SetDynamicVertexBuffer(0, vertices);
		commandList->SetDynamicIndexBuffer(indices);

		commandList->DrawIndexed(static_cast<uint32_t>(indices.size()));

		commandQueue->ExecuteCommandList(commandList);
	}
}
#endif
