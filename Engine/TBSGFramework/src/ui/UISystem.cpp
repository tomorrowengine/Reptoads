#include "ui/UISystem.h"
#include <sstream>


namespace ui
{
	UISystem::UISystem(gfx::IUIRenderer* a_renderer, gfx::IResourceManager& a_resourceManager, audio::AudioSystem& a_audioSystem, std::function<void(ptl::shared_ptr<UIText>)> onTextfieldFocussed)
		: m_renderer(a_renderer), m_uiStack(new UIStack()), m_resourceManager(a_resourceManager), m_audioSystem(a_audioSystem)
	{
		this->onTextfieldFocussed = onTextfieldFocussed;
	}


	UISystem::~UISystem()
	{
		m_hovering.clear();

		for (auto& it : m_loadedUI)
		{
			delete it.second;
		}

		m_loadedUI.clear();

		delete m_uiStack;

	}

	void UISystem::LoadUI(ptl::string ui, bool force)
	{
		const ptl::string fullPath = tbsg::Config::Get().MakeUIPath(ui);

		const std::uint64_t hash = tbsg::HashString(fullPath);

		auto it = m_loadedUI.find(hash);

		if (it == m_loadedUI.end() || force)
		{
			if (ui::UIStacktructure* uiStackStructure = m_uiLoader.Load(fullPath))
			{
				if (it != m_loadedUI.end())
				{
					delete it->second;
					m_loadedUI.erase(it);
				}

				m_loadedUI.emplace(hash, uiStackStructure);
			}
		}
	}

	ptl::weak_ptr<UIElement> UISystem::PushUI(ptl::string a_ui)
	{
		const ptl::string fullPath = tbsg::Config::Get().MakeUIPath(a_ui);
		const std::uint64_t hash = tbsg::HashString(fullPath);

		auto it = m_loadedUI.find(hash);

		if (it == m_loadedUI.end())
		{
			LoadUI(a_ui);
			it = m_loadedUI.find(hash);
		}

		if (it != m_loadedUI.end())
		{
			ptl::shared_ptr<UIElement> root = InstantiateUI(it->second);
			m_uiStack->PushElement(root);
			m_spawnedUI[hash] = root;
			SelectFirst();

			return root;
		}


		return ptl::weak_ptr<UIElement>();
	}

	ptl::weak_ptr<UIElement> UISystem::AddUI(ptl::string a_ui)
	{
		const ptl::string fullPath = tbsg::Config::Get().MakeUIPath(a_ui);
		const std::uint64_t hash = tbsg::HashString(fullPath);

		auto it = m_loadedUI.find(hash);

		if (it != m_loadedUI.end())
		{
			ptl::shared_ptr<UIElement> root = InstantiateUI(it->second);
			m_uiStack->AddElement(root);
			m_spawnedUI[hash] = root;
			SelectFirst();

			return root;
		}

		return ptl::weak_ptr<UIElement>();
	}

	void UISystem::PopUI(ptl::string a_ui)
	{
		const ptl::string fullPath = tbsg::Config::Get().MakeUIPath(a_ui);
		const std::uint64_t hash = tbsg::HashString(fullPath);

		auto it = m_spawnedUI.find(hash);

		if (it != m_spawnedUI.end())
		{
			PopUI(it->second);
		}
	}

	void UISystem::PopUI(ptl::weak_ptr<UIElement> a_rootElement)
	{
		auto it = m_spawnedUI.begin();
		while (it != m_spawnedUI.end())
		{
			if (it->second.lock() == a_rootElement.lock())
			{
				it = m_spawnedUI.erase(it);
			}
			else
			{
				it++;
			}
		}
		RemoveElementFromRenderer(a_rootElement.lock());

		m_uiStack->PopElement(a_rootElement);

	}

	void UISystem::Clear()
	{
		auto it = m_spawnedUI.begin();
		while (it != m_spawnedUI.end())
		{
			RemoveElementFromRenderer((*it).second.lock());

			m_uiStack->PopElement((*it).second.lock());

			it++;
		}
		
		m_hovering.clear();
		focussedInputField.reset();
		dialogVisible = false;
		onDialogConfirmed = std::function<void()>();
		onTextfieldFocussed = std::function<void(ptl::shared_ptr<UIText>)>{};
	}

