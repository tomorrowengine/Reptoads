#pragma once

#include "UIElement.h"
#include "rendering/IUIRenderer.h"
#include <regex>

namespace ui {

	struct UITextStructure : UIElementStructure
	{
		UITextStructure(ptl::string a_name, ptl::string tag, ptl::vector<UIEventStructure> a_events, glm::vec3 a_position, glm::vec2 a_scale, float a_rotation, ptl::vector<UIElementStructure*> a_children,
			int a_fontSize, ptl::string a_text, ptl::string a_fontPath, int a_horizontalAlignment, int a_veticalAlignment, glm::vec4 a_color, bool inputEnabled, ptl::string validation, bool obsecureText, ptl::string interactableTag) : 
			UIElementStructure(a_name, tag, a_events, a_position, a_scale, a_rotation, a_children),
			m_fontSize(a_fontSize),
			m_text(a_text),
			m_fontPath(a_fontPath),
			m_horizontalAlignment(a_horizontalAlignment),
			m_verticalAlignment(a_veticalAlignment),
			m_color(a_color)
		{
			m_type = UIElementType::Text;
			this->inputEnabled = inputEnabled;
			this->validation = validation;
			this->obsecureText = obsecureText;
			this->interactableTag = interactableTag;
		}

		int m_fontSize{};
		ptl::string m_text{};
		ptl::string m_fontPath{};
		ptl::string validation{};
		ptl::string interactableTag{};

		int m_horizontalAlignment{};
		int m_verticalAlignment{};
		glm::vec4 m_color{};

		bool inputEnabled{};
		bool obsecureText{};
	};

	class UIText : public UIElement
	{

	public:
		UIText();
		UIText(UIElementStructure* data);
		virtual ~UIText() = default;

		const ptl::string& GetFontPath() const { return m_fontPath; }
		void SetFontPath(const ptl::string& newFontPath) { m_fontPath = newFontPath; }
		const ptl::string GetText() const;
		const ptl::string& GetRawText() const { return text; };

		void SetText(const ptl::string& text);

		const ptl::string& GetOldText() const { return oldText; }
		const ptl::string& GetValidation() const { return validation; }
		const ptl::string& GetInteractableTag() const { return interactableTag; }

		const bool InputEnabled() const { return inputEnabled; }
		const bool ObsecureText() const { return obsecureText; }
		const bool Validate() const;

		const glm::vec2& GetSize() const { return m_size; }
		void SetSize(const glm::vec2& a_size) { m_size = a_size; }

		const glm::vec4& GetColor() const { return m_color; }
		void SetColor(const glm::vec4& a_color) { m_color = a_color; }

		const int& GetFontSize() const { return m_fontSize; }
		void SetFontSize(const int a_fontSize) { m_fontSize = a_fontSize; }
		
		const gfx::UIText::HorizontalAlignment& GetHorizontalAlignment() const { return m_horizontalAlignment; }
		void SetHorizontalAlignment(const  gfx::UIText::HorizontalAlignment a_horizontalAlignment) { m_horizontalAlignment = a_horizontalAlignment; }

		const  gfx::UIText::VerticalAlignment& GetVerticalAlignment() const { return m_verticalAlignment; }
		void SetVerticalAlignment(const  gfx::UIText::VerticalAlignment a_verticalAlignment) { m_verticalAlignment = a_verticalAlignment; }

		void SetTextTextureId(const gfx::TextureId& textreId) { textTextureId = textreId; }
		gfx::TextureId GetTextTextureId() const { return textTextureId; }

		void Update();

		bool OverlapsWithPoint(const int x, const int y) const override;

		const bool GetBarEnabled() const { return barEnabled; }
		void SetBarEnabled(const bool barEnabled);

		void OnFocus();
		void OnFocusLost();
	private:
		ptl::string text{};
		ptl::string oldText{};
		ptl::string m_fontPath{};
		ptl::string validation{};
		ptl::string interactableTag{};
		ptl::string placeHolderText{};

		int m_fontSize{};
		glm::vec2 m_size{};
		float m_zDepth{};
		glm::vec4 m_color{};
		gfx::UIText::HorizontalAlignment m_horizontalAlignment{};
		gfx::UIText::VerticalAlignment m_verticalAlignment{};
		gfx::TextureId textTextureId;
		bool inputEnabled{};
		bool obsecureText{};
		bool barEnabled{};
		bool beenFocussed{};
		
	};

}
