#include <locale>
#include <codecvt>
#include <brofiler/Brofiler.h>
#include "rendering/dx/render_passes/DxForwardPass.h"
#ifdef PLATFORM_WINDOWS
#include <d3dcompiler.h>

#include <rendering/dx/DX12Renderer.h>
#include <rendering/dx/DX12DebugRenderer.h>
#include "rendering/dx/DX12UIRenderer.h"

#include <rendering/dx/DX12Application.h>

#include <rendering/dx/CommandQueue.h>
#include <rendering/dx/CommandList.h>
#include <rendering/dx/Helpers.h>
#include "core/Config.h"

#include <DirectXColors.h>

#pragma warning(push)
#pragma warning( disable : 4127)
#include <glm/gtx/euler_angles.hpp>
#pragma warning(pop)
#include <iostream>
#include "rendering/dx/DX12ResourceManager.h"


#include "rendering/dx/render_passes/DxSkyboxPass.h"

namespace gfx
{
	using Microsoft::WRL::ComPtr;

#pragma region TestStructs
	
	
	
	enum RootParameters
	{
		MatricesCB,         
		MaterialCB,         
		LightPropertiesCB,  
		PointLights,        
		SpotLights,         
		Textures,           
		NumRootParameters
	};

	struct Mat
	{
		DirectX::XMMATRIX ModelMatrix;
		DirectX::XMMATRIX ModelViewMatrix;
		DirectX::XMMATRIX InverseTransposeModelViewMatrix;
		DirectX::XMMATRIX ModelViewProjectionMatrix;
	};
#pragma endregion 

	DX12Renderer::DX12Renderer(HINSTANCE hInstance, bool useVSync) : IRenderer(std::make_unique<DX12DebugRenderer>(this), std::make_unique<DX12UIRenderer>(this), std::make_unique<DX12ResourceManager>(this)),
		hInstance_(hInstance),
		viewport_({ 0.0f, 0.0f, static_cast<float>(tbsg::Config::Get().width), static_cast<float>(tbsg::Config::Get().height), 0.01f, 1.0f }),
		scissorRect_(CD3DX12_RECT(0, 0, LONG_MAX, LONG_MAX)),
		windowClientWidth_(tbsg::Config::Get().width),
		windowClientHeight_(tbsg::Config::Get().height),
		useVSync_(useVSync)
	{

	}

	
	template<typename T>
	static constexpr const T& clamp(const T& val, const T& min, const T& max)
	{
		return val < min ? min : val > max ? max : val;
	}



