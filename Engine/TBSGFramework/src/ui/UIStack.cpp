#include "ui\UIStack.h"

namespace ui {


	UIStack::UIStack(UIStacktructure * data)
	{
		if (data != nullptr) {
			m_name = data->m_name;
		}
	}

	UIStack::~UIStack()
	{
	}

	void UIStack::PushElement(ptl::shared_ptr<UIElement> a_element)
	{
		m_rootElements.push_back(a_element);

	}

	void UIStack::AddElement(ptl::shared_ptr<UIElement>a_element)
	{
		m_rootElements.push_front(a_element);
	}

	void UIStack::PopElement(ptl::weak_ptr<UIElement> a_element)
	{
		auto it = m_rootElements.begin();

		while (it != m_rootElements.end()) {
			auto element = a_element.lock();

			if (ptl::weak_ptr<UIElement>(*it).lock() == element) {
				it = m_rootElements.erase(it);
			}
			else {
				it++;
			}
		}
	}

	ptl::vector<ptl::weak_ptr<UIElement>> UIStack::GetChildElementsOfTag(const ptl::string& tag)
	{
		ptl::vector<ptl::weak_ptr<UIElement>> children;

		auto it = m_rootElements.begin();

		while (it != m_rootElements.end())
		{
			if ((*it)->GetTag().compare(tag) == 0)
			{
				children.push_back(*it);
			}

			ptl::vector<ptl::weak_ptr<UIElement>> childrenOfChildren = (*it)->GetChildElementsOfTag(tag);

			children.insert(children.end(), childrenOfChildren.begin(), childrenOfChildren.end());

			++it;
		}

		return children;
	}

	ptl::weak_ptr<UIElement> UIStack::GetChildElementOfTag(const ptl::string& tag)
	{
		auto childrenWithTag = GetChildElementsOfTag(tag);

		if (childrenWithTag.size() > 0)
		{
			return childrenWithTag[0];
		}

		return ptl::weak_ptr<UIElement>();
	}

	void UIStack::Clear()
	{
	}

}