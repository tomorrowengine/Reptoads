#include <rendering/IUIRenderer.h>
#include "core/Assertion.h"
#include "core/Defines.h"
#include <gameplay/Transform.h>
#include <scene/SceneGraph.h>

#include <ui/UIImage.h>
#include <ui/UIText.h>
#include "rendering/IResourceManager.h"

namespace gfx
{
	UIWidget::UIWidget(UIWidget&& other) noexcept : type{ other.type },
		parent{ other.parent },
		children{ std::move(other.children) },
		pos{ other.pos } {}

	IUIRenderer::IUIRenderer()
		: root_{ ptl::unique_ptr<UIWidget>{ new UIWidget() } }
	{
	}

	void IUIRenderer::Initialize(IResourceManager& gfxResourceManager)
	{
		resourceManager = &gfxResourceManager;
	}


	IUIRenderer::~IUIRenderer() = default;

	const ptl::array<uint32_t, 6> IUIRenderer::CachedUIImage::indices {
		0, 2, 1, 1, 2, 3
	};
	const ptl::array<uint32_t, 6> IUIRenderer::CachedUIText::indices = IUIRenderer::CachedUIImage::indices;

	void IUIRenderer::AddImage(ui::UIImage* uiImage, glm::mat4 modelMatrix)
	{
		auto& tex = resourceManager->GetTexture(uiImage->GetTextureId());
		CachedUIImage cached{};
		cached.uiImage = uiImage;
		cached.modelMatrix = modelMatrix;
		cached.vertices = CalculatePanelQuad(uiImage->GetSize(), uiImage->GetGlobalDepth(), modelMatrix, tex.GetUVMin());
		
		cached.meshId = resourceManager->CreateMesh(cached.vertices.data(), cached.vertices.size(), cached.indices.data(), cached.indices.size(), "UI-IMAGE");
		m_uiImages.push_back(cached);
	}

	void IUIRenderer::AddText(ui::UIText* uiText, glm::mat4 modelMatrix)
	{
		auto& tex = resourceManager->GetTexture(uiText->GetTextTextureId());
		CachedUIText cached{};
		cached.uiText = uiText;
		cached.modelMatrix = modelMatrix;
		cached.vertices = CalculatePanelQuad(uiText->GetSize(), uiText->GetGlobalDepth(), modelMatrix, tex.GetUVMin());
		
		cached.meshId = resourceManager->CreateMesh(cached.vertices.data(), cached.vertices.size(), cached.indices.data(), cached.indices.size(), "UI-TEXT");
		m_uiTexts.push_back(cached);
	}

	auto IUIRenderer::UpdateImage(ui::UIImage* uiImage, glm::mat4 newModelMatrix) -> CachedUIImage*
	{
		auto& tex = resourceManager->GetTexture(uiImage->GetTextureId());
		decltype(m_uiImages)::iterator it = std::find_if(m_uiImages.begin(), m_uiImages.end(), [uiImage](const auto& imagePair) {
			return uiImage == imagePair.uiImage;
		});

		ASSERT(it != m_uiImages.end());
		if(it != m_uiImages.end()) {
			it->modelMatrix = newModelMatrix;
			it->vertices = CalculatePanelQuad(uiImage->GetSize(), uiImage->GetGlobalDepth(), newModelMatrix, tex.GetUVMin());
			return &*it;
		}

		return nullptr;
	}

	auto IUIRenderer::UpdateText(ui::UIText* uiText, glm::mat4 newModelMatrix) -> CachedUIText*
	{
		decltype(m_uiTexts)::iterator it = std::find_if(m_uiTexts.begin(), m_uiTexts.end(), [uiText](const auto& textPair) {
			return uiText == textPair.uiText;
		});

		auto& tex = resourceManager->GetTexture(uiText->GetTextTextureId());
		ASSERT(it != m_uiTexts.end());
		if (it != m_uiTexts.end()) {
			it->modelMatrix = newModelMatrix;
			it->vertices = CalculatePanelQuad(uiText->GetSize(), uiText->GetGlobalDepth(), newModelMatrix, tex.GetUVMin());
			return &*it;
		}

		return nullptr;
	}

	void IUIRenderer::RemoveImage(ui::UIImage* uiImage)
	{
		resourceManager->DeleteMesh(GetCachedImage(uiImage).meshId);
		decltype(m_uiImages)::iterator it = std::find_if(m_uiImages.begin(), m_uiImages.end(), [uiImage](const auto& imagePair) {
			return uiImage == imagePair.uiImage;
		});

		ASSERT(it != m_uiImages.end());
		if (it != m_uiImages.end()) {
			m_uiImages.erase(it);
		}
	}

	void IUIRenderer::RemoveText(ui::UIText* uiText)
	{
		resourceManager->DeleteMesh(GetCachedText(uiText).meshId);
		decltype(m_uiTexts)::iterator it = std::find_if(m_uiTexts.begin(), m_uiTexts.end(), [uiText](const auto& textPair) {
			return uiText == textPair.uiText;
		});

		ASSERT(it != m_uiTexts.end());
		if (it != m_uiTexts.end()) {
			m_uiTexts.erase(it);
		}
	}