	void DX12Renderer::Initialize()
	{
		DX12Application::Create(hInstance_);

		window_ = DX12Application::Get().CreateRenderWindow(L"Reptoads", windowClientWidth_, windowClientHeight_, useVSync_);
		window_->Show();

		window_->OnResizeEvent = [this](ResizeEventArgs& e) {
			if (this->windowClientWidth_ != e.Width || this->windowClientHeight_ != e.Height) {
				this->windowClientWidth_ = glm::max(1, e.Width);
				this->windowClientHeight_ = glm::max(1, e.Height);

				if (lastUsedCamera) {
					float aspectRatio = this->windowClientWidth_ / (float)this->windowClientHeight_;
					lastUsedCamera->set_Projection(45.0f, aspectRatio, 0.1f, 100.0f);
				}

				viewport_ = CD3DX12_VIEWPORT(0.0f, 0.0f,
					static_cast<float>(this->windowClientWidth_), static_cast<float>(this->windowClientHeight_));

				renderTarget_.Resize(this->windowClientWidth_, this->windowClientHeight_);
				justResizedDuringFrame = true;
			}

			
		};

		std::function<void(KeyEventArgs& e)> keyHandler = [this](const KeyEventArgs& e) {
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
		};

		window_->OnKeyPressedEvent = keyHandler;
		window_->OnKeyReleasedEvent = keyHandler;

		window_->OnMouseMovedEvent = [this](MouseMotionEventArgs& e) {
			if (e.LeftButton) {
				const float mouseSpeed = 0.25f;
				this->pitch -= e.RelY * mouseSpeed;

				this->pitch = clamp(this->pitch, -90.0f, 90.0f);

				this->yaw -= e.RelX * mouseSpeed;
			}
		};

		auto device = DX12Application::Get().GetDevice();
		auto commandQueue = DX12Application::Get().GetCommandQueue(D3D12_COMMAND_LIST_TYPE_COPY);
		auto commandList = commandQueue->GetCommandList();

		
		
		
		
		
		

		ptl::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		auto t = converter.from_bytes(tbsg::Config::Get().MakeTexturePath("DefaultWhite.bmp"));
		commandList->LoadTextureFromFile(m_EarthTexture, t);

		
		ComPtr<ID3DBlob> vertexShaderBlob;
		ThrowIfFailed(D3DReadFileToBlob(converter.from_bytes(tbsg::Config::Get().MakeShaderPath("VertexShader_v.cso", 1)).c_str(), &vertexShaderBlob));

		
		ComPtr<ID3DBlob> pixelShaderBlob;
		ThrowIfFailed(D3DReadFileToBlob(converter.from_bytes(tbsg::Config::Get().MakeShaderPath("PixelShader_p.cso", 1)).c_str(), &pixelShaderBlob));
		

		
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

		CD3DX12_DESCRIPTOR_RANGE1 descriptorRage(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);

		CD3DX12_ROOT_PARAMETER1 rootParameters[RootParameters::NumRootParameters];
		rootParameters[RootParameters::MatricesCB].InitAsConstantBufferView(0, 0, D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY_VERTEX);
		rootParameters[RootParameters::MaterialCB].InitAsConstantBufferView(0, 1, D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY_PIXEL);
		rootParameters[RootParameters::LightPropertiesCB].InitAsConstants(sizeof(double) / 4, 1, 0, D3D12_SHADER_VISIBILITY_PIXEL);
		rootParameters[RootParameters::PointLights].InitAsShaderResourceView(0, 0, D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY_PIXEL);
		rootParameters[RootParameters::SpotLights].InitAsShaderResourceView(1, 0, D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY_PIXEL);
		rootParameters[RootParameters::Textures].InitAsDescriptorTable(1, &descriptorRage, D3D12_SHADER_VISIBILITY_PIXEL);

		CD3DX12_STATIC_SAMPLER_DESC linearRepeatSampler(0, D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR);
		CD3DX12_STATIC_SAMPLER_DESC anisotropicSampler(0, D3D12_FILTER_ANISOTROPIC);

		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDescription;
		rootSignatureDescription.Init_1_1(RootParameters::NumRootParameters, rootParameters, 1, &linearRepeatSampler, rootSignatureFlags);

		rootSignature_.SetRootSignatureDesc(rootSignatureDescription.Desc_1_1, featureData.HighestVersion);
		
		
		struct PipelineStateStream
		{
			CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
			CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
			CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveTopologyType;
			CD3DX12_PIPELINE_STATE_STREAM_VS VS;
			CD3DX12_PIPELINE_STATE_STREAM_PS PS;
			CD3DX12_PIPELINE_STATE_STREAM_BLEND_DESC blendDesc;
			CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL DsSettings;
			CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat;
			CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER Rasterizer;
			CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
			CD3DX12_PIPELINE_STATE_STREAM_SAMPLE_DESC SampleDesc;
		} pipelineStateStream;

		
		DXGI_FORMAT backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT;

		
		DXGI_SAMPLE_DESC sampleDesc = DX12Application::Get().GetMultisampleQualityLevels(backBufferFormat, D3D12_MAX_MULTISAMPLE_SAMPLE_COUNT);

		D3D12_RT_FORMAT_ARRAY rtvFormats = {};
		rtvFormats.NumRenderTargets = 1;
		rtvFormats.RTFormats[0] = backBufferFormat;

		CD3DX12_DEPTH_STENCIL_DESC dsDesc(D3D12_DEFAULT);
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		
		

		CD3DX12_BLEND_DESC blendDesc(D3D12_DEFAULT);
		blendDesc.AlphaToCoverageEnable = true;
		blendDesc.RenderTarget[0].BlendEnable = false;
		
		
		

		CD3DX12_RASTERIZER_DESC rasterizerDesc(D3D12_DEFAULT);
		rasterizerDesc.MultisampleEnable = true;


		pipelineStateStream.pRootSignature = rootSignature_.GetRootSignature().Get();
		pipelineStateStream.InputLayout = { VertexPositionNormalTexture::InputElements, VertexPositionNormalTexture::InputElementCount };
		pipelineStateStream.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineStateStream.VS = CD3DX12_SHADER_BYTECODE(vertexShaderBlob.Get());
		pipelineStateStream.PS = CD3DX12_SHADER_BYTECODE(pixelShaderBlob.Get());
		pipelineStateStream.blendDesc = blendDesc;
		pipelineStateStream.DsSettings = dsDesc;
		pipelineStateStream.DSVFormat = depthBufferFormat;
		pipelineStateStream.Rasterizer = rasterizerDesc;
		pipelineStateStream.RTVFormats = rtvFormats;
		pipelineStateStream.SampleDesc = sampleDesc;

		D3D12_PIPELINE_STATE_STREAM_DESC pipelineStateStreamDesc = {
			sizeof(PipelineStateStream), &pipelineStateStream
		};
		ThrowIfFailed(device->CreatePipelineState(&pipelineStateStreamDesc, IID_PPV_ARGS(&pipelineState_)));

		
		auto colorDesc = CD3DX12_RESOURCE_DESC::Tex2D(backBufferFormat,
			windowClientWidth_, windowClientHeight_,
			1, 1,
			sampleDesc.Count, sampleDesc.Quality,
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
		D3D12_CLEAR_VALUE colorClearValue;
		colorClearValue.Format = colorDesc.Format;
		colorClearValue.Color[0] = 0.4f;
		colorClearValue.Color[1] = 0.6f;
		colorClearValue.Color[2] = 0.9f;
		colorClearValue.Color[3] = 1.0f;

		Texture colorTexture = Texture(colorDesc, &colorClearValue,
			TextureUsage::RenderTarget,
			L"Color Render Target");

		
		auto depthDesc = CD3DX12_RESOURCE_DESC::Tex2D(depthBufferFormat,
			windowClientWidth_, windowClientHeight_,
			1, 1,
			sampleDesc.Count, sampleDesc.Quality,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
		D3D12_CLEAR_VALUE depthClearValue;
		depthClearValue.Format = depthDesc.Format;
		depthClearValue.DepthStencil = { 1.0f, 0 };

		Texture depthTexture = Texture(depthDesc, &depthClearValue,
			TextureUsage::Depth,
			L"Depth Render Target");

		
		renderTarget_.AttachTexture(AttachmentPoint::Color0, colorTexture);
		renderTarget_.AttachTexture(AttachmentPoint::DepthStencil, depthTexture);

		auto fenceValue = commandQueue->ExecuteCommandList(commandList);
		commandQueue->WaitForFenceValue(fenceValue);
		m_uiRenderer->Initialize(*m_resourceManager);
	}


	static DirectX::XMVECTOR ConvertVector(glm::vec4 vec)
	{
		return DirectX::XMVectorSet(vec.x, vec.y, vec.z, vec.w);
	}

	static glm::vec3 ToGlmVec3(DirectX::XMVECTOR vec)
	{
		DirectX::XMFLOAT3 f3;
		DirectX::XMStoreFloat3(&f3, vec);
		return glm::vec3(f3.x, f3.y, f3.z);
	}

	static glm::vec4 ToGlmVec4(DirectX::XMVECTOR vec)
	{
		DirectX::XMFLOAT4 f4;
		DirectX::XMStoreFloat4(&f4, vec);
		return glm::vec4(f4.x, f4.y, f4.z, f4.w);
	}

	static glm::quat ToGlmQuat(DirectX::XMVECTOR vec)
	{
		DirectX::XMFLOAT4 f4;
		DirectX::XMStoreFloat4(&f4, vec);
		return glm::quat(f4.x, f4.y, f4.z, f4.w);
	}

	static DirectX::XMMATRIX ConvertMatrix(glm::mat4 mat)
	{
		DirectX::XMMATRIX m{ ConvertVector(mat[0]), ConvertVector(mat[1]), ConvertVector(mat[2]), ConvertVector(mat[3]) };
		return m;
		
	}

	static void ComputeMatrices(const glm::mat4& model, const glm::mat4& view, const glm::mat4& viewProjection, Mat& mat)
	{
		mat.ModelMatrix = ConvertMatrix(model);
		mat.ModelViewMatrix = ConvertMatrix(view * model);
		mat.InverseTransposeModelViewMatrix = XMMatrixTranspose(XMMatrixInverse(nullptr, mat.ModelViewMatrix));
		mat.ModelViewProjectionMatrix = ConvertMatrix(viewProjection * model);
	}



	void DX12Renderer::RenderWorld(Camera& camera)
	{BROFILER_CATEGORY("DX12Renderer::RenderWorld", Profiler::Color::Blue)
		DX12Application::Get().PollWindowEvents();

	lastUsedCamera = &camera;

		auto commandQueue = DX12Application::Get().GetCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
		auto commandList = commandQueue->GetCommandList();

		
		{
			float clearColor[] = { 0.4f, 0.6f, 0.9f, 1.0f };

			commandList->ClearTexture(renderTarget_.GetTexture(AttachmentPoint::Color0), clearColor);
			commandList->ClearDepthStencilTexture(renderTarget_.GetTexture(AttachmentPoint::DepthStencil), D3D12_CLEAR_FLAG_DEPTH);
		}

		
		

		using namespace DirectX;

		
		XMVECTOR cameraTranslate = XMVectorSet(right - left, 0.0f, forward - backward, 1.0f) * 15.0f * static_cast<float>(1.f / 60.f);
		XMVECTOR cameraPan = XMVectorSet(0.0f, up - down, 0.0f, 1.0f) * 15.0f * static_cast<float>(1.f / 60.f);
		camera.Translate(ToGlmVec3(cameraTranslate), Space::Local);
		camera.Translate(ToGlmVec3(cameraPan), Space::Local);

		glm::quat cameraRotation = glm::quat_cast(glm::yawPitchRoll(glm::radians(yaw), glm::radians(pitch), 0.0f));
		

		auto fence = commandQueue->ExecuteCommandList(commandList);
		commandQueue->WaitForFenceValue(fence);

		RenderRenderables(camera);
	}

	void DX12Renderer::RenderUI(Camera& camera)
	{BROFILER_CATEGORY("DX12Renderer::RenderUI", Profiler::Color::Blue)
		m_uiRenderer->Render(camera);
	}

	void DX12Renderer::Present()
	{BROFILER_CATEGORY("DX12Renderer::Present", Profiler::Color::Blue)
		justResizedDuringFrame = false;
		window_->Present(renderTarget_.GetTexture(AttachmentPoint::Color0));
	}

	void DX12Renderer::Shutdown()
	{
		m_uiRenderer->Shutdown();

		DX12Application::Get().DestroyWindow(window_);
		window_.reset();
	}

	RenderableId DX12Renderer::AddRenderable(core::Transform* transform, MeshId meshId, MaterialId materialId)
	{
		return renderables.push_back(PunctualRenderable{transform, meshId, materialId});
	}

	bool DX12Renderer::ContainsRenderable(RenderableId renderableId)
	{
		return renderables.contains(renderableId);
	}

	void DX12Renderer::RemoveRenderable(RenderableId renderableId)
	{
		ASSERT(renderables.contains(renderableId));
		renderables.erase(renderableId);
	}

	RenderPassId DX12Renderer::AddRenderPass(IRenderPass* renderPass)
	{
		return renderPasses.push_back(ptl::unique_ptr<IRenderPass>{ renderPass });
	}

	bool DX12Renderer::ContainsRenderPass(RenderPassId renderPassId) 
	{
		return renderPasses.contains(renderPassId);
	}

	void DX12Renderer::RemoveRenderPass(RenderPassId renderPassId)
	{
		UNUSED(renderPassId);
		
		ASSERT_MSG(FALSE, "NOT IMPLEMENTED BECAUSE OF COMMENT HERE ABOVE");

		
		
	}

	void DX12Renderer::RenderRenderables(Camera& camera)
	{BROFILER_CATEGORY("DX12Renderer::RenderRenderables", Profiler::Color::Blue)
		glm::mat4 camViewMatrix = camera.get_ViewMatrix();
		glm::mat4 camProjMatrix = camera.get_ProjectionMatrix();
		glm::mat4 camViewProjMatrix = camProjMatrix * camViewMatrix;

		
		for (auto& renderPass : renderPasses) {

			renderPass->ExecutePass(camera, nullptr);
		}
	}
}
#endif