	void UISystem::Update(const core::DeltaTime& deltaTime, glm::ivec2 a_cursorPos)
	{
		auto it = m_hovering.begin();

		while (it != m_hovering.end())
		{
			bool interactable = true;
			auto element = (*it).lock();

			if (element && element->GetType() == UIElementType::Image)
			{
				interactable = std::static_pointer_cast<UIImage>(element)->IsInteractable();
			}

			if (element == nullptr || !interactable)
			{
				if (element)
				{
					OnMouseExit(element);
				}

				it = m_hovering.erase(it);
			}
			else
			{
				it++;
			}
		}

		for (auto& element : m_uiStack->GetElements())
		{
			Update(deltaTime, element.lock(), a_cursorPos);
		}
	}

	ptl::weak_ptr<UIElement> UISystem::GetFirstNeighbour(ptl::shared_ptr<UIElement> element, UISystem::NeighbourDirection a_direction)
	{
		ptl::vector<ptl::weak_ptr<UIElement>> interactables;

		ptl::weak_ptr<UIElement> closest = ptl::weak_ptr<UIElement>();

		for (auto stack : m_uiStack->GetElements())
		{
			GetAllInteractables(interactables, stack.lock());
		}

		for (auto image : interactables)
		{
			if (image.lock() != element)
			{
				switch (a_direction)
				{
				case UISystem::NeighbourDirection::Top: {
					if (image.lock()->GetGlobalPosition().y > element->GetGlobalPosition().y)
					{
						if (auto c = closest.lock())
						{
							if (glm::distance(image.lock()->GetGlobalPosition().y, element->GetGlobalPosition().y) <
								glm::distance(element->GetGlobalPosition().y, c->GetGlobalPosition().y))
							{
								closest = image;
							}
						}
						else
						{
							closest = image;
						}
					}
				}break;
				case UISystem::NeighbourDirection::Bottom: {
					if (image.lock()->GetGlobalPosition().y < element->GetGlobalPosition().y)
					{
						if (auto c = closest.lock())
						{
							if (glm::distance(image.lock()->GetGlobalPosition().y, element->GetGlobalPosition().y) <
								glm::distance(element->GetGlobalPosition().y, c->GetGlobalPosition().y))
							{
								closest = image;
							}
						}
						else
						{
							closest = image;
						}
					}
				}break;
				case UISystem::NeighbourDirection::Left: {
					if (image.lock()->GetGlobalPosition().x < element->GetGlobalPosition().x)
					{
						if (auto c = closest.lock())
						{
							if (glm::distance(image.lock()->GetGlobalPosition().x, element->GetGlobalPosition().x) <
								glm::distance(element->GetGlobalPosition().x, c->GetGlobalPosition().x))
							{
								closest = image;
							}
						}
						else
						{
							closest = image;
						}
					}
				}break;
				case UISystem::NeighbourDirection::Right: {
					if (image.lock()->GetGlobalPosition().x > element->GetGlobalPosition().x)
					{
						if (auto c = closest.lock())
						{
							if (glm::distance(image.lock()->GetGlobalPosition().x, element->GetGlobalPosition().x) <
								glm::distance(element->GetGlobalPosition().x, c->GetGlobalPosition().x))
							{
								closest = image;
							}
						}
						else
						{
							closest = image;
						}
					}
				}break;
				default:
					break;
				};
			}

		}


		return closest;
	}

	void UISystem::GetAllInteractables(ptl::vector<ptl::weak_ptr<UIElement>>& elements, ptl::shared_ptr<UIElement> a_parent)
	{
		if (a_parent->GetType() == UIElementType::Image)
		{
			auto image = std::static_pointer_cast<UIImage>(a_parent);

			if (image->IsInteractable())
			{
				elements.push_back(image);
			}
		}
		else if (a_parent->GetType() == UIElementType::Text)
		{
			auto text = std::static_pointer_cast<UIText>(a_parent);

			if (text->InputEnabled())
			{
				elements.push_back(text);
			}
		}
		for (auto child : a_parent->GetElements())
		{
			GetAllInteractables(elements, child.lock());
		}
	}

