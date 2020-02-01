#include "ui/LobbyUI.h"
#include "UserInterface.h"
#include "ui/UIElement.h"
#include "ui/UIText.h"
#include "ui/UIImage.h"
#include "ui/UISystem.h"
#include "resource_loading/EULALoader.h"


constexpr auto PI = 3.1415926545;

void UserInterface::Initialize(ptl::shared_ptr<LobbyEventQueue> lobbyEventQueue,
                               ptl::shared_ptr<UIEventQueue> uiEventQueue, ui::UISystem* uiSystem)
{
	lobbyEQ = lobbyEventQueue;
	uiEQ = uiEventQueue;


	this->uiSystem = uiSystem;
	uiSystem->AddObserver(this);

	lobbyUI.Initialize(this, &lobbyData);

	
	uiEQ->appendListener(UIInteractionEvent::OnSplashScreen1Ended, [this](std::function<void*()> data)
	{
		if (currentUserInterfaceState == UserInterfaceState::SplashScreen_1)
		{
			
			this->SwitchUI("SplashScreen1.ui", "SplashScreen2.ui");
			currentUserInterfaceState = UserInterfaceState::SplashScreen_2;
		}
	});

	uiEQ->appendListener(UIInteractionEvent::OnSplashScreen2Ended, [this](std::function<void*()> data)
	{
		if (currentUserInterfaceState == UserInterfaceState::SplashScreen_2)
		{
			
			this->SwitchUI("SplashScreen2.ui", "SplashScreen3.ui");
			currentUserInterfaceState = UserInterfaceState::SplashScreen_3;
		}
	});

	uiEQ->appendListener(UIInteractionEvent::OnSplashScreen3Ended, [this](std::function<void*()> data)
	{
		this->OnCheckForLoginScreen();
	});

	uiEQ->appendListener(UIInteractionEvent::OnValidTokenFound, [this](std::function<void*()>)
	{
		this->skipLogin = true;
	});

	uiEQ->appendListener(UIInteractionEvent::OnUpdateHealthPlayer, [this](std::function<void*()> data)
	{
		int* health = static_cast<int*>(data());
		OnUpdateHealthPlayer(health[0], health[1]);
	});

	uiEQ->appendListener(UIInteractionEvent::OnUpdateArmorPlayer, [this](std::function<void*()> data)
	{
		OnUpdateArmorPlayer(*static_cast<int*>(data()));
	});

	uiEQ->appendListener(UIInteractionEvent::OnUpdateNamePlayer, [this](std::function<void*()> data)
	{
		OnUpdateNamePlayer(*static_cast<ptl::string*>(data()));
	});

	uiEQ->appendListener(UIInteractionEvent::OnUpdateIconPlayer, [this](std::function<void*()> data)
	{
		OnUpdateIconPlayer(*static_cast<ptl::string*>(data()));
	});

	uiEQ->appendListener(UIInteractionEvent::OnUpdateDeckPlayer, [this](std::function<void*()> data)
	{
		OnUpdateDeckPlayer(*static_cast<int*>(data()));
	});

	uiEQ->appendListener(UIInteractionEvent::OnUpdateDiscardPlayer, [this](std::function<void*()> data)
	{
		OnUpdateDiscardPlayer(*static_cast<int*>(data()));
	});

	
	uiEQ->appendListener(UIInteractionEvent::OnUpdateHealthOpponent, [this](std::function<void*()> data)
	{
		int* health = static_cast<int*>(data());
		OnUpdateHealthOpponent(health[0], health[1]);
	});

	uiEQ->appendListener(UIInteractionEvent::OnUpdateArmorOpponent, [this](std::function<void*()> data)
	{
		OnUpdateArmorOpponent(*static_cast<int*>(data()));
	});

	uiEQ->appendListener(UIInteractionEvent::OnUpdateHandText, [this](std::function<void*()> data)
	{
		Packet packet = *static_cast<Packet*>(data());
		bool isPlayer{false};
		unsigned int number{0};
		packet >> isPlayer;
		packet >> number;
		UpdatePlayersHandSymbol(isPlayer, number);
	});

	uiEQ->appendListener(UIInteractionEvent::OnUpdateNameOpponent, [this](std::function<void*()> data)
	{
		OnUpdateNameOpponent(*static_cast<ptl::string*>(data()));
	});

	uiEQ->appendListener(UIInteractionEvent::OnUpdateIconOpponent, [this](std::function<void*()> data)
	{
		OnUpdateIconOpponent(*static_cast<ptl::string*>(data()));
	});


	
	uiEQ->appendListener(UIInteractionEvent::OnUpdateHealthMonster, [this](std::function<void*()> data)
	{
		int* health = static_cast<int*>(data());
		OnUpdateHealthMonster(health[0], health[1]);
	});

	uiEQ->appendListener(UIInteractionEvent::OnUpdateArmorMonster, [this](std::function<void*()> data)
	{
		OnUpdateArmorMonster(*static_cast<int*>(data()));
	});

	uiEQ->appendListener(UIInteractionEvent::OnUpdateNameMonster, [this](std::function<void*()> data)
	{
		OnUpdateNameMonster(*static_cast<ptl::string*>(data()));
	});

	uiEQ->appendListener(UIInteractionEvent::OnUpdateIconMonster, [this](std::function<void*()> data)
	{
		OnUpdateIconMonster(*static_cast<ptl::string*>(data()));
	});


	

	uiEQ->appendListener(UIInteractionEvent::OnPushCard, [this](std::function<void*()> data)
	{
		std::pair<int, ptl::string> pair = *static_cast<std::pair<int, ptl::string>*>(data());

		OnPushCard(pair.first, pair.second);
	});

	uiEQ->appendListener(UIInteractionEvent::OnPopCard, [this](std::function<void*()> data)
	{
		OnPopCard(*static_cast<int*>(data()));
	});

	uiEQ->appendListener(UIInteractionEvent::OnClearDeck, [this](std::function<void*()> data)
	{
		OnClearDeck();
	});

	
	uiEQ->appendListener(UIInteractionEvent::OnShowReward, [this](std::function<void*()>)
	{
		OnShowReward();
	});

	uiEQ->appendListener(UIInteractionEvent::OnWinScreen, [this](std::function<void*()>)
	{
		OnWinScreen();
	});

	uiEQ->appendListener(UIInteractionEvent::OnLoseScreen, [this](std::function<void*()>)
	{
		OnLoseScreen();
	});

	uiEQ->appendListener(UIInteractionEvent::OnUpdateGameTimer, [this](std::function<void*()> data)
	{
		OnUpdateGameTimer(*static_cast<unsigned int*>(data()));
	});

	uiEQ->appendListener(UIInteractionEvent::OnToggleLoadingScreen, [this](std::function<void*()>)
	{
		OnToggleLoadingScreen();
	});

	uiEQ->appendListener(UIInteractionEvent::OnSwitchToMainMenu, [this](std::function<void*()>)
	{
		OnSwitchToMainMenu();
	});

	uiEQ->appendListener(UIInteractionEvent::OnLoginStatus, [this](std::function<void*()> data)
	{
		OnLoginStatusUpdate(*static_cast<ptl::string*>(data()));
	});

	uiEQ->appendListener(UIInteractionEvent::OnUpdateLoadingScreen, [this](std::function<void*()> data)
	{
		UpdateLoadingScreenText(*static_cast<ptl::string*>(data()));
	});

	uiEQ->appendListener(UIInteractionEvent::OnServerDisconnect, [this](std::function<void*()> data)
	{
		this->uiSystem->Dialog("Lost connection to the Server", []()
		{
			std::exit(0);
		});
	});

	uiEQ->appendListener(UIInteractionEvent::OnUpdateFirstPlayerTag, [uiSystem](std::function<void*()> data)
	{
		int player = *static_cast<int*>(data());

		static auto easing = [](float start, float end, float step)
		{
			double t2 = (step - 1) * (step - 1);
			float newStep = 1 - t2 * t2 * cos(step * PI * 1.5);

			return ui::UITransition::linear(start, end, newStep);
		};

		auto element = uiSystem->GetChildElementOfTag("PlayerFirstTag").lock();

		if (element)
		{
			if (player == 1)
			{
				element->AddTransition("player_first_tag_rotation",
				                       ui::UITransitionProperties::Rotation,
				                       element->GetRotation(),
				                       PI / 2.0f,
				                       1.0f,
				                       easing
				);
			}
			else if (player == 0)
			{
				element->AddTransition("player_first_tag_rotation",
				                       ui::UITransitionProperties::Rotation,
				                       element->GetRotation(),
				                       -(PI / 2.0f),
				                       1.0f,
				                       easing
				);
			}
		}
	});

	
	this->OnStartUp();
}