	ptl::array<gfx::Vertex, 4> IUIRenderer::CalculatePanelQuad(glm::vec2 size, float zDepth, glm::mat4 modelMatrix, glm::vec2 minUVs, glm::vec3 color, glm::vec3 normal) const
	{
		std::array<gfx::Vertex, 4> vertices{};

		const static glm::vec2 dirs[4]{
			{0.f, 1.f},
			{1.f, 1.f},
			{0.f, 0.f},
			{1.f, 0.f},
		};

		

		glm::vec2 dirToUV(1.f, -1.f);
		for (int i = 0; i < 4; ++i) {
			gfx::Vertex vert{};

			glm::vec2 vertpos = glm::vec2{ modelMatrix * glm::vec4{ dirs[i] * size, 1.0f, 1.0f } };
			glm::vec2 vertuv = (dirs[i] * dirToUV);
			vertuv.x = glm::max(vertuv.x, minUVs.x);
			vertuv.x = glm::max(vertuv.x, minUVs.x);
			vert.pos = glm::vec3(vertpos.x, vertpos.y, zDepth);
			vert.uv = vertuv;
			vert.color = color;
			vert.normal = normal;

			vertices[i] = vert;
		}

		vertices[0].uv = { minUVs.x	, minUVs.y };
		vertices[1].uv = { 1.0f		, minUVs.y };
		vertices[2].uv = { minUVs.x	, 1.0f };
		vertices[3].uv = { 1.0f		, 1.0f };

		return vertices;
	}

	auto IUIRenderer::GetCachedImage(ui::UIImage* image) -> CachedUIImage&
	{
		auto it = std::find_if(m_uiImages.begin(), m_uiImages.end(), [image](const auto& cachedImage) {
			return image == cachedImage.uiImage;
		});
		ASSERT(it != m_uiImages.end());
		return *it;
	}

	auto IUIRenderer::GetCachedText(ui::UIText* uiText) -> CachedUIText&
	{
		auto it = std::find_if(m_uiTexts.begin(), m_uiTexts.end(), [uiText](const auto& cachedImage) {
			return uiText == cachedImage.uiText;
		});
		ASSERT(it != m_uiTexts.end());
		return *it;
	}

	 UIWidget* IUIRenderer::AddEmptyWidget(glm::vec2 pos, UIWidget* parent)
	 {
	 	UIWidget* widget = new UIWidget();
	 	widget->pos = pos;
	 	widget->parent = parent != nullptr ? parent : root_.get();
	 	if (parent != nullptr) {
	 		parent->children.emplace_back(widget);
	 	}
	 	else {
	 		root_->children.emplace_back(widget);
	 	}
	 	return widget;
	 }
	 void IUIRenderer::RemoveEmptyWidget(UIWidget* widget)
	 {
	 	ASSERT_MSG(widget != root_.get(), "ERROR: Trying to remove root node from ui graph!");
	 	ASSERT(widget != nullptr);
	 	auto* parent = widget->parent;
	 	if (parent != nullptr) {
	 		auto it = std::find_if(parent->children.begin(), parent->children.end(), [widget](const ptl::unique_ptr<UIWidget>& node) {
	 			return node.get() == widget;
	 		});
	 		ASSERT_MSG(it != parent->children.end(),
	 			"For some reason, the nodeToRemove has a parent which does not contain this nodeToRemove as a child.");
	 		parent->children.erase(it);
	 	}
	 	else {
	 		delete widget;
	
	
	 	}
	 }
	UIPanel* IUIRenderer::AddPanel(glm::vec2 pos, glm::vec2 size, gfx::TextureId texture, UIWidget* parent)
	{
		UIPanel* panel = new UIPanel();
		panel->pos = pos;
		panel->size = size;
		panel->texture = texture;
		panel->parent = parent != nullptr ? parent : root_.get();
		const auto depthInteger = GetDepthOfWidget(panel);
		panel->zDepth = 0.6f - static_cast<float>(depthInteger) / 300.f;
		if (parent != nullptr) {
			parent->children.emplace_back(panel);
		}
		else {
			root_->children.emplace_back(panel);
		}
		return panel;
	}
	void IUIRenderer::RemovePanel(UIPanel* panel)
	{
		
		
		RemoveEmptyWidget(panel);
	}
	UIText* IUIRenderer::AddText(glm::vec2 pos, glm::vec2 size, const ptl::string& text,
		UIText::HorizontalAlignment textAlignment, UIText::VerticalAlignment verticalAlignment, float textSize, glm::vec4 textColor, UIWidget* parent)
	{
		UIText* panel = new UIText();
		panel->pos = pos;
		panel->size = size;
		panel->parent = parent != nullptr ? parent : root_.get();
		panel->text = text;
		panel->fontSize = textSize;
		panel->textAlignment = textAlignment;
		panel->verticalAlignment = verticalAlignment;
		panel->textColor = textColor;
		auto depthInteger = GetDepthOfWidget(panel);
		panel->zDepth = static_cast<float>(depthInteger) / 20.f;
		if (parent != nullptr) {
			parent->children.emplace_back(panel);
		}
		else {
			root_->children.emplace_back(panel);
		}
		return panel;
	}
	void IUIRenderer::RemoveUIText(UIText* text)
	{
		
		
		RemoveEmptyWidget(text);
	}
	void IUIRenderer::RemoveWidget(UIWidget* widget)
	{
		switch (widget->type) {
		case UIWidgetType::Panel:
			RemovePanel(static_cast<UIPanel*>(widget));
			break;
		case UIWidgetType::Text:
			RemoveUIText(static_cast<UIText*>(widget));
			break;
		case UIWidgetType::Empty:
			RemoveEmptyWidget(widget);
			break;
		default: ASSERT_MSG(false, "Unknown widget type!");
		}
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	uint32_t IUIRenderer::GetDepthOfWidget(UIWidget* widget) const
	{
		UIWidget* parent = widget->parent;
		uint32_t depth = 0;
		while (parent != nullptr && parent != root_.get()) {
			parent = parent->parent;
			++depth;
		}
		return depth;
	}
}
