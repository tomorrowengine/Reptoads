#pragma once

#include "ui/UIElement.h"
#include "memory/Containers.h"
#include "memory/String.h"

namespace ui {
	struct UIStacktructure
	{
		UIStacktructure(ptl::string a_name, ptl::vector<UIElementStructure*> a_elements) : m_name(a_name), m_elements(a_elements) {}
		virtual ~UIStacktructure() {
			for (auto& it : m_elements) {
				delete it;
			}

			m_elements.clear();
		}

		ptl::string m_name;
		ptl::vector<UIElementStructure*> m_elements;
	}; 

	class UIStack
	{
	public:
		UIStack() = default;
		UIStack(UIStacktructure* data);
		~UIStack();

		void PushElement(ptl::shared_ptr<UIElement> a_element);
		void AddElement(ptl::shared_ptr<UIElement> a_element);
		void PopElement(ptl::weak_ptr<UIElement> a_element);
		void Clear();

		ptl::vector<ptl::weak_ptr<UIElement>> GetChildElementsOfTag(const ptl::string& tag);
		ptl::weak_ptr<UIElement> GetChildElementOfTag(const ptl::string& tag);

		ptl::deque<ptl::weak_ptr<UIElement>> GetElements() {
			return ptl::deque<ptl::weak_ptr<UIElement>>(m_rootElements.begin(), m_rootElements.end());
		}
	private:

		ptl::string m_name{};
		ptl::deque<ptl::shared_ptr<UIElement>> m_rootElements{};
	};
}