bool UserInterface::SwitchUiIfPossible(UserInterfaceState currentState, UserInterfaceState newCurrentState,
                                       const ptl::string& toUnload, const ptl::string& toLoad)
{
	if (currentUserInterfaceState == currentState)
	{
		SwitchUI(toUnload, toLoad);
		currentUserInterfaceState = newCurrentState;
		return true;
	}
	return false;
}


void UserInterface::OnElementReleased(ptl::weak_ptr<ui::UIElement> element)
{
	const auto possibleCard = element.lock();

	if (possibleCard)
	{
		if (possibleCard->GetType() != ui::UIElementType::Image) return;

		bool isCard = false;

		auto uiCardItr = std::find_if(cardsInHand.begin(), cardsInHand.end(), [&possibleCard](const auto& card)
		{
			return card.image.get() == possibleCard.get();
		});

		if (uiCardItr == cardsInHand.end()) return;

		int index = 0;
		for (auto& uiCard : cardsInHand)
		{
			int cardId = uiCard.id;

			if (uiCard.image.get() == possibleCard.get() && !uiCard.selected)
			{
				uiEQ->enqueue(UIInteractionEvent::OnSelectCard, [cardId]() mutable
				{
					return static_cast<void*>(&cardId);
				});

				auto easing = ui::UITransition::easeOutQuart;
				auto size = GetCardSize();

				auto parent = uiSystem->GetChildElementOfTag("PlayerCardBox").lock();

				if (parent)
				{
					if (parent->GetType() == ui::UIElementType::Image)
					{
						auto parentImage = std::static_pointer_cast<ui::UIImage>(parent);
						uiCard.image->AddTransition("card_selected_pos_x_select",
						                       ui::UITransitionProperties::PositionX,
						                       uiCard.image->GetPosition().x,
						                       parentImage->GetSize().x / 2.0f - size.x,
						                       0.5f,
						                       easing
						);
					}
				}


				uiCard.image->AddTransition("card_selected_pos_y",
				                       ui::UITransitionProperties::PositionY,
				                       uiCard.image->GetPosition().y,
				                       400,
				                       0.5f,
				                       easing
				);

				uiCard.image->AddTransition("card_selected_size_x",
				                       ui::UITransitionProperties::SizeX,
				                       uiCard.image->GetSize().x,
				                       size.x * 1.5f,
				                       0.25f,
				                       easing
				);

				uiCard.image->AddTransition("card_selected_size_y",
				                       ui::UITransitionProperties::SizeY,
				                       uiCard.image->GetSize().y,
				                       size.y * 1.5f,
				                       0.25f,
				                       easing
				);

				uiCard.selected = true;
			}
			else
			{
				if (uiCard.selected)
				{
					uiEQ->enqueue(UIInteractionEvent::OnDeselectCard, [cardId]() mutable
					{
						return static_cast<void*>(&cardId);
					});

					auto easing = ui::UITransition::easeOutQuart;
					auto size = GetCardSize();

					uiCard.image->AddTransition("card_selected_pos_x_select",
					                       ui::UITransitionProperties::PositionX,
					                       uiCard.image->GetPosition().x,
					                       GetCardPosition(index).x,
					                       0.5f,
					                       easing
					);

					uiCard.image->AddTransition("card_selected_pos_y",
					                       ui::UITransitionProperties::PositionY,
					                       uiCard.image->GetPosition().y,
					                       GetCardPosition(index).y,
					                       0.5f,
					                       easing
					);

					uiCard.image->AddTransition("card_selected_size_x",
					                       ui::UITransitionProperties::SizeX,
					                       uiCard.image->GetSize().x,
					                       size.x,
					                       0.5f,
					                       easing
					);

					uiCard.image->AddTransition("card_selected_size_y",
					                       ui::UITransitionProperties::SizeY,
					                       uiCard.image->GetSize().y,
					                       size.y,
					                       0.5f,
					                       easing
					);

					uiCard.image->SetInitialDepth(0.2f);
					uiCard.selected = false;
				}
			}
			index++;
		}

		SetConfirmEnabled(HasCardsSelected());
	}
}

