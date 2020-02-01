#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <string>
#include "Texture.h"
#include "core/Assertion.h"
#include "memory/Containers.h"
#include "RenderPass.h"
#include "memory/smart_ptr.h"


#include <rendering/Mesh.h>
#include "rendering/ResourceIds.h"
#include "core/StrHash.h"
#include "core/HashCombine.h"


namespace scene
{
	class SceneNode;
}

namespace ui
{
	class UIText;
	class UIImage;
}

namespace gfx
{
	class IResourceManager;
	class Camera;
	enum class UIWidgetType
	{
		Panel,
		Text,
		Empty,
	};

	struct UIWidget
	{
		UIWidget(UIWidgetType type = UIWidgetType::Empty) : type(type) {}
		virtual ~UIWidget() = default;
	
		UIWidget(const UIWidget& other) = delete;
		UIWidget(UIWidget&& other) noexcept;
		UIWidget& operator=(const UIWidget& other) = delete;
		UIWidget& operator=(UIWidget&& other) noexcept = delete;
	
		const UIWidgetType type;
	
		UIWidget* parent = nullptr;
		ptl::vector<ptl::unique_ptr<UIWidget>> children{};
	
	
		glm::vec2 pos{};
	};
	
	struct UIPanel : public UIWidget
	{
		UIPanel() : UIWidget(UIWidgetType::Panel) {}
	
		glm::vec2 size {};
		float zDepth = 0.0f;
	
		TextureId texture{};
	};
	
	struct UIText : public UIWidget
	{
		enum class HorizontalAlignment
		{
			Leading = 0, Center = 1, Trailing = 2, Justify = 3
		};
		enum class VerticalAlignment
		{
			Top = 0, Center = 1, Bottom = 2
		};
	
		UIText() : UIWidget(UIWidgetType::Text) {}
	
		
		
		
		
	
		glm::vec2 size {};
		float fontSize{};
		float zDepth = 0.0f;
		ptl::string text {};
		HorizontalAlignment textAlignment = HorizontalAlignment::Center;
		VerticalAlignment verticalAlignment = VerticalAlignment::Center;
		glm::vec4 textColor{};
		ptl::string font;
	};

	
	struct UITextOptionsForTexture
	{
		UITextOptionsForTexture() = default;
		 
		 
		 
		 
		 
		 
		 
		 

		friend bool operator==(const UITextOptionsForTexture& lhs, const UITextOptionsForTexture& rhs)
		{
			return lhs.size == rhs.size
				&& lhs.fontSize == rhs.fontSize
				&& lhs.text == rhs.text
				&& lhs.textAlignment == rhs.textAlignment
				&& lhs.verticalAlignment == rhs.verticalAlignment
				&& lhs.font == rhs.font
				&& lhs.color == rhs.color;
		}
		friend bool operator!=(const UITextOptionsForTexture& lhs, const UITextOptionsForTexture& rhs)
		{
			return !(lhs == rhs);
		}

		glm::vec2 size{1,1};
		float fontSize{32.f};
		ptl::string text{};
		UIText::HorizontalAlignment textAlignment = UIText::HorizontalAlignment::Center;
		UIText::VerticalAlignment verticalAlignment = UIText::VerticalAlignment::Center;
		ptl::string font{};
		uint32_t color{ 0xFFFFFFFF };
	};

	
	
	template<bool Const>
	class TUIWidgetPerLayerIterator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = UIWidget * ;
		using difference_type = std::ptrdiff_t;
		using pointer = UIWidget * ;
		using reference = UIWidget & ;
		TUIWidgetPerLayerIterator() = default;
		explicit TUIWidgetPerLayerIterator(UIWidget* root)
		{
			openSet_.push(root);
		}


	private:
		ptl::queue<UIWidget*> openSet_;

	public:
		void swap(TUIWidgetPerLayerIterator& other) noexcept
		{
			std::swap(openSet_, other.openSet_);
		}

		TUIWidgetPerLayerIterator& operator++ () 
		{
			ASSERT_MSG(!openSet_.empty(), "Invalid iterator increment!");
			UIWidget* widget = openSet_.front();
			openSet_.pop();
			for (auto& child : widget->children)
				openSet_.push(child.get());

			return *this;
		}

		TUIWidgetPerLayerIterator operator++ (int) 
		{
			ASSERT(!openSet_.empty() && "Out-of-bounds iterator increment!");
			TUIWidgetPerLayerIterator tmp(*this);
			++(*this);
			return tmp;
		}

		
		bool operator == (const TUIWidgetPerLayerIterator& rhs) const
		{
			return openSet_.size() == rhs.openSet_.size(); 
		}

		bool operator != (const TUIWidgetPerLayerIterator& rhs) const
		{
			return !(openSet_ == rhs.openSet_);
		}

		UIWidget& operator*() const
		{
			ASSERT(!openSet_.empty() && "Invalid iterator dereference!");
			return *openSet_.front();
		}

