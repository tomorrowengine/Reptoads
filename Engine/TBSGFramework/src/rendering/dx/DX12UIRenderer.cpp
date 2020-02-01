#ifdef PLATFORM_WINDOWS
#include "memory/smart_ptr.h"
#include <rendering/dx/DX12UIRenderer.h>
#include "core/Assertion.h"
#include "core/Defines.h"
#include "core/config.h"
#include <ui/UIText.h>
#include <ui/UIImage.h>

namespace gfx
{
	DX12UIRenderer::DX12UIRenderer(DX12Renderer* renderer)
		: renderer_(renderer), uiPanelRenderer_(renderer), textRenderer_(renderer) {}


    void DX12UIRenderer::Initialize(IResourceManager& gfxResourceManager)
	{
		resourceManager = &gfxResourceManager;
		uiPanelRenderer_.Initialize();
        if(tbsg::Config::Get().enableTextRendering)
        {
		    textRenderer_.Initialize();
        }
	}

	void DX12UIRenderer::Render(Camera& camera)
	{
		UNUSED(camera);

		ptl::vector<std::pair<float, std::pair<bool, void*>>> orderedUI;
		for (CachedUIImage& cachedUiImage : m_uiImages)
			if(cachedUiImage.uiImage->IsVisible())
				orderedUI.push_back({ cachedUiImage.uiImage->GetGlobalDepth(), {true, &cachedUiImage} });
		for(CachedUIText& cachedUiText : m_uiTexts)
			if (cachedUiText.uiText->IsVisible())
				orderedUI.push_back({ cachedUiText.uiText->GetGlobalDepth(), {false, &cachedUiText} });


		std::sort(orderedUI.begin(), orderedUI.end());
		if(!orderedUI.empty()) {
			uiPanelRenderer_.StartFrame();
			for (auto& zDepthAndUiImageOrText : orderedUI) {
				if(zDepthAndUiImageOrText.second.first) {
					auto* uiElement = static_cast<CachedUIImage*>(zDepthAndUiImageOrText.second.second);
					uiPanelRenderer_.RenderPanel(uiElement);
				}else {
					auto* uiElement = static_cast<CachedUIText*>(zDepthAndUiImageOrText.second.second);
					uiPanelRenderer_.RenderPanel(uiElement);
				}
			}
			uiPanelRenderer_.EndFrame();
		}

		
		
		
		
		
		
		
		
		
		
		
		
		
  
		
  
		
  
		
		
		
		
		
  
  
  
  
		
		
		
  
		
		
  
  
  
  
  
  
  
  
  
  
		
		
		
		
  
		
		
  
		
		
  
  
  
	}

	void DX12UIRenderer::Shutdown()
	{
        if (tbsg::Config::Get().enableTextRendering)
        {
            textRenderer_.Shutdown();
        }
		uiPanelRenderer_.Shutdown();
	}

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

	TextureId DX12UIRenderer::CreateTextTexture(const UITextOptionsForTexture& options)
	{
		auto& textTextures = textRenderer_.CreateTextureFromText(options);
		return textTextures.textureId;
	}

	void DX12UIRenderer::RemoveTextTexture(gfx::TextureId textureId)
	{
		textRenderer_.RemoveTextTexture(textureId);
	}

	void DX12UIRenderer::AddImage(ui::UIImage* uiImage, glm::mat4 modelMatrix)
	{
		IUIRenderer::AddImage(uiImage, modelMatrix);
		auto& cached = IUIRenderer::GetCachedImage(uiImage);
		uiPanelRenderer_.AddImage(cached);
	}

	void DX12UIRenderer::AddText(ui::UIText* uiText, glm::mat4 modelMatrix)
	{
		textRenderer_.AddUIText(uiText);
		IUIRenderer::AddText(uiText, modelMatrix);
	}

	auto DX12UIRenderer::UpdateImage(ui::UIImage* uiImage, glm::mat4 newModelMatrix)->CachedUIImage*
	{
		auto ptr = IUIRenderer::UpdateImage(uiImage, newModelMatrix);
		auto& cached = IUIRenderer::GetCachedImage(uiImage);
		uiPanelRenderer_.UpdateImage(cached);
		return ptr;
	}

	auto DX12UIRenderer::UpdateText(ui::UIText* uiText, glm::mat4 newModelMatrix)->CachedUIText*
	{
		textRenderer_.AddUIText(uiText);
		textRenderer_.RemoveUIText(*uiText);
		auto ptr = IUIRenderer::UpdateText(uiText, newModelMatrix);
		return ptr;
	}

	void DX12UIRenderer::RemoveImage(ui::UIImage* uiImage)
	{
		auto& cached = IUIRenderer::GetCachedImage(uiImage);
		uiPanelRenderer_.RemoveImage(*cached.uiImage);

		IUIRenderer::RemoveImage(uiImage);
	}

	void DX12UIRenderer::RemoveText(ui::UIText* uiText)
	{
		textRenderer_.RemoveUIText(*uiText);

		IUIRenderer::RemoveText(uiText);
	}

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

	
	
	
	
	
	
	
	
	
	
	













}
#endif