void UserInterface::OnElementFocussed(ptl::weak_ptr<ui::UIElement> element)
{
	auto card = element.lock();

	if (card)
	{
		int index = 0;
		for (auto& i : cardsInHand)
		{
			if (i.image.get() == card.get())
			{
				auto easing = ui::UITransition::easeOutQuart;
				auto size = GetCardSize();

				if (i.selected)
				{
					auto parent = uiSystem->GetChildElementOfTag("PlayerCardBox").lock();

					if (parent)
					{
						if (parent->GetType() == ui::UIElementType::Image)
						{
							auto parentImage = std::static_pointer_cast<ui::UIImage>(parent);
							i.image->AddTransition("card_selected_pos_x",
							                       ui::UITransitionProperties::PositionX,
							                       i.image->GetPosition().x,
							                       parentImage->GetSize().x / 2.0f - size.x * 1.125f,
							                       0.25f,
							                       easing
							);
						}
					}

					i.image->AddTransition("card_selected_size_x",
					                       ui::UITransitionProperties::SizeX,
					                       i.image->GetSize().x,
					                       size.x * 1.75f,
					                       0.25f,
					                       easing
					);

					i.image->AddTransition("card_selected_size_y",
					                       ui::UITransitionProperties::SizeY,
					                       i.image->GetSize().y,
					                       size.y * 1.75f,
					                       0.25f,
					                       easing
					);
				}
				else
				{
					i.image->AddTransition("card_selected_pos_x",
					                       ui::UITransitionProperties::PositionX,
					                       i.image->GetPosition().x,
					                       GetCardPosition(index).x - GetCardSize().x / 4.0f,
					                       0.25f,
					                       easing
					);

					i.image->AddTransition("card_selected_size_x",
					                       ui::UITransitionProperties::SizeX,
					                       i.image->GetSize().x,
					                       size.x * 1.5f,
					                       0.25f,
					                       easing
					);

					i.image->AddTransition("card_selected_size_y",
					                       ui::UITransitionProperties::SizeY,
					                       i.image->GetSize().y,
					                       size.y * 1.5f,
					                       0.25f,
					                       easing
					);

					i.image->SetInitialDepth(0.3f);
				}
			}

			index++;
		}
	}
}

void UserInterface::OnElementFocusLost(ptl::weak_ptr<ui::UIElement> element)
{
	auto card = element.lock();

	if (card)
	{
		int index = 0;
		for (auto& i : cardsInHand)
		{
			if (i.image.get() == card.get())
			{
				auto easing = ui::UITransition::easeOutQuart;
				auto size = GetCardSize();

				if (i.selected)
				{
					if (!i.image->HasTransition("card_selected_pos_x_select", ui::UITransitionProperties::PositionX))
					{
						auto parent = uiSystem->GetChildElementOfTag("PlayerCardBox").lock();

						if (parent)
						{
							if (parent->GetType() == ui::UIElementType::Image)
							{
								auto parentImage = std::static_pointer_cast<ui::UIImage>(parent);
								i.image->AddTransition("card_selected_pos_x",
								                       ui::UITransitionProperties::PositionX,
								                       i.image->GetPosition().x,
								                       parentImage->GetSize().x / 2.0f - size.x,
								                       0.25f,
								                       easing
								);
							}
						}
					}


					i.image->AddTransition("card_selected_size_x",
					                       ui::UITransitionProperties::SizeX,
					                       i.image->GetSize().x,
					                       size.x * 1.5f,
					                       0.25f,
					                       easing
					);

					i.image->AddTransition("card_selected_size_y",
					                       ui::UITransitionProperties::SizeY,
					                       i.image->GetSize().y,
					                       size.y * 1.5f,
					                       0.25f,
					                       easing
					);
				}
				else
				{
					i.image->AddTransition("card_selected_pos_x",
					                       ui::UITransitionProperties::PositionX,
					                       i.image->GetPosition().x,
					                       GetCardPosition(index).x,
					                       0.25f,
					                       easing
					);

					i.image->AddTransition("card_selected_size_x",
					                       ui::UITransitionProperties::SizeX,
					                       i.image->GetSize().x,
					                       size.x,
					                       0.25f,
					                       easing
					);

					i.image->AddTransition("card_selected_size_y",
					                       ui::UITransitionProperties::SizeY,
					                       i.image->GetSize().y,
					                       size.y,
					                       0.25f,
					                       easing
					);
					i.image->SetInitialDepth(0.2f);
				}
			}

			index++;
		}
	}
}