		pointer operator-> () const
		{
			ASSERT(!openSet_.empty() && "Invalid iterator dereference!");
			return openSet_.front();
		}

		
		operator TUIWidgetPerLayerIterator<true>() const
		{
			return TUIWidgetPerLayerIterator(*this);
		}
	};

	using UIWidgetPerLayerIterator = TUIWidgetPerLayerIterator<false>;
	using ConstUIWidgetPerLayerIterator = TUIWidgetPerLayerIterator<true>;

    
	class IUIRenderer
	{
	public:
		ptl::unique_ptr<UIWidget> root_;

		struct CachedUIImage
		{
			CachedUIImage() = default;

			ui::UIImage* uiImage;
			glm::mat4 modelMatrix;
			ptl::array<gfx::Vertex, 4> vertices;
			static const ptl::array<uint32_t, 6> indices;
			MeshId meshId{}; 
		};
		struct CachedUIText
		{
			CachedUIText() = default;

			ui::UIText* uiText;
			glm::mat4 modelMatrix;
			ptl::array<gfx::Vertex, 4> vertices;
			static const ptl::array<uint32_t, 6> indices;
			MeshId meshId{}; 
		};

	protected:
		ptl::vector<CachedUIImage> m_uiImages{};
		ptl::vector<CachedUIText> m_uiTexts{};

	public:
		IUIRenderer();
		virtual ~IUIRenderer() = 0;

		virtual void Initialize(IResourceManager& gfxResourceManager) = 0;
		virtual void Render(Camera& a_camera) = 0;
		virtual void Shutdown() = 0;

		
		
		
		
		virtual UIWidget* AddEmptyWidget(glm::vec2 pos, UIWidget* parent = nullptr);
		
		
		
		
		
		virtual UIPanel* AddPanel(glm::vec2 pos, glm::vec2 size, gfx::TextureId texture, UIWidget* parent = nullptr);
		
		
		
		
		
		
		
		
		virtual UIText* AddText(glm::vec2 pos, glm::vec2 size, const ptl::string& text,
			UIText::HorizontalAlignment textAlignment, UIText::VerticalAlignment verticalAlignment, float textSize, glm::vec4 textColor, UIWidget* parent);
			

		virtual void AddImage(ui::UIImage* uiImage, glm::mat4 modelMatrix);
		virtual void AddText(ui::UIText* uiText, glm::mat4 modelMatrix);
		virtual auto UpdateImage(ui::UIImage* uiImage, glm::mat4 newModelMatrix) -> CachedUIImage*;
		virtual auto UpdateText(ui::UIText* uiText, glm::mat4 newModelMatrix)->CachedUIText*;
		virtual void RemoveImage(ui::UIImage* uiImage);
		virtual void RemoveText(ui::UIText* uiText);

		
		virtual TextureId CreateTextTexture(const UITextOptionsForTexture& options) = 0;
		virtual void RemoveTextTexture(gfx::TextureId texture) = 0;

	public:
		glm::vec2 m_targetScreenSize{ 1920.f, 1080.f };

		
		
		virtual void RemoveWidget(UIWidget* widget);
		
		
		
		
		
		
		
		
		
		
		
		
		
		

	protected:
		ptl::array<gfx::Vertex, 4> CalculatePanelQuad(glm::vec2 size, float zDepth, glm::mat4 modelMatrix, glm::vec2 minUVs,
			glm::vec3 color = {1.0f, 1.0f, 1.0f},
			glm::vec3 normal = {0.0f, 0.0f, -1.0f}) const;

		auto GetCachedImage(ui::UIImage* image) -> CachedUIImage&;
		auto GetCachedText(ui::UIText* uiText) -> CachedUIText&;
		virtual void RemoveEmptyWidget(UIWidget* widget);
		virtual void RemovePanel(UIPanel* panel);
		virtual void RemoveUIText(UIText* text);
		uint32_t GetDepthOfWidget(UIWidget* widget) const;
		IResourceManager* resourceManager{};
	};
}

namespace std
{
	template<>
	struct hash<::gfx::UIText>
	{
		std::size_t operator()(const ::gfx::UIText& uiText) const noexcept {
			std::size_t seed = 9;

			::core::hash_combine(seed, uiText.text);
			::core::hash_combine(seed, static_cast<size_t>(uiText.textAlignment));
			::core::hash_combine(seed, static_cast<size_t>(uiText.verticalAlignment));
			::core::hash_combine(seed, std::hash<std::string>{}({ uiText.font.c_str() }));
			::core::hash_combine(seed, uiText.fontSize);

			return seed;
		}
	};

	template<>
	struct hash<::gfx::UITextOptionsForTexture>
	{
		std::size_t operator()(const ::gfx::UITextOptionsForTexture& uiText) const noexcept {
			std::size_t seed = 9;

			::core::hash_combine(seed, uiText.text);
			::core::hash_combine(seed, static_cast<size_t>(uiText.textAlignment));
			::core::hash_combine(seed, static_cast<size_t>(uiText.verticalAlignment));
			::core::hash_combine(seed, std::hash<std::string>{}({ uiText.font.c_str() }));
			::core::hash_combine(seed, uiText.fontSize);

			return seed;
		}
	};
}
