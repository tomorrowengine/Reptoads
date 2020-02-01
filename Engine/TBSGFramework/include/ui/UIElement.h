#pragma once

#include "gameplay/Transform.h"
#include "memory/Containers.h"
#include "memory/String.h"
#include "rendering/IUIRenderer.h"
#include "ui/UITransition.h"
#include "functional"
#include "core/HashCombine.h"


namespace ui
{
	enum class UITransitionProperties;
}

namespace std
{
	template<>
	struct hash<std::pair<ptl::string, ::ui::UITransitionProperties>>
	{
		std::size_t operator()(const std::pair<ptl::string, ::ui::UITransitionProperties> pair) const noexcept
		{
			std::size_t seed = 9;

			::core::hash_combine(seed, pair.first);
			::core::hash_combine(seed, static_cast<size_t>(pair.second));

			return seed;
		}
	};
}

namespace ui {

	struct UIEventStructure {
		ptl::string m_type;
		ptl::string m_action;
		ptl::string m_data;
	};

	enum class UIElementType {
		Element,
		Image,
		Text
	};

	struct UIElementStructure
	{
		UIElementStructure(ptl::string a_name, ptl::string tag, ptl::vector<UIEventStructure> a_events, glm::vec3 a_position, glm::vec2 a_scale, float a_rotation, ptl::vector<UIElementStructure*> a_children) :
			m_name(a_name),
			tag(tag),
			m_events(a_events),
			m_position(a_position),
			m_scale(a_scale),
			m_rotation(a_rotation),
			m_children(a_children),
			m_type(UIElementType::Element)
		{}

		virtual ~UIElementStructure() {
			for (ptl::vector<UIElementStructure*>::iterator it = m_children.begin(); it != m_children.end(); ++it) {
				delete *it;
			}

			m_children.clear();
		}

		const UIElementType GetType() const noexcept { return m_type; }


		ptl::string m_name{};
		ptl::string tag{};

		ptl::vector<UIEventStructure> m_events;
		glm::vec3 m_position{};
		glm::vec2 m_scale{};
		float m_rotation{};

		ptl::vector<UIElementStructure*> m_children{};

	protected:
		UIElementType m_type{};

	};

	struct UIEvent {
		ptl::string m_type;
		ptl::string m_action;
		ptl::string m_data;
	};

	enum class UITransitionProperties
	{
		PositionX,
		PositionY,
		PositionXY,
		SizeX,
		SizeY,
		SizeXY,
		Rotation
	};

	class UIElement : public ptl::enable_shared_from_this<UIElement>
	{
	public:
		UIElement();
		UIElement(UIElementStructure* data);
		virtual ~UIElement();

		const glm::vec2 GetGlobalPosition() const;

		const glm::vec2& GetPosition() const { return m_position; }
		void SetPosition(const glm::vec2& a_position) { m_position = a_position; }

		const ptl::string& GetName() const;
		void SetName(const ptl::string& name);

		const ptl::string& GetTag() const noexcept;
		void SetTag(const ptl::string& tag);

		const bool IsVisible() const noexcept;
		void SetVisible(const bool visible);

		const ptl::vector<UIEvent>& GetEvents() const noexcept { return m_events; }

		const float GetGlobalRotation() const;

		const float& GetRotation() const { return m_rotation; }
		void SetRotation(const float a_rotation) { m_rotation = a_rotation; }
        const float GetGlobalDepth() const ;

		ptl::vector<ptl::weak_ptr<UIElement>> GetChildElementsOfTag(const ptl::string& tag);
		ptl::weak_ptr<UIElement> GetChildElementOfTag(const ptl::string& tag);

		ptl::vector<ptl::weak_ptr<UIElement>> GetElements() {
			return ptl::vector<ptl::weak_ptr<UIElement>>(m_children.begin(), m_children.end());
		}

		const float& GetInitialDepth() const { return initialDepth; }
		void SetInitialDepth(const float& a_depth) { initialDepth = a_depth; }

		void AddEvent(UIEvent a_event) { m_events.push_back(a_event); }

		void SetParent(ptl::shared_ptr<UIElement> a_element);

		ptl::weak_ptr<UIElement> GetParent();

		const UIElementType GetType() const noexcept { return m_type; }

		virtual bool OverlapsWithPoint(const int x, const int y) const;

		const glm::mat4 GetModelMatrix() const;

        void AddEvents(UIEvent uiEvent) { m_events.push_back(uiEvent); }
        void SetEvent(UIEvent uiEvent);

        ptl::vector<ptl::shared_ptr<UIElement>>& GetChildren() { return m_children; }

		ptl::unordered_map<std::pair<ptl::string, UITransitionProperties>, UITransition>& GetTransitions() { return transitions; }
		UITransition& AddTransition(const ptl::string& name, const UITransitionProperties elementValue, float start, float end, float duration, std::function<float(float, float, float)> method = UITransition::linear, bool expireOnEnd = true, bool autoStart = true);
		bool GetTransition(const ptl::string& name, const UITransitionProperties elementValue, UITransition& transition);
		bool HasTransition(const ptl::string& name, const UITransitionProperties elementValue);

		void RemoveTransition(const ptl::string& name, const UITransitionProperties elementValue);
		void ClearTransitions();

	protected:
		std::shared_ptr<UIElement> sharedFromThis() {
			return shared_from_this();
		}

		ptl::string m_name{};
		ptl::string tag{};

		ptl::vector<ptl::shared_ptr<UIElement>> m_children{};
		ptl::vector<UIEvent> m_events{};
		ptl::unordered_map<std::pair<ptl::string, UITransitionProperties>, UITransition> transitions;

		ptl::weak_ptr<UIElement> m_parent{};
		glm::vec2 m_position{};
		float m_rotation{};
		UIElementType m_type{};
		float initialDepth{};
		bool visible{true};
	};
}