void UserInterface::OnEvent(ptl::weak_ptr<ui::UIElement> element, ui::UIEvent attachedEvent)
{
	
	if (attachedEvent.m_action == "start_multiplayer")
	{
		initialOkPosition = glm::vec2(0.0f, 0.0f);
		OnPressMultiplayer(element);
	}

	else if (attachedEvent.m_action == "open_credits")
	{
		this->SwitchUiIfPossible(UserInterfaceState::MainMenu, UserInterfaceState::Credits, "MainMenu.ui",
		                         "CreditsScreen.ui");
	}

	else if (attachedEvent.m_action == "close_credits")
	{
		this->SwitchUiIfPossible(UserInterfaceState::Credits, UserInterfaceState::MainMenu, "CreditsScreen.ui",
		                         "MainMenu.ui");
	}

	else if (attachedEvent.m_action == "create_lobby")
	{
		lobbyUI.OnPressCreateLobby(element);
	}
	else if (attachedEvent.m_action == "open_lobbysearch")
	{
		if (currentUserInterfaceState == UserInterfaceState::MainMenu)
		{
			lobbyUI.LobbySelectScreen();
		}
	}
	else if (attachedEvent.m_action == "join_lobby")
	{
		lobbyUI.JoinLobby(std::stoul(attachedEvent.m_data.c_str()));
	}
	else if (attachedEvent.m_action == "select_lobby_deck")
	{
		uiEQ->enqueue(UIInteractionEvent::OnPressStartGame, std::function<void*()>());
	}

	else if (attachedEvent.m_action == "show_nextdecks")
	{
		lobbyUI.ShowNextDecks();
	}
	else if (attachedEvent.m_action == "show_previousdecks")
	{
		lobbyUI.ShowPreviousDecks();
	}
	else if (attachedEvent.m_action == "selecting_deck")
	{
		if (currentUserInterfaceState == UserInterfaceState::LobbyRoom)
		{
			currentUserInterfaceState = UserInterfaceState::WaitingRoom;
			unsigned int deckNR = ptl::stoul(attachedEvent.m_data);
			lobbyEQ->enqueue(LobbyEvent::SelectDeck, [deckNR]() mutable
			{
				return static_cast<void*>(&deckNR);
			});
			uiEQ->enqueue(UIInteractionEvent::OnSwitchToWaitLobby, nullptr);
		}
	}
	else if (attachedEvent.m_action == "cancel_lobby")
	{
		lobbyUI.CancelLobby();
	}
	else if (attachedEvent.m_action == "start_game")
	{
		uiEQ->enqueue(UIInteractionEvent::OnPressStartGame, std::function<void*()>());
	}
	else if (attachedEvent.m_action == "login_pressed")
	{
		uiEQ->enqueue(UIInteractionEvent::OnPressLogin, nullptr);
	}
	else if (attachedEvent.m_action == "register_pressed")
	{
		uiEQ->enqueue(UIInteractionEvent::OnPressRegister, nullptr);
		if (currentUserInterfaceState == UserInterfaceState::EULAScreen)
		{
		}
		else
		{
			uiEQ->enqueue(UIInteractionEvent::OnPressRegister, nullptr);
		}
	}
	else if (attachedEvent.m_action == "eula_accept")
	{
		auto regBtn = uiSystem->GetChildElementOfTag("register");
		if (!regBtn.expired())
		{
			regBtn.lock()->SetVisible(true);
		}
	}
	else if (attachedEvent.m_action == "register_back_pressed")
	{
		uiEQ->enqueue(UIInteractionEvent::OnBackToLogin, nullptr);
	}
	else if (attachedEvent.m_action == "show_eula")
	{
		auto emailField = uiSystem->GetChildElementOfTag("EmailFieldText").lock();
		auto passwordField = uiSystem->GetChildElementOfTag("PasswordFieldText").lock();
		auto usernameField = uiSystem->GetChildElementOfTag("UserNameTextField").lock();

		if (usernameField && passwordField && emailField)
		{
			if (usernameField->GetType() == ui::UIElementType::Text && passwordField->GetType() == ui::UIElementType::
				Text && emailField->GetType() == ui::UIElementType::Text)

			{
				ptl::string username{std::static_pointer_cast<ui::UIText>(usernameField)->GetRawText()};
				ptl::string password{std::static_pointer_cast<ui::UIText>(passwordField)->GetRawText()};
				ptl::string email{std::static_pointer_cast<ui::UIText>(emailField)->GetRawText()};
				std::pair<ptl::string, std::pair<ptl::string, ptl::string>> loginInfo = std::make_pair(
					password, std::make_pair(email, username));
				uiEQ->enqueue(UIInteractionEvent::OnShowEULA, [loginInfo]() mutable
				{
					return static_cast<void*>(&loginInfo);
				});
			}
			else
			{
				uiSystem->Dialog("There was an error please report to support!");
			}
		}
		else
		{
			uiSystem->Dialog("There was an error please report to support!");
		}
	}
	else if (attachedEvent.m_action == "account_register")
	{
		uiEQ->enqueue(UIInteractionEvent::OnRegisterAccount, nullptr);
	}
	else if (attachedEvent.m_action == "load_right")
	{
		uiEQ->enqueue(UIInteractionEvent::OnPressEULARight, nullptr);
	}
	else if (attachedEvent.m_action == "load_left")
	{
		uiEQ->enqueue(UIInteractionEvent::OnPressEULALeft, nullptr);
	}
	else if (attachedEvent.m_action == "refresh_lobbies")
	{
		
		auto refreshButtonRef = uiSystem->GetChildElementOfTag("RefreshLobbyListButton").lock();
		auto refreshButtonImage = std::static_pointer_cast<ui::UIImage>(refreshButtonRef);
		refreshButtonImage->SetInteractable(false);
		refreshButtonImage->SetInitialDepth(0.1f);
		refreshButtonImage->SetPosition(glm::vec2(-9999.0f, -9999.9f));
		lobbyEQ->enqueue(LobbyEvent::RequestLobbyList, nullptr);
	}
	else if (attachedEvent.m_action == "show_nextlobbies")
	{
		this->lobbyUI.ShowNextLobbies();
	}
	else if (attachedEvent.m_action == "show_previouslobbies")
	{
		this->lobbyUI.ShowPreviousLobbies();
	}
	else if (attachedEvent.m_action == "QuitApplication")
	{
		uiEQ->enqueue(UIInteractionEvent::Quit, nullptr);
	}
	else if (attachedEvent.m_action == "MusicButton")
	{
		uiEQ->enqueue(UIInteractionEvent::OnAudioToggle, nullptr);
	}
	else if (attachedEvent.m_action == "back_to_menu")
	{
		this->uiSystem->PopUI("GameDemo.ui");
		this->uiSystem->PopUI("WinScreen.ui");
		this->uiSystem->PopUI("LoseScreen.ui");
		this->uiSystem->PopUI("PauseScreen.ui");
		this->uiSystem->PushUI("MainMenu.ui");
		this->currentUserInterfaceState = UserInterfaceState::MainMenu;
	}
	else if (attachedEvent.m_action == "submit_pressed")
	{
		OnPressSubmitCards(element);
	}
	else if (attachedEvent.m_action == "resume_game")
	{
		this->uiSystem->PopUI("PauseScreen.ui");
	}
}