	void UISystem::RemoveElementFromRenderer(ptl::shared_ptr<UIElement> element)
	{
		switch (element->GetType())
		{
		case UIElementType::Image:
			m_renderer->RemoveImage(static_cast<UIImage*>(element.get()));
			break;
		case UIElementType::Text:
			m_renderer->RemoveText(static_cast<UIText*>(element.get()));
			break;
		default:
			break;
		}

		for (auto e : element->GetElements())
		{
			RemoveElementFromRenderer(e.lock());
		}
	}

	void UISystem::Dialog(ptl::string a_msg, std::function<void()> onDialogConfirmed, glm::vec2 position)
	{
		this->onDialogConfirmed = onDialogConfirmed;
		
		if (dialogVisible)
		{
			PopUI("Dialog.ui");
		}

		auto root = PushUI("Dialog.ui").lock();
		
		if (root)
		{
			root->SetPosition(position);

			auto element = root->GetChildElementOfTag("DialogText").lock();

			if (element)
			{
				if (element->GetType() == UIElementType::Text)
				{
					std::static_pointer_cast<UIText>(element)->SetText(a_msg);
				}
			}
		}


	}

	ptl::vector<ptl::weak_ptr<UIElement>> UISystem::GetChildElementsOfTag(const ptl::string& tag)
	{
		return m_uiStack->GetChildElementsOfTag(tag);
	}

	ptl::weak_ptr<UIElement> UISystem::GetChildElementOfTag(const ptl::string& tag)
	{
		return m_uiStack->GetChildElementOfTag(tag);
	}

	void UISystem::SelectFirst()
	{
		if (m_hovering.size() == 0)
		{
			ptl::vector<ptl::weak_ptr<UIElement>> interactables;
			for (auto stack : m_uiStack->GetElements())
			{
				GetAllInteractables(interactables, stack.lock());
			}
			if (interactables.size() > 0)
			{
				m_hovering.push_back(interactables[0].lock());
				OnMouseEnter(interactables[0].lock());
			}
		}
	}

	void UISystem::Update(const core::DeltaTime& deltaTime, ptl::shared_ptr<UIElement> element, glm::ivec2 a_cursorPos)
	{

		UpdateTransitions(deltaTime, element);

		if (a_cursorPos.x > 0 || a_cursorPos.y > 0)
		{
			if (element->OverlapsWithPoint(a_cursorPos.x, a_cursorPos.y))
			{
				if (!Hovering(element))
				{
					if (m_hovering.size() > 0)
					{
						auto hoveringElement = m_hovering[0].lock();

						if (hoveringElement)
						{
							if (element->GetGlobalDepth() > hoveringElement->GetGlobalDepth())
							{
								auto it = m_hovering.begin();

								while (it != m_hovering.end())
								{
									if ((*it).lock() == hoveringElement)
									{
										it = m_hovering.erase(it);
										break;
									}
									else
									{
										it++;
									}
								}

								OnMouseExit(hoveringElement);

								m_hovering.push_back(element);
								OnMouseEnter(element);
							}
						}
						else
						{
							m_hovering.push_back(element);
							OnMouseEnter(element);
						}
					}
					else
					{
						m_hovering.push_back(element);
						OnMouseEnter(element);
					}
				}
			}
			else if (Hovering(element))
			{
				auto it = m_hovering.begin();

				while (it != m_hovering.end())
				{
					if ((*it).lock() == element)
					{
						it = m_hovering.erase(it);
						break;
					}
					else
					{
						it++;
					}
				}

				OnMouseExit(element);
			}
		}
		else
		{
			SelectFirst();
		}

		auto focussed = focussedInputField.lock();
		if (focussed)
		{
			auto t = std::chrono::high_resolution_clock::now();

			auto diff = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(t - barStart).count() / 1000.0;

			if (diff >= 0.5f)
			{
				barStart = t;
				focussed->SetBarEnabled(!focussed->GetBarEnabled());
			}
		}


		for (auto child : element->GetElements())
		{
			Update(deltaTime, child.lock(), a_cursorPos);
		}

		switch (element->GetType())
		{
		case UIElementType::Image: {
			auto image = static_cast<UIImage*>(element.get());
			if (!m_resourceManager.ContainsTexture(image->GetTextureId()))
			{
				m_resourceManager.CreateTexture(image->GetTextureId());
			}

			m_renderer->UpdateImage(image, element->GetModelMatrix());
		}	break;
		case UIElementType::Text: {
			if (tbsg::Config::Get().enableTextRendering)
			{
				auto text = static_cast<UIText*>(element.get());

				if (text->GetText().compare(text->GetOldText()) != 0)
				{
					m_renderer->RemoveText(text);
					text->Update();
					m_renderer->AddText(text, text->GetModelMatrix());
				}
				else
				{
					m_renderer->UpdateText(text, element->GetModelMatrix());
				}
			}


		}	break;
		default:
			break;
		}
	}

