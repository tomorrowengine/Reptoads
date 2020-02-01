#include "ui/UIElement.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <algorithm>

namespace ui
{
	UIElement::UIElement()
	{
	}

	UIElement::UIElement(UIElementStructure * data)
	{
		m_type = UIElementType::Element;

		if (data != nullptr)
		{
			m_name = data->m_name;
			tag = data->tag;

			for (auto e : data->m_events)
			{
				m_events.push_back(UIEvent{
					e.m_type,
					e.m_action,
					e.m_data
					});
			}
			m_rotation = data->m_rotation;
			SetPosition(glm::vec2(data->m_position.x, data->m_position.y));
			initialDepth = data->m_position.z;
		}
	}


	UIElement::~UIElement()
	{
	}

	const glm::vec2 UIElement::GetGlobalPosition() const
	{
		auto mm = GetModelMatrix();

		glm::vec3 sc, t, sk;
		glm::vec4 p;
		glm::quat o;

		glm::decompose(mm, sc, o, t, sk, p);

		return glm::vec2(t.x, t.y);
	}

	const ptl::string & UIElement::GetName() const
	{
		return m_name;
	}

	void UIElement::SetName(const ptl::string & name)
	{
		m_name = name;
	}

	const ptl::string & UIElement::GetTag() const noexcept
	{
		return this->tag;
	}

	void UIElement::SetTag(const ptl::string & tag)
	{
		this->tag = tag;
	}

	const float UIElement::GetGlobalRotation() const
	{
		auto mm = GetModelMatrix();

		glm::vec3 sc, t, sk;
		glm::vec4 p;
		glm::quat o;

		glm::decompose(mm, sc, o, t, sk, p);
		glm::vec3 euler = glm::eulerAngles(o) * 3.14159f / 180.f;
		return euler.z;
	}

	const float UIElement::GetGlobalDepth() const
	{
		float depth = 0.01f + initialDepth;

		if (auto parent = m_parent.lock())
		{
			auto childeren = parent->GetChildren();

			auto it = childeren.begin();

			while (it != childeren.end())
			{
				if ((*it).get() == this)
				{
					break;
				}

				depth += 0.01f;

				it++;
			}

			depth += parent->GetGlobalDepth();
		}

		return (std::min)(0.99f, depth);
	}

	ptl::vector<ptl::weak_ptr<UIElement>> UIElement::GetChildElementsOfTag(const ptl::string& tag)
	{
		ptl::vector<ptl::weak_ptr<UIElement>> children;

		auto it = m_children.begin();

		while (it != m_children.end())
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

	ptl::weak_ptr<UIElement> UIElement::GetChildElementOfTag(const ptl::string& tag)
	{
		auto childrenWithTag = GetChildElementsOfTag(tag);

		if (childrenWithTag.size() > 0)
		{
			return childrenWithTag[0];
		}

		return ptl::weak_ptr<UIElement>();
	}

	void UIElement::SetParent(ptl::shared_ptr<UIElement> a_element)
	{
		if (ptl::shared_ptr<UIElement> parent = m_parent.lock())
		{
			auto it = parent->m_children.begin();

			while (it != parent->m_children.end())
			{
				if ((*it) == sharedFromThis())
				{
					it = parent->m_children.erase(it);
				}
				else
				{
					it++;
				}
			}
		}

		m_parent = ptl::weak_ptr<UIElement>(a_element);
		a_element->m_children.push_back(sharedFromThis());
	}

	ptl::weak_ptr<UIElement> UIElement::GetParent()
	{
		return m_parent;
	}

	bool UIElement::OverlapsWithPoint(const int, const int) const
	{
		return false;
	}

	const glm::mat4 UIElement::GetModelMatrix() const
	{
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(m_position.x, m_position.y, 0.02f));
		glm::mat4 rotationMatrix = glm::eulerAngleYXZ(0.0f, 0.0f, m_rotation);
		glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

		glm::mat4 mm = translationMatrix * rotationMatrix * scalingMatrix;

		if (auto parent = m_parent.lock())
		{
			mm = parent->GetModelMatrix() * mm;
		}

		return mm;
	}
	 
	void UIElement::SetEvent(UIEvent uiEvent)
	{
		if (!m_events.empty())
		{
			m_events.clear();
			m_events.push_back(uiEvent);
		}
	}

	UITransition & UIElement::AddTransition(const ptl::string & name, UITransitionProperties elementValue, float start, float end, float duration, std::function<float(float, float, float)> method, bool expireOnEnd, bool autoStart)
	{
		auto pair = std::make_pair(name, elementValue);

		auto it = transitions.find(pair);
		if (it != transitions.end())
		{
			transitions.erase(it);
		}


		transitions.emplace(pair, UITransition{ start, end, duration, method, expireOnEnd });
		if (autoStart)
		{
			transitions[pair].Start();
		}

		return transitions[pair];
	}

	bool UIElement::GetTransition(const ptl::string & name, UITransitionProperties elementValue, UITransition& transition)
	{
		auto it = transitions.find(std::make_pair(name, elementValue));
		if (it != transitions.end())
		{
			transition = it->second;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool UIElement::HasTransition(const ptl::string& name, const UITransitionProperties elementValue)
	{
		auto it = transitions.find(std::make_pair(name, elementValue));
		return it != transitions.end();
	}

	void UIElement::RemoveTransition(const ptl::string & name, UITransitionProperties elementValue)
	{
		auto it = transitions.find(std::make_pair(name, elementValue));

		if (it != transitions.end())
		{
			transitions.erase(it);
		}
	}

	void UIElement::ClearTransitions()
	{
		transitions.clear();
	}

	const bool UIElement::IsVisible() const noexcept
	{
		return visible;
	}

	void UIElement::SetVisible(const bool visible)
	{
		this->visible = visible;
	}

}