void UserInterface::OnStartUp()
{
	uiSystem->PushUI("SplashScreen1.ui");
	currentUserInterfaceState = UserInterfaceState::SplashScreen_1;
	

	timer.Start();
}

void UserInterface::OnCheckForLoginScreen()
{
	if (currentUserInterfaceState == UserInterfaceState::SplashScreen_3)
	{
		
		if (skipLogin)
		{
			this->OnToggleLoadingScreen();
			this->UpdateLoadingScreenText("Connecting to Server...");
			currentUserInterfaceState = UserInterfaceState::Login;
			uiSystem->PopUI("SplashScreen3.ui");
			uiEQ->enqueue(UIInteractionEvent::OnLoadGameData, nullptr);
		}
		else
		{
			currentUserInterfaceState = UserInterfaceState::Login;

			this->SwitchUI("SplashScreen3.ui", "Login.ui");
			this->OnLoginScreenStartup();
		}
	}
}

void UserInterface::OnLoginScreenStartup()
{
	uiEQ->appendListener(UIInteractionEvent::OnPressLogin, [this](std::function<void*()>)
	{
		auto usernameField = uiSystem->GetChildElementOfTag("EmailFieldText").lock();
		auto passwordField = uiSystem->GetChildElementOfTag("PasswordFieldText").lock();

		if (usernameField && passwordField)
		{
			if (usernameField->GetType() == ui::UIElementType::Text && passwordField->GetType() == ui::UIElementType::
				Text)

			{
				ptl::string username{std::static_pointer_cast<ui::UIText>(usernameField)->GetRawText()};
				ptl::string password{std::static_pointer_cast<ui::UIText>(passwordField)->GetRawText()};

				if (!username.empty() && !password.empty())
				{
					this->OnToggleLoadingScreen();
					this->UpdateLoadingScreenText("Connecting to Server...");
					std::pair<ptl::string, ptl::string> loginInfo = std::make_pair(username, password);
					uiEQ->enqueue(UIInteractionEvent::OnLoginStatus, [loginInfo]() mutable
					{
						return static_cast<void*>(&loginInfo);
					});
					uiSystem->PopUI("Login.ui");
				}
				else
				{
					uiSystem->Dialog("Please fill in both fields!");
				}
			}
		}
	});

	uiEQ->appendListener(UIInteractionEvent::OnPressRegister, [this](std::function<void*()>)
	{
		SwitchUiIfPossible(UserInterfaceState::Login, UserInterfaceState::RegisterScreen, "Login.ui", "Register.ui");
	});

	uiEQ->appendListener(UIInteractionEvent::OnBackToLogin, [this](std::function<void*()>)
	{
		if (currentUserInterfaceState == UserInterfaceState::RegisterScreen)
		{
			SwitchUiIfPossible(UserInterfaceState::RegisterScreen, UserInterfaceState::Login, "Register.ui",
			                   "Login.ui");
		}
		else
		{
			SwitchUiIfPossible(UserInterfaceState::EULAScreen, UserInterfaceState::Login, "EULA.ui", "Login.ui");
		}
	});
	ui::UIText* textElement = {nullptr};
	uiEQ->appendListener(UIInteractionEvent::OnShowEULA,
	                     [this, eula = ptl::string{}, userdata = std::pair<
			                     ptl::string, std::pair<ptl::string, ptl::string>>{}, leftHandler = UIEventQueue::Handle
		                     {},
		                     rightHandler = UIEventQueue::Handle{}, registerHandler = UIEventQueue::Handle{}, counter =
		                     0,
		                     max = 600](std::function<void*()> data) mutable
	                     {
		                     userdata = *static_cast<std::pair<ptl::string, std::pair<ptl::string, ptl::string>>*>(
			                     data());
		                     if (eula.empty())
		                     {
			                     eula = tbsg::LoadEULAFromFile("eula.txt");
		                     }
		                     counter = 0;
		                     if (!leftHandler)
		                     {
			                     leftHandler = uiEQ->appendListener(UIInteractionEvent::OnPressEULALeft,
			                                                        [this,&eula,&counter,&userdata, max](
			                                                        std::function<void*()>) mutable
			                                                        {
				                                                        --counter;
				                                                        std::cout << counter << std::endl;
				                                                        if (counter >= 0 && counter < eula.size())
				                                                        {
					                                                        const auto offset = counter * max;
					                                                        uiSystem->SetTextForElementsOfTag(
						                                                        eula.substr(offset, max), "eula_text");
				                                                        }
				                                                        else if (counter > eula.size())
				                                                        {
					                                                        counter = eula.size() - max;
					                                                        counter = eula.size() / max;
				                                                        }
				                                                        else
				                                                        {
					                                                        counter = 0;
				                                                        }
			                                                        });
		                     }
		                     if (!rightHandler)
		                     {
			                     rightHandler = uiEQ->appendListener(UIInteractionEvent::OnPressEULARight,
			                                                         [this,&eula, max, &counter, &userdata](
			                                                         std::function<void*()>) mutable
			                                                         {
				                                                         counter++;
				                                                         auto offset = counter * max;
				                                                         if (offset < eula.size())
				                                                         {
					                                                         uiSystem->SetTextForElementsOfTag(
						                                                         eula.substr(offset, max), "eula_text");
				                                                         }
				                                                         else
				                                                         {
					                                                         counter -= 1;
					                                                         offset = counter * max;
					                                                         uiSystem->SetTextForElementsOfTag(
						                                                         eula.substr(offset, max), "eula_text");
				                                                         }
			                                                         });
		                     }
		                     if (!registerHandler)
		                     {
			                     registerHandler = uiEQ->appendListener(UIInteractionEvent::OnRegisterAccount,
			                                                            [this, &eula, max, &counter, &userdata](
			                                                            std::function<void*()>) mutable
			                                                            {
				                                                            uiSystem->Dialog("Call Register API here");
			                                                            });
		                     }
		                     if (SwitchUiIfPossible(UserInterfaceState::RegisterScreen, UserInterfaceState::EULAScreen,
		                                            "Register.ui", "EULA.ui"))
		                     {
			                     uiSystem->SetTextForElementsOfTag(eula.substr(0, max), "eula_text");
			                     uiSystem->GetChildElementOfTag("register").lock()->SetVisible(false);
		                     }
	                     });


	auto config = tbsg::Config::Get();
	if (!config.apiUsername.empty() && !config.apiPassword.empty())
	{
		auto usernameField = uiSystem->GetChildElementOfTag("EmailFieldText").lock();
		auto passwordField = uiSystem->GetChildElementOfTag("PasswordFieldText").lock();

		if (usernameField && passwordField)
		{
			if (usernameField->GetType() == ui::UIElementType::Text && passwordField->GetType() == ui::UIElementType::
				Text)
			{
				std::static_pointer_cast<ui::UIText>(usernameField)->SetText(config.apiUsername);
				std::static_pointer_cast<ui::UIText>(passwordField)->SetText(config.apiPassword);
				printf("Auto login using %s...\n", config.apiUsername.c_str());
				uiEQ->enqueue(UIInteractionEvent::OnPressLogin, std::function<void*()>());
			}
		}
	}
}




