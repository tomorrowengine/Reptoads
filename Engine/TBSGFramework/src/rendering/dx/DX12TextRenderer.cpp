
#include "rendering/dx/ResourceStateTracker.h"

#include <rendering/dx/DX12TextRenderer.h>
#include "rendering/dx/DX12Renderer.h"
#include <d3dx12.h>
#include "rendering/dx/Helpers.h"
#include <rendering/dx/DX12Application.h>
#include <d3d11.h>
#include <d3d11on12.h>
#include <d3dx12.h>
#include <rendering/dx/CommandQueue.h>
#include <pix_win.h>
#include <d3dcompiler.h>
#include <codecvt>
#include "core/Assertion.h"
#include <core/Config.h>
#include <dwrite_3.h>

#include <ui/UIText.h>
#include <ghc/filesystem.hpp>


namespace gfx
{
	template <class T> static void SafeRelease(T **ppT)
	{
		if (*ppT) {
			(*ppT)->Release();
			*ppT = NULL;
		}
	}

	static uint32_t PackColor(float r, float g, float b, float a)
	{
		uint8_t r255 = static_cast<uint8_t>(glm::clamp(r, 0.0f, 1.0f) * 255.f);
		uint8_t g255 = static_cast<uint8_t>(glm::clamp(g, 0.0f, 1.0f) * 255.f);
		uint8_t b255 = static_cast<uint8_t>(glm::clamp(b, 0.0f, 1.0f) * 255.f);
		uint8_t a255 = static_cast<uint8_t>(glm::clamp(a, 0.0f, 1.0f) * 255.f);

		return r255 | g255 << 8 | b255 << 16 | a255 << 24;
	}

	static std::array<float, 4> UnpackColor(uint32_t rgba)
	{
		float r = static_cast<float>((rgba & 0x000000FF));
		float g = static_cast<float>((rgba & 0x0000FF00) >> 8);
		float b = static_cast<float>((rgba & 0x00FF0000) >> 16);
		float a = static_cast<float>((rgba & 0xFF000000) >> 24);

		return { r, g, b, a };
	}

	static DWRITE_TEXT_ALIGNMENT GetHorizontalTextAlignmentFromComponent(UIText::HorizontalAlignment alignment)
	{
		switch (alignment) {
		case UIText::HorizontalAlignment::Leading: return DWRITE_TEXT_ALIGNMENT_LEADING;
		case UIText::HorizontalAlignment::Center: return DWRITE_TEXT_ALIGNMENT_CENTER;
		case UIText::HorizontalAlignment::Trailing: return DWRITE_TEXT_ALIGNMENT_TRAILING;
		case UIText::HorizontalAlignment::Justify: return DWRITE_TEXT_ALIGNMENT_JUSTIFIED;
		default: throw std::exception();
		}
	}

	static DWRITE_PARAGRAPH_ALIGNMENT GetVerticalTextAlignmentFromComponetn(UIText::VerticalAlignment vertAlignment)
	{
		switch (vertAlignment) {
		case UIText::VerticalAlignment::Top: return DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
		case UIText::VerticalAlignment::Center: return DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		case UIText::VerticalAlignment::Bottom: return DWRITE_PARAGRAPH_ALIGNMENT_FAR;
		default: throw std::exception();
		}
	}

	DX12TextRenderer::DX12TextRenderer(DX12Renderer* renderer)
		: renderer_(renderer)
	{

	}

	
	
	
	void DX12TextRenderer::Initialize()
	{

		auto device = DX12Application::Get().GetDevice();
		

		auto d3d12CommandQueue = DX12Application::Get().GetCommandQueue()->GetD3D12CommandQueue();
		UINT d3d11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG) && !defined(RENDERING_DISABLE_DEBUGLAYER)
		d3d11DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		Microsoft::WRL::ComPtr<ID3D11Device> d3d11Device;
		ThrowIfFailed(D3D11On12CreateDevice(
			device.Get(),
			d3d11DeviceFlags,
			nullptr,
			0,
			reinterpret_cast<IUnknown**>(d3d12CommandQueue.GetAddressOf()),
			1,
			0,
			&d3d11Device,
			&m_d3d11DeviceContext,
			nullptr
		));

