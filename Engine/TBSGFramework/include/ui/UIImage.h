#pragma once

#include "UIElement.h"

namespace ui {

	struct UIImageStructure : UIElementStructure
	{
		UIImageStructure(ptl::string a_name, ptl::string tag, ptl::vector<UIEventStructure> a_events, glm::vec3 a_position, glm::vec2 a_scale, float a_rotation, ptl::vector<UIElementStructure*> a_children, ptl::string a_textureId, bool a_interactable) :
			UIElementStructure(a_name, tag, a_events, a_position, a_scale, a_rotation, a_children),
			m_textureId(a_textureId), m_interactable(a_interactable)
		{
			m_type = UIElementType::Image;
		}

		ptl::string m_textureId;
		bool m_interactable;

	};

	class UIImage : public UIElement
	{
	public:
		UIImage();
		UIImage(UIElementStructure* data);
		virtual ~UIImage();

		ptl::string GetTextureId() const { return m_textureId; }
		void SetTextureId(ptl::string a_imagePath) { m_textureId = a_imagePath; }

		const glm::vec2& GetSize() const { return m_size; }
		void SetSize(const glm::vec2& a_size) { m_size = a_size; }

		const bool& IsInteractable() const noexcept { return m_interactable; }
		void SetInteractable(bool a_interactable) noexcept { m_interactable = a_interactable; }

	    bool OverlapsWithPoint(const int x, const int y) const override;
	private:
		bool m_interactable;
		ptl::string m_textureId{};
		glm::vec2 m_size{};
		float m_zDepth{};
		bool m_selected{};
	};

}