	void UISystem::UpdateTransitions(const core::DeltaTime& deltaTime, ptl::shared_ptr<UIElement> element)
	{
		auto& transitions = element->GetTransitions();

		auto it = transitions.begin();

		while (it != transitions.end())
		{
			auto& transition = (*it);
			auto nameValuePair = transition.first;
			transition.second.Update(static_cast<float>(deltaTime.GetDeltaTime()));

			glm::vec2 pos = element->GetPosition();
			glm::vec2 size{ 1, 1 };
			ptl::shared_ptr <UIText> elementAsText;
			ptl::shared_ptr <UIImage> elementAsImage;

			if (element->GetType() == UIElementType::Image)
			{
				elementAsImage = std::static_pointer_cast<UIImage>(element);
				size = elementAsImage->GetSize();
			}
			else if (element->GetType() == UIElementType::Text)
			{
				elementAsText = std::static_pointer_cast<UIText>(element);
				size = elementAsText->GetSize();
			}

			switch (nameValuePair.second)
			{
			case UITransitionProperties::PositionX: {
				pos.x = transition.second.Get();
			}	break;
			case UITransitionProperties::PositionY: {
				pos.y = transition.second.Get();
			}	break;
			case UITransitionProperties::PositionXY: {
				pos.x = transition.second.Get();
				pos.y = transition.second.Get();
			}	break;
			case UITransitionProperties::SizeX: {
				size.x = transition.second.Get();
			}	break;
			case UITransitionProperties::SizeY: {
				size.y = transition.second.Get();
			}	break;
			case UITransitionProperties::SizeXY: {
				size.x = transition.second.Get();
				size.y = transition.second.Get();
			}	break;
			case UITransitionProperties::Rotation: {
				element->SetRotation(transition.second.Get());
			}	break;
			}

			element->SetPosition(pos);

			if (elementAsImage)
			{
				elementAsImage->SetSize(size);
			}
			else if (elementAsText)
			{
				elementAsText->SetSize(size);
			}

			if (transition.second.Expired())
			{
				it = transitions.erase(it);
			}
			else
			{
				it++;
			}
		}
	}

	void UISystem::OnMouseEnter(ptl::shared_ptr<UIElement> element)
	{
		NotifyObservers([element](UISystemInterface* observer)
		{
			observer->OnElementFocussed(element);
		});

		FireEvents(element, "OnFocussed");
	}

	void UISystem::OnMouseExit(ptl::shared_ptr<UIElement> element)
	{
		NotifyObservers([element](UISystemInterface* observer)
		{
			observer->OnElementFocusLost(element);
		});

		FireEvents(element, "OnFocusLost");
	}

