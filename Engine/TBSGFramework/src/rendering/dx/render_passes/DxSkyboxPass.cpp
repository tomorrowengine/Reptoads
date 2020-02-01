#include "rendering/GeometricShapes.h"
#include "core/Config.h"
#ifdef PLATFORM_WINDOWS
#include "rendering/dx/render_passes/DxSkyboxPass.h"
#include "rendering/dx/DX12Renderer.h"
#include "rendering/dx/DX12Application.h"
#include "rendering/dx/Helpers.h"
#include "rendering/dx/DX12Shader.h"
#include <rendering/dx/CommandQueue.h>
#include <rendering/dx/CommandList.h>

namespace gfx
{
	DxSkyboxPass::DxSkyboxPass(DX12Renderer* renderer, ptl::string skyboxFile) 
		: renderer(renderer)
	{
		Microsoft::WRL::ComPtr<ID3D12Device2> device = DX12Application::Get().GetDevice();

		auto& rm = renderer->GetGfxResourceManager();

		static ShaderId skyboxVertexShader = rm.CreateShader(tbsg::Config::Get().MakeShaderPath("SkyboxShader_v.cso", 1));
		static ShaderId skyboxPixelShader = rm.CreateShader(tbsg::Config::Get().MakeShaderPath("SkyboxShader_p.cso", 1));

		DX12Shader* dxSkyboxVertexShader = static_cast<DX12Shader*>(&rm.GetShader(skyboxVertexShader));
		DX12Shader* dxSkyboxPixelShader = static_cast<DX12Shader*>(&rm.GetShader(skyboxPixelShader));

		TextureId skyboxTextureId = rm.CreateTexture(skyboxFile);
		skyboxCubemap = rm.GetTexture(skyboxTextureId);

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

		const uint32_t rootParamAmount = 2;
		CD3DX12_ROOT_PARAMETER1 rootParameters[rootParamAmount];
		rootParameters[0].InitAsConstantBufferView(0, 0, D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY_VERTEX);
		rootParameters[1].InitAsDescriptorTable(1, &descriptorRange, D3D12_SHADER_VISIBILITY_PIXEL);

		CD3DX12_STATIC_SAMPLER_DESC linearMirrorSampler(0, D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_MIRROR, D3D12_TEXTURE_ADDRESS_MODE_MIRROR);


		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDescription;
		rootSignatureDescription.Init_1_1(rootParamAmount, rootParameters, 1, &linearMirrorSampler, rootSignatureFlags);

		skyboxRootSignature.SetRootSignatureDesc(rootSignatureDescription.Desc_1_1, featureData.HighestVersion);

		
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
		} pipelineStateStream;

		CD3DX12_DEPTH_STENCIL_DESC dsd(D3D12_DEFAULT);
		dsd.DepthEnable = false;
		dsd.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

		CD3DX12_RASTERIZER_DESC rd(D3D12_DEFAULT);
		rd.CullMode = D3D12_CULL_MODE_NONE;

		
		DXGI_FORMAT backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT;

		
		DXGI_SAMPLE_DESC sampleDesc = DX12Application::Get().GetMultisampleQualityLevels(backBufferFormat, D3D12_MAX_MULTISAMPLE_SAMPLE_COUNT);

		D3D12_RT_FORMAT_ARRAY rtvFormats = {};
		rtvFormats.NumRenderTargets = 1;
		rtvFormats.RTFormats[0] = backBufferFormat;

		pipelineStateStream.pRootSignature = skyboxRootSignature.GetRootSignature().Get();
		pipelineStateStream.InputLayout = { VertexPositionNormalTexture::InputElements, VertexPositionNormalTexture::InputElementCount };
		pipelineStateStream.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineStateStream.VS = CD3DX12_SHADER_BYTECODE(dxSkyboxVertexShader->getShaderBlob());
		pipelineStateStream.PS = CD3DX12_SHADER_BYTECODE(dxSkyboxPixelShader->getShaderBlob());
		pipelineStateStream.DSVFormat = depthBufferFormat;
		pipelineStateStream.RTVFormats = rtvFormats;
		pipelineStateStream.SampleDesc = sampleDesc;
		pipelineStateStream.DepthStencil = dsd;
		pipelineStateStream.Rasterizer = rd;


		D3D12_PIPELINE_STATE_STREAM_DESC pipelineStateStreamDesc = {
			sizeof(PipelineStateStream), &pipelineStateStream
		};
		ThrowIfFailed(device->CreatePipelineState(&pipelineStateStreamDesc, IID_PPV_ARGS(&skyboxPipelineState)));

		ptl::vector<uint32_t> indices;
		ptl::vector<Vertex> vertices;
		gfx::CreateCube(25, indices, vertices, true);
		MeshId skyboxCubeMesh = rm.CreateMesh(vertices.data(), vertices.size(), indices.data(), indices.size(), "Skybox Cube");

		skyboxMesh = static_cast<DX12Mesh*>(&rm.GetMesh(skyboxCubeMesh));
	}

	void DxSkyboxPass::ExecutePass(Camera & camera, void * perFrameResources)
	{
		auto commandQueue = DX12Application::Get().GetCommandQueue();
		auto commandList = commandQueue->GetCommandList();

		commandList->SetPipelineState(skyboxPipelineState);
		commandList->SetGraphicsRootSignature(skyboxRootSignature);

		commandList->SetViewport(renderer->GetViewport());
		commandList->SetScissorRect(renderer->GetScissorsRect());

		commandList->SetRenderTarget(renderer->GetRenderTarget());

		glm::vec3 cam_pos = camera.get_Translation();

		glm::mat4 p_mat = camera.get_ProjectionMatrix();
		glm::mat4 v_mat = camera.get_ViewMatrix();
		glm::mat4 m_mat = glm::translate(glm::mat4{ 1.0f }, cam_pos);

		glm::mat4 mvp_mat = p_mat * v_mat * m_mat;
		struct SkyboxMatricesCB
		{
			glm::mat4 ModelMatrix;
			glm::mat4 MVPMatrix;
			glm::vec3 CameraPosition;
		};
		SkyboxMatricesCB sbMatricesCb{};
		sbMatricesCb.ModelMatrix = m_mat;
		sbMatricesCb.MVPMatrix = mvp_mat;
		sbMatricesCb.CameraPosition = cam_pos;

		commandList->SetGraphicsDynamicConstantBuffer(0, sizeof(SkyboxMatricesCB), &sbMatricesCb);

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = skyboxCubemap.GetD3D12ResourceDesc().Format;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MipLevels = (UINT)-1; 

		commandList->SetShaderResourceView(1, 0, skyboxCubemap, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, 0, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, &srvDesc);

		commandList->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		commandList->SetVertexBuffer(0, *skyboxMesh->GetVertexBuffer());
		commandList->SetIndexBuffer(*skyboxMesh->GetIndexBuffer());
		commandList->DrawIndexed(skyboxMesh->GetIdxCount());
		
		uint64_t fence;
		fence = commandQueue->ExecuteCommandList(commandList);
		commandQueue->WaitForFenceValue(fence);
	}

}
#endif