void UserInterface::OnPressMultiplayer(ptl::weak_ptr<ui::UIElement> element)
{
	uiEQ->enqueue(UIInteractionEvent::OnPressMultiplayer, std::function<void*()>());
}




void UserInterface::OnPressSubmitCards(ptl::weak_ptr<ui::UIElement> element)
{
	if (this->HasCardsSelected())
	{
		uiEQ->enqueue(UIInteractionEvent::OnPressSubmitCards, std::function<void*()>());
		bool enabled = false;
		uiEQ->enqueue(UIInteractionEvent::OnSetConfirmEnabled, [enabled]() mutable
		{
			return static_cast<void*>(&enabled);
		});
	}
}



void UserInterface::OnUpdateHealthPlayer(int health, int maxHealth)
{
	uiSystem->SetTextForElementsOfTag(ptl::to_string(health), "PlayerHealthText");
}

void UserInterface::OnUpdateArmorPlayer(int armor)
{
	uiSystem->SetTextForElementsOfTag(ptl::to_string(armor), "PlayerArmorText");
}

void UserInterface::OnUpdateNamePlayer(ptl::string name)
{
	uiSystem->SetTextForElementsOfTag(name, "PlayerName");
}

void UserInterface::OnUpdateIconPlayer(ptl::string image)
{
	uiSystem->SetImageForElementsOfTag(image, "PlayerIcon");
}

void UserInterface::OnUpdateDeckPlayer(int amount)
{
	uiSystem->SetTextForElementsOfTag(ptl::to_string(amount), "PlayerDeckText");
}

void UserInterface::OnUpdateDiscardPlayer(int amount)
{
	uiSystem->SetTextForElementsOfTag(ptl::to_string(amount), "PlayerDiscardText");
}



void UserInterface::OnUpdateHealthOpponent(int health, int maxHealth)
{
	uiSystem->SetTextForElementsOfTag(ptl::to_string(health), "RivalHealthText");
}

void UserInterface::OnUpdateArmorOpponent(int armor)
{
	uiSystem->SetTextForElementsOfTag(ptl::to_string(armor), "RivalArmorText");
}

void UserInterface::UpdatePlayersHandSymbol(bool isPlayer, unsigned number)
{
	ptl::string tag{(isPlayer) ? "PlayerHandText" : "OpponentHandText"};
	std::cout << tag << " updated with " << number << '\n';
	uiSystem->SetTextForElementsOfTag(ptl::to_string(number), tag);
}

void UserInterface::OnUpdateNameOpponent(ptl::string name)
{
	uiSystem->SetTextForElementsOfTag(name, "RivalName");
}

void UserInterface::OnUpdateIconOpponent(ptl::string image)
{
	uiSystem->SetImageForElementsOfTag(image, "RivalIcon");
}



void UserInterface::OnUpdateHealthMonster(int health, int maxHealth)
{
	SetHealthBarValue("MonsterHealthText", "MonsterHealthBar", health, maxHealth, true, true);
}

void UserInterface::OnUpdateArmorMonster(int armor)
{
	uiSystem->SetTextForElementsOfTag(ptl::to_string(armor), "MonsterArmorText");
}

void UserInterface::OnUpdateNameMonster(ptl::string name)
{
	uiSystem->SetTextForElementsOfTag(name, "MonsterName");
}

void UserInterface::OnUpdateIconMonster(ptl::string image)
{
	uiSystem->SetImageForElementsOfTag(image, "MonsterIcon");
}



void UserInterface::OnPushCard(int cardId, ptl::string texture)
{
	const auto parent = uiSystem->GetChildElementOfTag("PlayerCardBox").lock();

	if (parent)
	{
		if (parent->GetType() == ui::UIElementType::Image)
		{
			texture.erase(remove_if(texture.begin(), texture.end(), isspace), texture.end());
			
			auto parentImage = std::static_pointer_cast<ui::UIImage>(parent);

			auto image = ptl::make_shared<ui::UIImage>();
			image->SetSize(GetCardSize());
			image->SetTextureId(texture);
			image->SetPosition(GetCardPosition(cardsInHand.size()));
			image->SetInteractable(true);
			image->SetInitialDepth(0.2f);
			uiSystem->AddElement(parent, image);

			cardsInHand.push_back(UICard{image, cardId, false});
			std::cout << "Display card " << image << '/n';
		}
	}
}

