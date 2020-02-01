#pragma once
#include "rendering/ResourceIds.h"

#ifdef PLATFORM_WINDOWS
#include <glm/vec2.hpp>
#include <unordered_map>
#include "rendering/Texture.h"
#include "core/RefCounted.h"
#include "IndexBuffer.h"
#include "CommandQueue.h"
#include <d2d1_3.h>
#include <dwrite_3.h>
#include <d3d11.h>
#include <d3d11on12.h>
#include <wrl.h>
#include <rendering/IUIRenderer.h>
#include <rendering/dx/Helpers.h>
#include "RootSignature.h"


namespace gfx
{
	struct TextFormatOptions
	{
		std::wstring fontFamaly;
		int fontSize;
		DWRITE_TEXT_ALIGNMENT horizontalAlignment;
		DWRITE_PARAGRAPH_ALIGNMENT verticalAlignment;

		friend bool operator==(const TextFormatOptions& lhs, const TextFormatOptions& rhs)
		{
			return lhs.fontFamaly == rhs.fontFamaly
				&& lhs.fontSize == rhs.fontSize
				&& lhs.horizontalAlignment == rhs.horizontalAlignment
				&& lhs.verticalAlignment == rhs.verticalAlignment;
		}

		friend bool operator!=(const TextFormatOptions& lhs, const TextFormatOptions& rhs) { return !(lhs == rhs); }
	};
	struct ColorBrushOptions
	{
		D2D1::ColorF color;

		ColorBrushOptions()
			: color(D2D1::ColorF::Black) {}
		explicit ColorBrushOptions(const D2D1::ColorF& color)
			: color(color) {}

		friend bool operator==(const ColorBrushOptions& lhs, const ColorBrushOptions& rhs)
		{
			return abs(lhs.color.r - rhs.color.r) < 0.05f &&
				abs(lhs.color.g - rhs.color.g) < 0.05f &&
				abs(lhs.color.b - rhs.color.b) < 0.05f &&
				abs(lhs.color.a - rhs.color.a) < 0.05f;
		}

		friend bool operator!=(const ColorBrushOptions& lhs, const ColorBrushOptions& rhs) { return !(lhs == rhs); }
	};
}

namespace std
{
	template<>
	struct hash<gfx::TextFormatOptions>
	{
		size_t operator()(const gfx::TextFormatOptions& options) const noexcept
		{
			size_t seed = 1233210;

			std::hash_combine(seed, options.fontFamaly);
			std::hash_combine(seed, options.fontSize);
			std::hash_combine(seed, options.horizontalAlignment);
			std::hash_combine(seed, options.verticalAlignment);

			return seed;
		}
	};
	template<>
	struct hash<gfx::ColorBrushOptions>
	{
		size_t operator()(const gfx::ColorBrushOptions& options) const noexcept
		{
			size_t seed = 65534;

			std::hash_combine(seed, options.color.r);
			std::hash_combine(seed, options.color.g);
			std::hash_combine(seed, options.color.b);
			std::hash_combine(seed, options.color.a);

			return seed;
		}
	};
}

namespace gfx
{
	class DX12Renderer;

	class DX12TextRenderer
	{


		
		struct TextTexture
		{
			gfx::Texture* dx12Texture;
			TextureId textureId;
			Microsoft::WRL::ComPtr<ID3D11Resource> wrappedResource;
			Microsoft::WRL::ComPtr<ID2D1Bitmap1> d2dRenderTarget;
		};

		Microsoft::WRL::ComPtr<IDWriteFactory5> m_dWriteFactory;

		const wchar_t* wszText = L"Hello World!";
		UINT32 cTextLength = (UINT32)wcslen(wszText);

		Microsoft::WRL::ComPtr<ID2D1Factory3> m_d2dFactory;
		
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_clearingBrush;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> blackBrush;
		
		Microsoft::WRL::ComPtr<ID2D1Device2> m_d2dDevice;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext2> m_d2dDeviceContext;
		Microsoft::WRL::ComPtr<ID3D11Resource> m_wrappedColorBuffer;
		Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_d2dRenderTarget;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3d11DeviceContext;
		Microsoft::WRL::ComPtr<ID3D11On12Device> m_d3d11On12Device;

		Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
		Microsoft::WRL::ComPtr<IDWriteFontSet> fontSet;
		Microsoft::WRL::ComPtr<IDWriteFontCollection1> fontCollection;

		Microsoft::WRL::ComPtr<ID3D12InfoQueue> m_infoQueue;

		std::shared_ptr<CommandQueue> commandQueue_;
		std::shared_ptr<CommandList> commandList_;

		RootSignature uiOverlayRootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> uiOverlayPipelineState;

		IndexBuffer quadIndexBuffer;
		std::unordered_map<UITextOptionsForTexture, ref_counted<TextTexture>> textCache_;
		std::unordered_multimap<gfx::TextureId, UITextOptionsForTexture> textureHandleToCache_;

		std::unordered_map<TextFormatOptions, Microsoft::WRL::ComPtr<IDWriteTextFormat>> textFormatCache;
		std::unordered_map<ColorBrushOptions, Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>> colorBrushCache;

		DX12Renderer* renderer_;


	public:
		DX12TextRenderer(DX12Renderer* renderer);

		void Initialize();

		void StartFrame();
		void RenderUIText(IUIRenderer::CachedUIText& cachedUiText);
		void EndFrame();

		void Shutdown();

	public:
		TextTexture& CreateTextureFromText(const gfx::UIText& uiText);
		TextTexture& CreateTextureFromText(const ui::UIText* uiText);
		TextTexture& CreateTextureFromText(const UITextOptionsForTexture& uiText);
		void RemoveTextTexture(gfx::TextureId textureId);

		void AddUIText(ui::UIText* uiText);
		void RemoveUIText(ui::UIText& uiText);

		TextTexture* GetCachedTextTexture(const UITextOptionsForTexture& options);

	private:
		Microsoft::WRL::ComPtr<IDWriteTextFormat> GetTextFormat(const UITextOptionsForTexture& options);
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> GetColorBrush(const UITextOptionsForTexture& options);

		Microsoft::WRL::ComPtr<IDWriteTextFormat> CreateTextFormat(const TextFormatOptions& textFormatOptions);
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> CreateColorBrush(const ColorBrushOptions& textFormatOptions);

		void PushD2DWarningFilter();
		void PopD2DWarningFiler();
	};
}

#endif