	void UISystem::OnMouseDown(ptl::shared_ptr<UIElement> element)
	{
		NotifyObservers([element](UISystemInterface* observer)
		{
			observer->OnElementPressed(element);
		});

		FireEvents(element, "OnButtonPressed");

		auto focussed = focussedInputField.lock();
		if (focussed)
		{
			focussed->SetBarEnabled(false);

			focussed->OnFocusLost();
			focussedInputField.reset();

		}

		if (element->GetType() == UIElementType::Text)
		{
			auto text = std::static_pointer_cast<UIText>(element);

			if (text->InputEnabled())
			{
				focussedInputField = text;
				text->OnFocus();
				barStart = std::chrono::high_resolution_clock::now();
				text->SetBarEnabled(true);

				if(onTextfieldFocussed){
					onTextfieldFocussed(text);
				}
			}
		}
	}

	void UISystem::OnMouseUp(ptl::shared_ptr<UIElement> element)
	{
		NotifyObservers([element](UISystemInterface* observer)
		{
			observer->OnElementReleased(element);
		});

		FireEvents(element, "OnButtonReleased");
	}

	bool UISystem::Hovering(ptl::shared_ptr<UIElement> element)
	{
		auto it = m_hovering.begin();

		while (it != m_hovering.end())
		{
			if ((*it).lock() == element)
			{
				return true;
			}
			else
			{
				it++;
			}
		}

		return false;
	}

	void UISystem::FireEvents(ptl::shared_ptr<UIElement> element, ptl::string a_type)
	{
		for (auto e : element->GetEvents())
		{
			if (e.m_type == a_type)
			{
				if (e.m_action == "loadUITop")
				{
					PushUI(e.m_data);
				}
				else if (e.m_action == "loadUIBottom")
				{
					AddUI(e.m_data);
				}
				else if (e.m_action == "unloadUI")
				{
					PopUI(e.m_data);
				}
				else if (e.m_action == "setImage")
				{
					if (element->GetType() == UIElementType::Image)
					{
						if (!m_resourceManager.ContainsTexture(e.m_data.c_str()))
						{
							m_resourceManager.CreateTexture(e.m_data.c_str());
						}

						if (m_resourceManager.ContainsTexture(e.m_data.c_str()))
						{
							std::static_pointer_cast<UIImage>(element)->SetTextureId(e.m_data);
						}
					}
				}
				else if (e.m_action == "playSound")
				{
					m_audioSystem.CreateSound(e.m_data);
					m_audioSystem.PlaySoundEffect(m_audioSystem.GetSoundIDFromName(e.m_data));
				}
				else if (e.m_action == "playSoundLoop")
				{
					m_audioSystem.CreateSound(e.m_data);
					m_audioSystem.PlaySoundEffect(m_audioSystem.GetSoundIDFromName(e.m_data), true);
				}
				else if (e.m_action == "stopSound")
				{
					m_audioSystem.StopAllSoundEffects();
				}
				else if (e.m_action == "dialogConfirm")
				{
					PopUI("Dialog.ui");
					if (onDialogConfirmed)
					{
						onDialogConfirmed();
					}
					onDialogConfirmed = std::function<void()>{};
				}
				else
				{
					NotifyObservers([element, e](UISystemInterface* observer)
					{
						observer->OnEvent(element, e);
					});
				}
			}
		}

		if (element->GetType() == UIElementType::Text)
		{
			auto text = std::static_pointer_cast<UIText>(element);

			if (!text->GetInteractableTag().empty())
			{
				auto elements = GetChildElementsOfTag(text->GetInteractableTag());

				for (auto& element : elements)
				{
					FireEvents(element.lock(), a_type);
				}
			}
		}
	}

	bool UISystem::OnClick(tbsg::InputAction a_action)
	{
		if (a_action == tbsg::InputAction::EPressed)
		{
			for (auto element : m_hovering)
			{
				if (element.lock())
				{
					OnMouseDown(element.lock());
				}
			}
		}
		else if (a_action == tbsg::InputAction::EReleased)
		{
			for (auto element : m_hovering)
			{
				if (element.lock())
				{
					OnMouseUp(element.lock());
				}
			}
		}

		return false;
	}

