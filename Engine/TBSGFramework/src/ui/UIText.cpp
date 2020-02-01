#include "ui/UIText.h"

namespace ui {
	UIText::UIText() : UIElement()
	{
		m_type = UIElementType::Text;

	}

	UIText::UIText(UIElementStructure * data) : UIElement(data)
	{
		m_type = UIElementType::Text;

		if (UITextStructure* textData = static_cast<UITextStructure*>(data)) {
			m_fontSize = textData->m_fontSize;
			text = textData->m_text;
			oldText = text;
			inputEnabled = textData->inputEnabled;
			validation = textData->validation;
			obsecureText = textData->obsecureText;
			m_fontPath = textData->m_fontPath;
			interactableTag = textData->interactableTag;

			m_horizontalAlignment = static_cast<gfx::UIText::HorizontalAlignment>(textData->m_horizontalAlignment);
			m_verticalAlignment = static_cast<gfx::UIText::VerticalAlignment>(textData->m_verticalAlignment);
			SetColor(textData->m_color);

			SetSize(data->m_scale);
		}
	}

	const ptl::string UIText::GetText() const
	{
		ptl::string newText = text;

		if (obsecureText)
		{
			for (auto& c : newText)
			{
				c = '*';
			}
		}
		
		if (barEnabled)
		{
			newText += "|";
		}

		return newText;
	}

	void UIText::SetText(const ptl::string & text)
	{
		this->text = text;
	}

	const bool UIText::Validate() const
	{
		if (validation.empty())
		{
			return true;
		}

		std::regex regx(validation.c_str());
		
		return std::regex_match(GetRawText().c_str(), regx);
	}

	void UIText::Update()
	{
		this->oldText = this->GetText();
	}

	bool UIText::OverlapsWithPoint(const int x, const int y) const
	{
		if (!inputEnabled) return false;

		const glm::vec2& pos = GetGlobalPosition();
		const glm::vec2& size = GetSize();

		if (x < int(pos.x)) return false;
		if (y < int(pos.y)) return false;
		if (x >= pos.x + size.x) return false;
		if (y >= pos.y + size.y) return false;

		return true;
	}

	void UIText::SetBarEnabled(const bool barEnabled)
	{
		this->barEnabled = barEnabled;
	}

	void UIText::OnFocus()
	{
		if (!beenFocussed)
		{
			beenFocussed = true;
			placeHolderText = GetRawText();
			SetText("");
		}
	}

	void UIText::OnFocusLost()
	{
		if (GetRawText().empty())
		{
			SetText(placeHolderText);
			beenFocussed = false;
		}
	}
}
