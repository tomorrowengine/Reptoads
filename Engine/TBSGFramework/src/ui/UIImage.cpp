#include "ui/UIImage.h"

namespace ui {
	UIImage::UIImage() : UIElement()
	{
		m_type = UIElementType::Image;
	}

	UIImage::UIImage(UIElementStructure * data) : UIElement(data)
	{

		m_type = UIElementType::Image;

		if (UIImageStructure* imageData = static_cast<UIImageStructure*>(data)) {
			m_textureId = imageData->m_textureId;
			m_interactable = imageData->m_interactable;
			SetSize(data->m_scale);
		}
	}


	UIImage::~UIImage()
	{
	}

	bool UIImage::OverlapsWithPoint(const int x, const int y) const
	{
		if (!IsInteractable()) return false;

		const glm::vec2& pos = GetGlobalPosition();
		const glm::vec2& size = GetSize();

		if (x < int(pos.x)) return false;
		if (y < int(pos.y)) return false;
		if (x >= pos.x + size.x) return false;
		if (y >= pos.y + size.y) return false;

		return true;
	}
}