		ASSERT(m_d3d11DeviceContext.Get() != nullptr);

		
		ThrowIfFailed(d3d11Device.As(&m_d3d11On12Device));

		
		{
			D2D1_FACTORY_OPTIONS d2dFactoryOptions = {};


			D2D1_DEVICE_CONTEXT_OPTIONS deviceOptions = D2D1_DEVICE_CONTEXT_OPTIONS_NONE;
			ThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3), &d2dFactoryOptions, &m_d2dFactory));
			Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
			ThrowIfFailed(m_d3d11On12Device.As(&dxgiDevice));
			ThrowIfFailed(m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice));
			ThrowIfFailed(m_d2dDevice->CreateDeviceContext(deviceOptions, &m_d2dDeviceContext));
			ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory5), &m_dWriteFactory));
			
			IDWriteFontSetBuilder1* pFontSetBuilder;
			HRESULT hr = m_dWriteFactory->CreateFontSetBuilder(&pFontSetBuilder);
			ThrowIfFailed(hr);

			{
				ptl::string fontPath = "data/fonts/";

				HRESULT loadFontFileResult;
				for (auto& p : ghc::filesystem::recursive_directory_iterator{ fontPath }) {
					if (p.is_regular_file()) {
						IDWriteFontFile* pFontFile;
						loadFontFileResult = m_dWriteFactory->CreateFontFileReference(ptl::transform_to<ptl::wstring>(p.path().string()).c_str(),  nullptr, &pFontFile);
						if (FAILED(loadFontFileResult)) {
							std::cerr << "ERROR: Cannot load font file: '" << p << "' located in fontPath: '" << fontPath << "', HResult: '" << loadFontFileResult << "'" << std::endl;
						}

						loadFontFileResult = pFontSetBuilder->AddFontFile(pFontFile);
						if (FAILED(loadFontFileResult)) {
							std::cerr << "ERROR: Cannot add font file: '" << p << "' located in fontPath: '" << fontPath << "', to the font set. HResult: '" << loadFontFileResult << "'" << std::endl;
						}
					}
				}
			}

			
			
			


			
			

			IDWriteFontSet* sysFontSet;
			hr = m_dWriteFactory->GetSystemFontSet(&sysFontSet);
			ThrowIfFailed(hr);

			pFontSetBuilder->AddFontSet(sysFontSet);

			hr = pFontSetBuilder->CreateFontSet(&fontSet);
			ThrowIfFailed(hr);

			hr = m_dWriteFactory->CreateFontCollectionFromFontSet(fontSet.Get(), &fontCollection);
			ThrowIfFailed(hr);
		}

		
		
		float dpiX;
		float dpiY;
		m_d2dFactory->GetDesktopDpi(&dpiX, &dpiY);
		D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpiX,
			dpiY
		);

		auto thingDesc = renderer_->GetRenderTarget().GetTexture(AttachmentPoint::Color0).GetD3D12Resource()->GetDesc();

		
		
		
		
		
		
		
		
		

		
		
		
		
		
		
		
		

		
		{
			
		}

		ThrowIfFailed(m_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF{ 0, 0.0f }, &m_clearingBrush));
		ThrowIfFailed(m_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF{ D2D1::ColorF::Black }, &blackBrush));

		
#if defined(_DEBUG) && !defined(RENDERING_DISABLE_DEBUGLAYER)
		ThrowIfFailed(device->QueryInterface(IID_PPV_ARGS(&m_infoQueue)));