void UserInterface::OnPopCard(int cardId)
{
	auto it = cardsInHand.begin();



	while (it != cardsInHand.end())
	{
		if ((*it).id == cardId && ((*it).selected || !this->HasCardsSelected()))
		{


			auto easing = ui::UITransition::easeInOutQuart;

			(*it).image->AddTransition("up",
				ui::UITransitionProperties::PositionY,
				(*it).image->GetPosition().y,
				1190.0f,
				0.25f,
				easing
			);



			uiSystem->RemoveElement((*it).image);

			it = cardsInHand.erase(it);

			break;
		}
		else
		{
			it++;
		}
	}

	ReorderCards();
}

void UserInterface::OnClearDeck()
{
	auto it = cardsInHand.begin();

	while (it != cardsInHand.end())
	{
		uiSystem->RemoveElement((*it).image);

		it = cardsInHand.erase(it);
	}
}

void UserInterface::OnShowReward()
{
}

void UserInterface::OnConfirmReward(ptl::weak_ptr<ui::UIElement> element)
{
	uiEQ->enqueue(UIInteractionEvent::OnConfirmReward, std::function<void*()>());
}


void UserInterface::OnWinScreen()
{
	uiSystem->PushUI("WinScreen.ui");
}

void UserInterface::OnLoseScreen()
{
	uiSystem->PushUI("LoseScreen.ui");
}

void UserInterface::OnUpdateGameTimer(unsigned int time)
{
	uiSystem->SetTextForElementsOfTag(ptl::to_string(time), "TimerText");
}

void UserInterface::OnLeaveMatch(ptl::weak_ptr<ui::UIElement> element)
{
	uiEQ->enqueue(UIInteractionEvent::OnLeaveMatch, std::function<void*()>());
}

void UserInterface::OnToggleLoadingScreen()
{
	showLoadingScreen = !showLoadingScreen;
	if (showLoadingScreen)
	{
		uiSystem->PushUI("LoadingScreen.ui");
		std::cout << "Push loading screen" << std::endl;
	}
	else
	{
		uiSystem->PopUI("LoadingScreen.ui");
		std::cout << "Pop loading screen" << std::endl;
	}
}

void UserInterface::OnSwitchToMainMenu()
{
	if (SwitchUiIfPossible(UserInterfaceState::Login, UserInterfaceState::MainMenu, "Login.ui", "MainMenu.ui"))
	{
		lobbyData.refreshLobbyView = true;
		uiSystem->SetTextForElementsOfTag("Welcome " + lobbyData.currentUserName, "PlayerName");
	}
}


void UserInterface::OnLoginStatusUpdate(const ptl::string& message)
{
	uiSystem->SetTextForElementsOfTag(ptl::to_string(message), "login_status");
}

bool UserInterface::OnPauseScreen(tbsg::InputAction action)
{
	if (action == tbsg::InputAction::EPressed)
	{
		if (currentUserInterfaceState == UserInterfaceState::GameScreen)
		{
			uiSystem->PushUI("PauseScreen.ui");
		}
	}
	return false;
}

void UserInterface::UpdateLoadingScreenText(const ptl::string& message)
{
	uiSystem->SetTextForElementsOfTag(message, "loadingText");
}

bool UserInterface::SkipSplashScreen(tbsg::InputAction a_action)
{
	if (a_action == tbsg::InputAction::EPressed)
	{
		switch (currentUserInterfaceState)
		{
		case UserInterfaceState::SplashScreen_1:
			{
				uiEQ->enqueue(UIInteractionEvent::OnSplashScreen1Ended, nullptr);
				timer.Reset();
				break;
			}
		case UserInterfaceState::SplashScreen_2:
			{
				uiEQ->enqueue(UIInteractionEvent::OnSplashScreen2Ended, nullptr);
				timer.Reset();
				break;
			}
		case UserInterfaceState::SplashScreen_3:
			{
				uiEQ->enqueue(UIInteractionEvent::OnSplashScreen3Ended, nullptr);
				timer.Stop();
				break;
			}
		default:
			break;
		}
	}
	return false;
}

void UserInterface::SwitchUI(const ptl::string& toUnload, const ptl::string& toLoad)
{
	uiSystem->PopUI(toUnload);
	uiSystem->PushUI(toLoad);
}

void UserInterface::Update(double deltaTime)
{
	timer.Update(deltaTime);
	if (currentUserInterfaceState == UserInterfaceState::SplashScreen_1)
	{
		if (timer.timePassed > 2.0)
		{
			uiEQ->enqueue(UIInteractionEvent::OnSplashScreen1Ended, nullptr);
			timer.Reset();
		}
	}
	else if (currentUserInterfaceState == UserInterfaceState::SplashScreen_2)
	{
		if (timer.timePassed > 2.0)
		{
			uiEQ->enqueue(UIInteractionEvent::OnSplashScreen2Ended, nullptr);
			timer.Reset();
		}
	}
	else if (currentUserInterfaceState == UserInterfaceState::SplashScreen_3)
	{
		if (timer.timePassed > 2.0)
		{
			uiEQ->enqueue(UIInteractionEvent::OnSplashScreen3Ended, nullptr);
			timer.Stop();
		}
	}

	else if (currentUserInterfaceState == UserInterfaceState::LobbySelect && lobbyData.refreshLobbyView)
	{
		if (timer.timePassed > 5.0)
		{
			lobbyEQ->enqueue(LobbyEvent::RequestLobbyList, nullptr);
			timer.Reset();
		}
	}
}


