#pragma once

#include "ui/UIStack.h"
#include "resource_loading/UILoader.h"
#include "memory/Containers.h"
#include "memory/String.h"

#include "core/Config.h"
#include "core/StrHash.h"
#include "rendering/IUIRenderer.h"
#include "ui/UIStack.h"
#include "ui/UIImage.h"
#include "ui/UIText.h"
#include "rendering/IResourceManager.h"
#include <core/Input.h>
#include <core/DeltaTime.h>

#include "ui/UISystemInterface.h"
#include "Utility/Observable.h"
#include "audio/AudioSystem.h"
#include <chrono>


namespace ui
{
	class UISystem : public Observable<UISystemInterface>
	{
		enum class NeighbourDirection {
			Top = 0,
			Bottom = 1,
			Left = 2,
			Right = 3
		};

	public:
		UISystem(gfx::IUIRenderer* a_renderer, gfx::IResourceManager& a_resourceManager, audio::AudioSystem& a_audioSystem, std::function<void(ptl::shared_ptr<UIText>)> onTextfieldFocussed);
		~UISystem();

		void LoadUI(ptl::string a_ui, bool force = false);

		ptl::weak_ptr<UIElement> PushUI(ptl::string a_ui);
		ptl::weak_ptr<UIElement> AddUI(ptl::string a_ui);
		void PopUI(ptl::string a_ui);
		void PopUI(ptl::weak_ptr<UIElement> a_rootElement);
		void Clear();

		void Update(const core::DeltaTime& deltaTime, glm::ivec2 a_cursorPos);

		bool OnClick(tbsg::InputAction a_action);
		bool OnBackspace(tbsg::InputAction a_action);

		bool OnUp(tbsg::InputAction a_action);
		bool OnDown(tbsg::InputAction a_action);
		bool OnLeft(tbsg::InputAction a_action);
		bool OnRight(tbsg::InputAction a_action);

		bool OnAxisVertical(float a_value);
		bool OnAxisHorizontal(float a_value);

		bool OnCharacterTyped(char character);
		bool OnTextDialogResult(ptl::string text);

		UIStack* GetUIStack() { return m_uiStack; }
		void RemoveElementFromRenderer(ptl::shared_ptr<UIElement> element);

		void Dialog(ptl::string a_msg, std::function<void()> onDialogConfirmed = std::function<void()>{}, glm::vec2 position = glm::vec2{ 563.0f, 230.0f });

		ptl::vector<ptl::weak_ptr<UIElement>> GetChildElementsOfTag(const ptl::string& tag);
		ptl::weak_ptr<UIElement> GetChildElementOfTag(const ptl::string& tag);

		void SetTextForElementsOfTag(const ptl::string& text, const ptl::string& tag);
		void SetImageForElementsOfTag(const ptl::string& image, const ptl::string& tag);

		void AddElement(ptl::shared_ptr<UIElement> parent, ptl::shared_ptr<UIElement> element);
		void AddElement(const ptl::string& parentTag, ptl::shared_ptr<UIElement> element);

		void RemoveElement(ptl::shared_ptr<UIElement> element);
		
	private:
		float m_vertical{};
		float m_horizontal{};

		ptl::weak_ptr<UIElement> GetFirstNeighbour(ptl::shared_ptr<UIElement> element, UISystem::NeighbourDirection a_direction);

		void GetAllInteractables(ptl::vector<ptl::weak_ptr<UIElement>>& elements, ptl::shared_ptr<UIElement> parent);

		void SelectFirst();

		void Update(const core::DeltaTime& deltaTime, ptl::shared_ptr<UIElement> rootElement, glm::ivec2 a_cursorPos);
		void UpdateTransitions(const core::DeltaTime& deltaTime, ptl::shared_ptr<UIElement> element);
		void OnMouseEnter(ptl::shared_ptr<UIElement> element);
		void OnMouseExit(ptl::shared_ptr<UIElement> element);
		void OnMouseDown(ptl::shared_ptr<UIElement> element);
		void OnMouseUp(ptl::shared_ptr<UIElement> element);

		bool Hovering(ptl::shared_ptr<UIElement> element);

		void FireEvents(ptl::shared_ptr<UIElement> element, ptl::string a_type);

		ptl::shared_ptr<UIElement> InstantiateUI(UIStacktructure* a_uiStack);
		ptl::shared_ptr<UIElement> InstantiateUI(ptl::shared_ptr<UIElement> a_parent, UIElementStructure * a_uiElement);

		ptl::unordered_map<std::uint64_t, UIStacktructure*> m_loadedUI{};
		ptl::unordered_map<std::uint64_t, ptl::weak_ptr<UIElement>> m_spawnedUI{};

		gfx::IUIRenderer* m_renderer{};
		gfx::IResourceManager& m_resourceManager;
		audio::AudioSystem& m_audioSystem;
		UILoader m_uiLoader{};
		UIStack* m_uiStack;
		
		ptl::vector<ptl::weak_ptr<UIElement>> m_hovering{};

		bool dialogVisible{};
		ptl::weak_ptr<UIText> focussedInputField{};
		
		std::chrono::high_resolution_clock::time_point barStart{};

		std::function<void(ptl::shared_ptr<UIText>)> onTextfieldFocussed{};
		std::function<void()> onDialogConfirmed{};
	};

}