#endif

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


		CD3DX12_STATIC_SAMPLER_DESC linearRepeatSampler(0, D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR);
		CD3DX12_STATIC_SAMPLER_DESC anisotropicSampler(0, D3D12_FILTER_ANISOTROPIC);

		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDescription;
		rootSignatureDescription.Init_1_1(rootParamNums, rootParameters, 1, &linearRepeatSampler, rootSignatureFlags);

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
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 9, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
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

		auto commandQueue = DX12Application::Get().GetCommandQueue();
		auto commandList = commandQueue->GetCommandList();
		commandList->CopyIndexBuffer(quadIndexBuffer, IUIRenderer::CachedUIText::indices.size(), DXGI_FORMAT_R32_UINT, IUIRenderer::CachedUIText::indices.data());
		auto fence = commandQueue->ExecuteCommandList(commandList);
		commandQueue->WaitForFenceValue(fence);
	}

	void DX12TextRenderer::StartFrame()
	{
		
		
		
		
		
		
		
		
		

		auto viewport = renderer_->GetViewport();
		auto screenSize = renderer_->GetUIRenderer()->m_targetScreenSize;
		glm::mat4 ortho = glm::orthoLH(0.0f, screenSize.x,
			0.0f, screenSize.y,
			viewport.MinDepth, viewport.MaxDepth);

		commandQueue_ = DX12Application::Get().GetCommandQueue();
		commandList_ = commandQueue_->GetCommandList();

		commandList_->SetPipelineState(uiOverlayPipelineState);
		commandList_->SetGraphicsRootSignature(uiOverlayRootSignature);

		commandList_->SetViewport(renderer_->GetViewport());
		commandList_->SetScissorRect(renderer_->GetScissorsRect());

		commandList_->SetRenderTarget(renderer_->GetRenderTarget());

		commandList_->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		struct uiOverlayMatricesCB
		{
			glm::mat4 OrthoViewMat;
		};
		uiOverlayMatricesCB uiMatricesCb{ ortho };

		commandList_->SetGraphicsDynamicConstantBuffer(0, sizeof(uiOverlayMatricesCB), &uiMatricesCb);
	}

	void DX12TextRenderer::RenderUIText(IUIRenderer::CachedUIText& cachedUiText)
	{
		ui::UIText* uiImage = cachedUiText.uiText;
		UITextOptionsForTexture options{
			uiImage->GetSize(),
			uiImage->GetFontSize(),
			uiImage->GetOldText(),
			static_cast<UIText::HorizontalAlignment>(uiImage->GetHorizontalAlignment()),
			static_cast<UIText::VerticalAlignment>(uiImage->GetVerticalAlignment()),
			uiImage->GetFontPath(),
			PackColor(uiImage->GetColor().x, uiImage->GetColor().y, uiImage->GetColor().z, uiImage->GetColor().w)
		};

		auto* textTexture = GetCachedTextTexture(options);
		ASSERT(textTexture != nullptr);

		
		commandList_->SetShaderResourceView(1, 0, *textTexture->dx12Texture, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

		
		commandList_->SetDynamicVertexBuffer(0, cachedUiText.vertices.size(),
			sizeof(gfx::Vertex), cachedUiText.vertices.data());
		commandList_->SetIndexBuffer(quadIndexBuffer);
		
		
		commandList_->DrawIndexed(static_cast<uint32_t>(cachedUiText.indices.size()));

		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
	}

	void DX12TextRenderer::EndFrame()
	{
		auto fence = commandQueue_->ExecuteCommandList(commandList_);
		commandQueue_->WaitForFenceValue(fence);
		commandQueue_.reset();
		commandList_.reset();

		
		
		
		
		
		
		
		
		
		
		
	}

	void DX12TextRenderer::Shutdown()
	{

	}

	auto DX12TextRenderer::CreateTextureFromText(const gfx::UIText& uiText) -> TextTexture&
	{
		UNUSED(uiText);

		UITextOptionsForTexture options{};
		return CreateTextureFromText(options);
	}

	auto DX12TextRenderer::CreateTextureFromText(const ui::UIText* uiText) -> TextTexture&
	{
		UITextOptionsForTexture options{
			uiText->GetSize(),
			uiText->GetFontSize(),
			uiText->GetOldText(),
			static_cast<UIText::HorizontalAlignment>(uiText->GetHorizontalAlignment()),
			static_cast<UIText::VerticalAlignment>(uiText->GetVerticalAlignment()),
			uiText->GetFontPath(),
			PackColor(uiText->GetColor().x, uiText->GetColor().y, uiText->GetColor().z, uiText->GetColor().w)
		};
		return CreateTextureFromText(options);
	}


	auto DX12TextRenderer::CreateTextureFromText(const UITextOptionsForTexture& uiText) -> TextTexture&
	{
		if (!tbsg::Config::Get().enableTextRendering) {
			static bool initialized = false;
			static Texture emptyTexture;
			static TextTexture textTexture;
			if (!initialized) {
				TextureId emptyTextureId = renderer_->GetGfxResourceManager().GetDefaultTextureId();
				emptyTexture = renderer_->GetGfxResourceManager().GetDefaultTexture();
				textTexture = TextTexture{ &emptyTexture, emptyTextureId, {}, {} };
				initialized = true;

			}
			return textTexture;
		}

		using namespace std::string_literals;

		static uint32_t counter = 0;

		auto size = uiText.size;

		ptl::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		ptl::wstring wideId = converter.from_bytes("UIText texture:'"s + uiText.text + "'");

		
		D3D12_RESOURCE_DESC backbufferDesc = renderer_->GetRenderTarget().GetTexture(AttachmentPoint::Color0).GetD3D12Resource()->GetDesc();
		D3D12_RESOURCE_DESC textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(backbufferDesc.Format, static_cast<UINT64>(size.x), static_cast<UINT>(size.y), 1, 1, 1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, D3D12_TEXTURE_LAYOUT_UNKNOWN, 65536);


		auto device = DX12Application::Get().GetDevice();
		Microsoft::WRL::ComPtr<ID3D12Resource> textureResource;

		CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_DEFAULT);
		ThrowIfFailed(device->CreateCommittedResource(&heapProperties,
			D3D12_HEAP_FLAG_NONE ,
			&textureDesc,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			nullptr,
			IID_PPV_ARGS(&textureResource)));

		
		ResourceStateTracker::AddGlobalResourceState(textureResource.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET);

		Texture texture{};
		texture.SetTextureUsage(TextureUsage::RenderTarget);
		texture.SetD3D12Resource(textureResource);
		texture.CreateViews();
		texture.SetName(wideId);

		Microsoft::WRL::ComPtr<ID3D11Resource> wrappedResource;

		
		
		float dpiX;
		float dpiY;
		m_d2dFactory->GetDesktopDpi(&dpiX, &dpiY);
		D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpiX,
			dpiY
		);

		D3D11_RESOURCE_FLAGS d3d11Flags = { };
		d3d11Flags.BindFlags = D3D11_BIND_RENDER_TARGET;
		
		ThrowIfFailed(m_d3d11On12Device->CreateWrappedResource(
			textureResource.Get(),
			&d3d11Flags,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			IID_PPV_ARGS(&wrappedResource)
		));

		Microsoft::WRL::ComPtr<ID2D1Bitmap1> d2dRenderTarget;
		
		Microsoft::WRL::ComPtr<IDXGISurface> surface;
		ThrowIfFailed(wrappedResource.As(&surface));
		ThrowIfFailed(m_d2dDeviceContext->CreateBitmapFromDxgiSurface(
			surface.Get(),
			&bitmapProperties,
			&d2dRenderTarget
		));

		
		PushD2DWarningFilter();

		
		m_d3d11On12Device->AcquireWrappedResources(wrappedResource.GetAddressOf(), 1);

		
		m_d2dDeviceContext->SetTarget(d2dRenderTarget.Get());
		m_d2dDeviceContext->BeginDraw();
		m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
		m_d2dDeviceContext->FillRectangle(D2D1_RECT_F{ 0,0,uiText.size.x,uiText.size.y }, m_clearingBrush.Get());

		
		D2D1_RECT_F d2dRect = { 0, 0, uiText.size.x, uiText.size.y };

		std::wstring wide = converter.from_bytes(uiText.text.c_str());

		auto textFormat = GetTextFormat(uiText);
		auto textBrush = GetColorBrush(uiText);
		auto textColor = textBrush->GetColor();

		if (glm::abs(textColor.r - 1.0f) < 0.05f &&
			glm::abs(textColor.g - 1.0f) < 0.05f &&
			glm::abs(textColor.b - 1.0f) < 0.05f &&
			glm::abs(textColor.a - 1.0f) < 0.05f) {
			const int outlineSize = 2;
			d2dRect = { outlineSize, outlineSize, uiText.size.x, uiText.size.y };

			m_d2dDeviceContext->DrawText(
				wide.c_str(),
				static_cast<UINT32>(uiText.text.size()),
				textFormat.Get(),
				&d2dRect,
				blackBrush.Get()
			);
			d2dRect = { outlineSize, -outlineSize, uiText.size.x, uiText.size.y };

			m_d2dDeviceContext->DrawText(
				wide.c_str(),
				static_cast<UINT32>(uiText.text.size()),
				textFormat.Get(),
				&d2dRect,
				blackBrush.Get()
			);
			d2dRect = { -outlineSize, outlineSize, uiText.size.x, uiText.size.y };

			m_d2dDeviceContext->DrawText(
				wide.c_str(),
				static_cast<UINT32>(uiText.text.size()),
				textFormat.Get(),
				&d2dRect,
				blackBrush.Get()
			);
			d2dRect = { -outlineSize, -outlineSize, uiText.size.x, uiText.size.y };

			m_d2dDeviceContext->DrawText(
				wide.c_str(),
				static_cast<UINT32>(uiText.text.size()),
				textFormat.Get(),
				&d2dRect,
				blackBrush.Get()
			);
		}


		m_d2dDeviceContext->DrawText(
			wide.c_str(),
			static_cast<UINT32>(uiText.text.size()),
			textFormat.Get(),
			&d2dRect,
			textBrush.Get()
		);

		ThrowIfFailed(m_d2dDeviceContext->EndDraw());

		
		
		
		m_d3d11On12Device->ReleaseWrappedResources(wrappedResource.GetAddressOf(), 1);

		
		m_d3d11DeviceContext->Flush();

		PopD2DWarningFiler();

		ptl::string textureId = ptl::string{ "Text texture #" } +std::to_string(++counter);
		
		
		
		
		auto placedTextTextureIt = textCache_.find(uiText);
		if (placedTextTextureIt != textCache_.end()) {
			auto& placedTextTexture = placedTextTextureIt->second.value();
			auto texId = placedTextTexture.textureId;
			textureHandleToCache_.emplace(texId, uiText);
			return placedTextTexture;
		}
		else {
			TextureId placedTextureId = renderer_->GetGfxResourceManager().AddTexture(&Texture{ textureResource });
			Texture& placedTexture = renderer_->GetGfxResourceManager().GetTexture(placedTextureId);
			TextTexture textTexture{ &placedTexture, placedTextureId, wrappedResource, d2dRenderTarget };
			textCache_.emplace(uiText, textTexture);
			return textCache_.at(uiText).value();
		}
	}

	void DX12TextRenderer::RemoveTextTexture(gfx::TextureId textureId)
	{
		auto it = textureHandleToCache_.find(textureId);
		if (it == textureHandleToCache_.end()) {
			ASSERT_MSG(false,
				"Cannot find texture handle in internal map! It probably got deleted already!");
			return;
		}

		auto textCache_it = textCache_.find(it->second);
		if (textCache_it != textCache_.end()) {
			auto& ref_texture = textCache_it->second;

			ASSERT(ref_texture.ref_count() > 0);
			ref_texture.decrease_ref_count();
			if (ref_texture.ref_count() <= 0) {
				renderer_->GetGfxResourceManager().DeleteTexture(textCache_it->second.value().textureId);
				textCache_.erase(textCache_.find(it->second));
				textureHandleToCache_.erase(it);
			}
		}
	}

	void DX12TextRenderer::AddUIText(ui::UIText* uiText)
	{
		UITextOptionsForTexture options{
			uiText->GetSize(),
			uiText->GetFontSize(),
			uiText->GetOldText(),
			static_cast<UIText::HorizontalAlignment>(uiText->GetHorizontalAlignment()),
			static_cast<UIText::VerticalAlignment>(uiText->GetVerticalAlignment()),
			uiText->GetFontPath(),
			PackColor(uiText->GetColor().x, uiText->GetColor().y, uiText->GetColor().z, uiText->GetColor().w)
		};
		auto it = textCache_.find(options);
		if (it == textCache_.end()) {
			TextTexture tt = CreateTextureFromText(options);
			textCache_.insert({ options, ref_counted<TextTexture>{tt} });
			textureHandleToCache_.insert({ textCache_[options].value().textureId, options });
		}
		else {
			it->second.increase_ref_count();
		}

		
		uiText->SetTextTextureId(textCache_[options].value().textureId);
	}

	void DX12TextRenderer::RemoveUIText(ui::UIText& uiText)
	{
		UITextOptionsForTexture options{
			uiText.GetSize(),
			uiText.GetFontSize(),
			uiText.GetOldText(),
			static_cast<UIText::HorizontalAlignment>(uiText.GetHorizontalAlignment()),
			static_cast<UIText::VerticalAlignment>(uiText.GetVerticalAlignment()),
			uiText.GetFontPath(),
			PackColor(uiText.GetColor().x, uiText.GetColor().y, uiText.GetColor().z, uiText.GetColor().w)
		};

		auto it = textCache_.find(options);
		ASSERT(tbsg::Config::Get().enableTextRendering ? it != textCache_.end() : true);
		if (it != textCache_.end()) {
			
			
			
			
			
			RemoveTextTexture(it->second.value().textureId);
		}
	}

	DX12TextRenderer::TextTexture* DX12TextRenderer::GetCachedTextTexture(const UITextOptionsForTexture& options)
	{
		auto it = textCache_.find(options);
		if (it == textCache_.end())
			return nullptr;

		return &it->second.value();
	}

	Microsoft::WRL::ComPtr<IDWriteTextFormat> DX12TextRenderer::GetTextFormat(const UITextOptionsForTexture& options)
	{
		TextFormatOptions formatOptions{ };
		formatOptions.fontFamaly = ptl::transform_to<std::wstring>(options.font);
		formatOptions.fontSize = static_cast<int>(std::round( options.fontSize ));
		formatOptions.horizontalAlignment = GetHorizontalTextAlignmentFromComponent(options.textAlignment);
		formatOptions.verticalAlignment = GetVerticalTextAlignmentFromComponetn(options.verticalAlignment);

		auto it = textFormatCache.find(formatOptions);
		if(it == textFormatCache.end()) {
			textFormatCache[formatOptions] = CreateTextFormat(formatOptions);
		}

		return textFormatCache[formatOptions];
	}

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> DX12TextRenderer::GetColorBrush(const UITextOptionsForTexture& options)
	{
		ColorBrushOptions formatOptions;

		auto float4s = UnpackColor(options.color);
		formatOptions.color.r = float4s[0];
		formatOptions.color.g = float4s[1];
		formatOptions.color.b = float4s[2];
		formatOptions.color.a = float4s[3];
		
		auto it = colorBrushCache.find(formatOptions);
		if (it == colorBrushCache.end()) {
			colorBrushCache[formatOptions] = CreateColorBrush(formatOptions);
		}

		return colorBrushCache[formatOptions];
	}

	Microsoft::WRL::ComPtr<IDWriteTextFormat> DX12TextRenderer::CreateTextFormat(
		const TextFormatOptions& textFormatOptions)
	{
		Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat;

		ThrowIfFailed(m_dWriteFactory->CreateTextFormat(
			textFormatOptions.fontFamaly.c_str(),
			fontCollection.Get(),
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			textFormatOptions.fontSize,
			L"en-us",
			&textFormat
		));
		ThrowIfFailed(textFormat->SetTextAlignment(textFormatOptions.horizontalAlignment));
		ThrowIfFailed(textFormat->SetParagraphAlignment(textFormatOptions.verticalAlignment));

		return textFormat;
	}

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> DX12TextRenderer::CreateColorBrush(
		const ColorBrushOptions& textFormatOptions)
	{
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> textBrush;
		ThrowIfFailed(m_d2dDeviceContext->CreateSolidColorBrush(textFormatOptions.color, &textBrush));
		return textBrush;
	}

	void DX12TextRenderer::PushD2DWarningFilter()
	{
#if defined(_DEBUG) && !defined(RENDERING_DISABLE_DEBUGLAYER)
		D3D12_MESSAGE_SEVERITY severities[] =
		{
			D3D12_MESSAGE_SEVERITY_INFO,
		};

		
		D3D12_MESSAGE_ID denyIds[] =
		{
			
			D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_DEPTHSTENCILVIEW_NOT_SET,
		};

		D3D12_INFO_QUEUE_FILTER filter = {};
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;

		ThrowIfFailed(m_infoQueue->PushStorageFilter(&filter));
#endif
	}

	void DX12TextRenderer::PopD2DWarningFiler()
	{
#if defined(_DEBUG) && !defined(RENDERING_DISABLE_DEBUGLAYER)
		m_infoQueue->PopStorageFilter();
#endif
	}
}