	bool UISystem::OnBackspace(tbsg::InputAction a_action)
	{
		if (a_action == tbsg::InputAction::EPressed)
		{
			auto focussed = focussedInputField.lock();

			if (focussed)
			{
				focussed->SetText(focussed->GetRawText().substr(0, focussed->GetRawText().size() - 1));
			}

		}
		return false;
	}

	bool UISystem::OnUp(tbsg::InputAction a_action)
	{
		if (a_action == tbsg::InputAction::EPressed)
		{
			if (m_hovering.size() > 0)
			{
				if (auto neighbour = GetFirstNeighbour(m_hovering[0].lock(), UISystem::NeighbourDirection::Top).lock())
				{

					OnMouseExit(m_hovering[0].lock());

					m_hovering.clear();
					m_hovering.push_back(neighbour);

					OnMouseEnter(neighbour);
				}

			}
		}


		return false;
	}

	bool UISystem::OnDown(tbsg::InputAction a_action)
	{
		if (a_action == tbsg::InputAction::EPressed)
		{
			if (m_hovering.size() > 0)
			{
				if (auto neighbour = GetFirstNeighbour(m_hovering[0].lock(), UISystem::NeighbourDirection::Bottom).lock())
				{

					OnMouseExit(m_hovering[0].lock());

					m_hovering.clear();
					m_hovering.push_back(neighbour);

					OnMouseEnter(neighbour);
				}

			}
		}


		return false;
	}

	bool UISystem::OnLeft(tbsg::InputAction a_action)
	{
		if (a_action == tbsg::InputAction::EPressed)
		{

			if (m_hovering.size() > 0)
			{
				if (auto neighbour = GetFirstNeighbour(m_hovering[0].lock(), UISystem::NeighbourDirection::Left).lock())
				{

					OnMouseExit(m_hovering[0].lock());

					m_hovering.clear();
					m_hovering.push_back(neighbour);

					OnMouseEnter(neighbour);
				}

			}
		}
		return false;
	}

	bool UISystem::OnRight(tbsg::InputAction a_action)
	{
		if (a_action == tbsg::InputAction::EPressed)
		{

			if (m_hovering.size() > 0)
			{
				if (auto neighbour = GetFirstNeighbour(m_hovering[0].lock(), UISystem::NeighbourDirection::Right).lock())
				{

					OnMouseExit(m_hovering[0].lock());

					m_hovering.clear();
					m_hovering.push_back(neighbour);

					OnMouseEnter(neighbour);
				}

			}
		}
		return false;
	}

	bool UISystem::OnAxisVertical(float a_value)
	{
		if (std::abs(m_vertical) < 0.3f && std::abs(a_value) >= 0.3f)
		{
			if (a_value < 0)
			{
				OnDown(tbsg::InputAction::EPressed);
			}
			else
			{
				OnUp(tbsg::InputAction::EPressed);
			}
		}

		m_vertical = a_value;
		return false;
	}

	bool UISystem::OnAxisHorizontal(float a_value)
	{
		if (std::abs(m_horizontal) < 0.3f && std::abs(a_value) >= 0.3f)
		{
			if (a_value < 0)
			{
				OnLeft(tbsg::InputAction::EPressed);
			}
			else
			{
				OnRight(tbsg::InputAction::EPressed);
			}
		}

		m_horizontal = a_value;
		return false;
	}

	bool UISystem::OnCharacterTyped(char character)
	{
		auto focussed = focussedInputField.lock();

		if (focussed)
		{
			focussed->SetText(focussed->GetRawText() + character);
		}

		return false;
	}

	bool UISystem::OnTextDialogResult(ptl::string text)
	{
		auto focussed = focussedInputField.lock();

		if (focussed)
		{
			focussed->SetText(text);
		}
		return false;
	}

	ptl::shared_ptr<UIElement> UISystem::InstantiateUI(UIStacktructure * a_uiStack)
	{
		ptl::shared_ptr<UIElement> parentElement = ptl::make_shared<ui::UIElement>();
		parentElement->SetName(a_uiStack->m_name);

		for (auto element : a_uiStack->m_elements)
		{
			InstantiateUI(parentElement, element)->SetParent(parentElement);
		}

		return parentElement;
	}