void UserInterface::SetConfirmEnabled(bool enabled)
{
	const bool enabledCalculated = enabled && HasCardsSelected();

	auto confirmButton = uiSystem->GetChildElementOfTag("ConfirmButton").lock();

	if (confirmButton && confirmButton->GetType() == ui::UIElementType::Image)
	{
		auto confirmImage = std::static_pointer_cast<ui::UIImage>(confirmButton);
		confirmImage->SetInteractable(enabledCalculated);

		if (initialOkPosition == glm::vec2(0.0f, 0.0f))
		{
			initialOkPosition = confirmButton->GetPosition();
		}

		if (confirmButton)
		{
			if (enabledCalculated)
			{
				auto easing = ui::UITransition::easeInOutQuart;

				confirmButton->AddTransition("confirm_button_x",
				                             ui::UITransitionProperties::PositionX,
				                             confirmButton->GetPosition().x,
				                             1920.0f / 2.0f + 150,
				                             0.25f,
				                             easing
				);

				confirmButton->AddTransition("confirm_button_y",
				                             ui::UITransitionProperties::PositionY,
				                             confirmButton->GetPosition().y,
				                             1080.0f / 2.0f,
				                             0.25f,
				                             easing
				);
			}
			else
			{
				auto easing = ui::UITransition::easeInOutQuart;

				confirmButton->AddTransition("confirm_button_x",
				                             ui::UITransitionProperties::PositionX,
				                             confirmButton->GetPosition().x,
				                             initialOkPosition.x,
				                             0.25f,
				                             easing
				);

				confirmButton->AddTransition("confirm_button_y",
				                             ui::UITransitionProperties::PositionY,
				                             confirmButton->GetPosition().y,
				                             initialOkPosition.y,
				                             0.25f,
				                             easing
				);
			}
		}
	}
}

bool UserInterface::HasCardsSelected()
{
	for (auto& element : cardsInHand)
	{
		if (element.selected)
		{
			return true;
		}
	}
	return false;
}

glm::vec2 UserInterface::GetCardSize()
{
	auto parent = uiSystem->GetChildElementOfTag("PlayerCardBox").lock();

	if (parent)
	{
		if (parent->GetType() == ui::UIElementType::Image)
		{
			auto parentImage = std::static_pointer_cast<ui::UIImage>(parent);

			return glm::vec2{parentImage->GetSize().x / 4.5, parentImage->GetSize().y};
		}
	}
	return glm::vec2();
}

glm::vec2 UserInterface::GetCardPosition(int index)
{
	auto parent = uiSystem->GetChildElementOfTag("PlayerCardBox").lock();

	if (parent)
	{
		if (parent->GetType() == ui::UIElementType::Image)
		{
			auto parentImage = std::static_pointer_cast<ui::UIImage>(parent);

			return glm::vec2{parentImage->GetSize().x / 6.5 * index, normalHeight};
		}
	}

	return glm::vec2();
}


void UserInterface::SetHealthBarValue(const ptl::string& textTag, const ptl::string& barTag, int health, int maxHealth,
                                      bool horizontal, bool inverted)
{
	auto healthElement = uiSystem->GetChildElementOfTag(barTag).lock();

	if (healthElement)
	{
		if (healthElement->GetType() == ui::UIElementType::Image)
		{
			auto healthImage = static_cast<ui::UIImage*>(healthElement.get());

			int xIn = 0;
			int yIn = 1;

			if (!horizontal)
			{
				xIn = 1;
				yIn = 0;
			}

			if (healthSizeInitial.find(barTag) == healthSizeInitial.end())
			{
				healthSizeInitial[barTag] = healthImage->GetSize()[xIn];
				healthPosInitial[barTag] = healthImage->GetPosition()[xIn];
			}

			glm::vec2 newSize{};

			newSize[xIn] = (healthSizeInitial[barTag] / static_cast<float>(maxHealth)) * static_cast<float>(health);
			newSize[yIn] = healthImage->GetSize()[yIn];

			healthImage->SetSize(newSize);

			if (inverted)
			{
				glm::vec2 newPos{};
				newPos[xIn] = healthPosInitial[barTag] + (healthSizeInitial[barTag] - newSize[xIn]);
				newPos[yIn] = healthImage->GetPosition()[yIn];
				healthImage->SetPosition(newPos);
			}
		}
	}

	ptl::stringstream text;

	ptl::string healthText = ptl::to_string(health);
	ptl::string maxHealthText = ptl::to_string(maxHealth);

	if (horizontal)
	{
		text << health;
	}
	else
	{
		for (size_t i = 0; i < healthText.length(); i++)
		{
			text << healthText[i];
			text << "\n";
		}
	}
	text << "/";
	if (horizontal)
	{
		text << maxHealth;
	}
	else
	{
		text << "\n";

		for (size_t i = 0; i < maxHealthText.length(); i++)
		{
			text << maxHealthText[i];
			text << "\n";
		}
	}

	uiSystem->SetTextForElementsOfTag(ptl::string(text.str().c_str()), textTag);
}

void UserInterface::ReorderCards()
{
	for (unsigned int i = 0; i < cardsInHand.size(); i++)
	{
		auto parent = uiSystem->GetChildElementOfTag("PlayerCardBox").lock();

		if (parent)
		{
			if (parent->GetType() == ui::UIElementType::Image)
			{
				auto parentImage = std::static_pointer_cast<ui::UIImage>(parent);
				cardsInHand[i].selected = false;

				auto easing = ui::UITransition::easeInOutQuart;

				auto pos = GetCardPosition(i);
				auto size = GetCardSize();

				cardsInHand[i].image->AddTransition("card_selected_pos_y",
				                                    ui::UITransitionProperties::PositionY,
				                                    cardsInHand[i].image->GetPosition().y,
				                                    pos.y,
				                                    0.25f,
				                                    easing
				);

				cardsInHand[i].image->AddTransition("card_selected_pos_x",
				                                    ui::UITransitionProperties::PositionX,
				                                    cardsInHand[i].image->GetPosition().x,
				                                    pos.x,
				                                    0.25f,
				                                    easing
				);

				cardsInHand[i].image->AddTransition("card_selected_size_x",
				                                    ui::UITransitionProperties::SizeX,
				                                    cardsInHand[i].image->GetSize().x,
				                                    size.x,
				                                    0.25f,
				                                    easing
				);

				cardsInHand[i].image->AddTransition("card_selected_size_y",
				                                    ui::UITransitionProperties::SizeY,
				                                    cardsInHand[i].image->GetSize().y,
				                                    size.y,
				                                    0.25f,
				                                    easing
				);
			}
		}
	}

	SetConfirmEnabled(false);
}

unsigned int UserInterface::GetIdBasedOnImage(ptl::weak_ptr<ui::UIImage> image)
{
	auto imagePointer = image.lock();
	for (auto& element : cardsInHand)
	{
		if (element.image == imagePointer)
		{
			return element.id;
		}
	}
	return 0;
}