	ptl::shared_ptr<UIElement> UISystem::InstantiateUI(ptl::shared_ptr<UIElement> a_parent, UIElementStructure * a_uiElement)
	{
		ptl::shared_ptr<UIElement> element = nullptr;
		if (a_uiElement->GetType() == UIElementType::Image)
		{
			UIImageStructure* imageStructure = static_cast<UIImageStructure*>(a_uiElement);

			if (!m_resourceManager.ContainsTexture(imageStructure->m_textureId.c_str()))
			{
				m_resourceManager.CreateTexture(imageStructure->m_textureId);
			}

			if (m_resourceManager.ContainsTexture(imageStructure->m_textureId.c_str()))
			{
				element = ptl::make_shared<UIImage>(imageStructure);
				m_renderer->AddImage(std::static_pointer_cast<UIImage>(element).get(), element->GetModelMatrix());
			}
			else
			{
				element = ptl::make_shared<UIElement>(a_uiElement);
			}

		}
		else if (a_uiElement->GetType() == UIElementType::Text)
		{
			UITextStructure* textStructure = static_cast<UITextStructure*>(a_uiElement);
			element = ptl::make_shared<UIText>(textStructure);
			m_renderer->AddText(std::static_pointer_cast<UIText>(element).get(), element->GetModelMatrix());
		}
		else
		{
			element = ptl::make_shared<UIElement>(a_uiElement);
		}

		for (auto child : a_uiElement->m_children)
		{
			InstantiateUI(element, child)->SetParent(element);
		}

		return element;
	}

	void UISystem::SetTextForElementsOfTag(const ptl::string& text, const ptl::string& tag)
	{
		auto elements = GetChildElementsOfTag(tag);

		auto it = elements.begin();

		while (it != elements.end())
		{
			auto element = (*it).lock();

			if (element->GetType() == UIElementType::Text)
			{
				static_cast<ui::UIText*>(element.get())->SetText(text);
			}

			it++;
		}
	}

	void UISystem::SetImageForElementsOfTag(const ptl::string& image, const ptl::string& tag)
	{
		auto elements = GetChildElementsOfTag(tag);

		auto it = elements.begin();

		while (it != elements.end())
		{
			auto element = (*it).lock();

			if (element->GetType() == UIElementType::Image)
			{
				static_cast<ui::UIImage*>(element.get())->SetTextureId(image);
			}

			it++;
		}
	}

	void UISystem::AddElement(ptl::shared_ptr<UIElement> parent, ptl::shared_ptr<UIElement> element)
	{
		element->SetParent(parent);

		switch (element->GetType())
		{
		case UIElementType::Text: {
			std::static_pointer_cast<UIText>(element).get()->Update();
			m_renderer->AddText(std::static_pointer_cast<UIText>(element).get(), element->GetModelMatrix());
		} break;
		case UIElementType::Image: {
			auto image = std::static_pointer_cast<UIImage>(element).get();

			if (!m_resourceManager.ContainsTexture(image->GetTextureId()))
			{
				m_resourceManager.CreateTexture(image->GetTextureId());
			}
			m_renderer->AddImage(image, element->GetModelMatrix());
		} break;
		}
	}

	void UISystem::AddElement(const ptl::string& parentTag, ptl::shared_ptr<UIElement> element)
	{
		auto parent = this->GetChildElementOfTag(parentTag).lock();

		if (parent)
		{
			AddElement(parent, element);
		}
	}

	void UISystem::RemoveElement(ptl::shared_ptr<UIElement> element)
	{
		auto parent = element->GetParent().lock();
		if (parent)
		{
			ptl::vector<ptl::shared_ptr<UIElement>>& children = parent->GetChildren();

			auto it = children.begin();

			while (it != children.end())
			{
				if ((*it).get() == element.get())
				{
					RemoveElementFromRenderer(element);
					it = children.erase(it);
				}
				else
				{
					it++;
				}
			}
		}
		else
		{
			PopUI(element);
		}
	}
}
