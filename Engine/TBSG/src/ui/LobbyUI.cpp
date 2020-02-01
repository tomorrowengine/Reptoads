#include "UserInterface.h"
#include "ui/LobbyUI.h"
#include "ui/UIElement.h"
#include "ui/UIText.h"
#include "ui/UIImage.h"
#include "ui/UISystem.h"
#include "memory/smart_ptr.h"
#include "utils/EventQueues.h"
#include "net/Packet.h"
#include <glm/vec2.hpp>
#include "net/LobbyCommands.h"

namespace ui
{
	void LobbyUI::Initialize(UserInterface* ui, LobbyUIData* lobbydata)
	{
		userinterface = ui;
		lobbyData = lobbydata;

		userinterface->lobbyEQ->appendListener(LobbyEvent::ParseLobbyInfo, [this](std::function<void* ()> data)
		{
			if (lobbyData->refreshLobbyView)
			{
				if (!lobbyData->availableLobbies.empty())
				{
					lobbyData->availableLobbies.clear();
				}
				lobbyData->availableLobbies = *static_cast<ptl::vector<LobbyDisplayInformation>*>(data());


				unsigned int amountToAdd = lobbyData->maximumLobbiesToDisplay;
				if (lobbyData->availableLobbies.size() < 6)
				{
					amountToAdd = static_cast<unsigned int>(lobbyData->availableLobbies.size());
				}
				lobbyData->currentMaxLobbyIndex = 0;
				this->OnPushLobbies(lobbyData->currentMaxLobbyIndex, amountToAdd);
			}
		});

		userinterface->uiEQ->appendListener(UIInteractionEvent::OnParseDecksData, [this](std::function<void* ()>data)
		{
			
			auto previousButtonRef = this->userinterface->uiSystem->GetChildElementOfTag("PreviousDecksButton").lock();
			if (previousButtonRef) { previousButtonRef->SetVisible(true); }

			auto nextButtonRef = this->userinterface->uiSystem->GetChildElementOfTag("NextDecksButton").lock();
			if (nextButtonRef) { nextButtonRef->SetVisible(true); }

			if (!lobbyData->availableDecks.empty())
			{
				lobbyData->availableDecks.clear();
			}
			lobbyData->availableDecks = *static_cast<ptl::vector<DeckDisplayInformation>*>(data());

			unsigned int amountToAdd = lobbyData->maximumDecksToDisplay;
			if (lobbyData->availableDecks.size() < 2)
			{
				amountToAdd = static_cast<unsigned int>(lobbyData->availableDecks.size());
			}
			lobbyData->currentMaxDeckIndex = 0;
			this->OnPushDecks(lobbyData->currentMaxDeckIndex, amountToAdd);
		});

		userinterface->lobbyEQ->appendListener(LobbyEvent::LobbyError, [this](std::function<void* ()> data)
			{


				const auto Dialog = [this](auto mesg)
				{
					this->userinterface->uiSystem->Dialog(mesg, [this]()
					{
						this->userinterface->SwitchUiIfPossible(UserInterfaceState::GameScreen, UserInterfaceState::LobbySelect, "LobbyRoom.ui", "LobbySelect.ui");
						this->userinterface->SwitchUiIfPossible(UserInterfaceState::LobbyRoom, UserInterfaceState::LobbySelect, "LobbyRoom.ui", "LobbySelect.ui");
						this->userinterface->SwitchUiIfPossible(UserInterfaceState::WaitingRoom, UserInterfaceState::LobbySelect, "LobbyRoom.ui", "LobbySelect.ui");
						this->userinterface->lobbyEQ->enqueue(LobbyEvent::RequestLobbyList, nullptr);
						this->userinterface->currentUserInterfaceState = UserInterfaceState::LobbySelect;
						this->userinterface->timer.Start();
					});
				};

				auto packet = *static_cast<Packet*>(data());
				
				unsigned lobbyErrorCode{};
				packet >> lobbyErrorCode;
				tbsg::LobbyError lobbyError{static_cast<tbsg::LobbyError>(lobbyErrorCode)};
				switch(lobbyError)
				{
				case tbsg::LobbyError::Unknown:
					Dialog("An unexpected error occurred, leaving lobby...");
					break;
				case tbsg::LobbyError::Full:
					Dialog("Lobby is full");
					break;
				case tbsg::LobbyError::Exist:
					Dialog("Lobby Exist");
					break;
				case tbsg::LobbyError::Started: 
					Dialog("Lobby Started");
					break;
				case tbsg::LobbyError::AlreadyJoined: 
					Dialog("Lobby AlreadyJoined");
					break;
				case tbsg::LobbyError::OnlyTheOwnerCanStartTheGame: break;
				case tbsg::LobbyError::NoGameServerAvailable:
					Dialog("Lobby NoGameServerAvailable");
					break;
				case tbsg::LobbyError::NoDeckSelected:
					Dialog("Lobby NoGameServerAvailable");
					break;
				}

			});

		ui->lobbyEQ->appendListener(LobbyEvent::JoinExistingLobby, [this](std::function<void* ()>)
			{
				this->SetupCurrentLobbyInfo(CurrentLobbyUpdateInfo::OpponentName, lobbyData->currentUserName);
				this->userinterface->uiEQ->enqueue(UIInteractionEvent::OnRequestDeckData, nullptr);
				this->userinterface->SwitchUiIfPossible(UserInterfaceState::LobbySelect, UserInterfaceState::LobbyRoom, "LobbySelect.ui", "LobbyRoom.ui");
				this->userinterface->timer.Stop();
				this->TempHideStatusText();
			});


		ui->lobbyEQ->appendListener(LobbyEvent::SuccessfulLobbyCreated, [this](std::function<void* ()>)
		{
			this->userinterface->uiEQ->enqueue(UIInteractionEvent::OnRequestDeckData, nullptr);
			this->userinterface->SwitchUiIfPossible(UserInterfaceState::LobbySelect, UserInterfaceState::LobbyRoom, "LobbySelect.ui", "LobbyRoom.ui");
			this->userinterface->timer.Stop();
			this->TempHideStatusText();
		});

		ui->uiEQ->appendListener(UIInteractionEvent::OnSwitchToWaitLobby, [this](std::function<void* ()>)
			{
	
				this->DisableLobbyCardSelecting();
				lobbyData->displayWaitingInfo = true;
				if (lobbyData->currentLobby.isOwnerOfLobby)
				{
					this->SetupCurrentLobbyInfo(CurrentLobbyUpdateInfo::OwnerIsWaitingForOpponent, ptl::string{}, true);
				}
				else
				{
					this->SetupCurrentLobbyInfo(CurrentLobbyUpdateInfo::OpponentIsWaitingForOwnerDeck, ptl::string{}, false);
				}
			});
		

		ui->uiEQ->appendListener(UIInteractionEvent::OnSetConfirmEnabled, [this](std::function<void* ()> data)
		{
			bool enabled = *static_cast<bool*>(data());
			this->userinterface->SetConfirmEnabled(enabled);
		});
	
		
		
		
	
		ui->lobbyEQ->appendListener(LobbyEvent::LobbyLeft, [this](std::function<void* ()>)
		{
				this->userinterface->SwitchUiIfPossible(UserInterfaceState::LobbyRoom, UserInterfaceState::LobbySelect, "LobbyRoom.ui", "LobbySelect.ui");
				this->userinterface->timer.Start();
		});

		ui->lobbyEQ->appendListener(LobbyEvent::OnServerDisconnect, [this](std::function<void* ()>)
		{
			if(!lobbyData->startedGame){
					userinterface->uiSystem->Dialog("You lost connection to the server.", [this]()
				{
					this->ForceBackToLogin();
				});
			}
		});


		ui->lobbyEQ->appendListener(LobbyEvent::AccountInUse, [this](std::function<void* ()>)
		{

			userinterface->uiSystem->Dialog("The Account is already in use!", [this]()
			{
				this->ForceBackToLogin();
			});
		});

		ui->lobbyEQ->appendListener(LobbyEvent::OnServerConnect, [this](std::function<void* ()>)
		{
			lobbyData->startedGame = false;
			userinterface->uiEQ->enqueue(UIInteractionEvent::OnSwitchToMainMenu, nullptr);
			userinterface->uiEQ->enqueue(UIInteractionEvent::OnToggleLoadingScreen, nullptr);
		});
		ui->lobbyEQ->appendListener(LobbyEvent::LobbyGameStarted, [this](std::function<void* ()>)
		{
			lobbyData->startedGame = true;
			if (this->userinterface->SwitchUiIfPossible(UserInterfaceState::WaitingRoom, UserInterfaceState::GameScreen, "LobbyRoom.ui", "GameDemo.ui"))
			{
				this->userinterface->uiEQ->enqueue(UIInteractionEvent::OnToggleLoadingScreen,nullptr);
				this->userinterface->uiSystem->PopUI("MainMenu.ui");
				this->lobbyData->refreshLobbyView = false;
			}
		});
		ui->lobbyEQ->appendListener(LobbyEvent::LobbyGameStopped, [this](std::function<void* ()>)
		{
			lobbyData->startedGame = false;
		});
		
		ui->lobbyEQ->appendListener(LobbyEvent::LobbyPlayerJoined, [this](std::function<void* ()>)
			{
				this->SetupCurrentLobbyInfo(CurrentLobbyUpdateInfo::OwnerIsWaitingForOpponentDeck, ptl::string{}, true);
			});

		ui->lobbyEQ->appendListener(LobbyEvent::LobbyPlayerLeft, [this](std::function<void* ()>)
			{
				this->SetupCurrentLobbyInfo(CurrentLobbyUpdateInfo::OwnerIsWaitingForOpponent, ptl::string{}, true);
			});

		ui->lobbyEQ->appendListener(LobbyEvent::SelectDeck, [this](std::function<void* ()>)
			{
				lobbyData->ownerDeckConfirmed = true;
				this->CheckForDeckConfirmation();
			});

		ui->lobbyEQ->appendListener(LobbyEvent::OpponentSelectDeck, [this](std::function<void* ()>)
			{
				lobbyData->opponentDeckConfirmed = true;
				this->CheckForDeckConfirmation();
			});
			
			
	}

	void LobbyUI::ShowNextLobbies()
	{
		const auto indexCount = this->lobbyData->currentMaxLobbyIndex + this->lobbyData->maximumLobbiesToDisplay;
		if (indexCount <= this->lobbyData->availableLobbies.size())
		{
			this->lobbyData->currentMaxLobbyIndex += this->lobbyData->maximumLobbiesToDisplay;
			this->OnPushLobbies(this->lobbyData->currentMaxLobbyIndex, this->lobbyData->maximumLobbiesToDisplay);
		}
	}

	void LobbyUI::ShowPreviousLobbies()
	{
		const auto indexCount = static_cast<int>(this->lobbyData->currentMaxLobbyIndex - this->lobbyData->maximumLobbiesToDisplay);
		if (indexCount >= 0)
		{
			this->lobbyData->currentMaxLobbyIndex -= this->lobbyData->maximumLobbiesToDisplay;
			this->OnPushLobbies(this->lobbyData->currentMaxLobbyIndex, this->lobbyData->maximumLobbiesToDisplay);
		}
	}

	void LobbyUI::OnPushLobbies(const unsigned int startIndex, unsigned int amountToAdd)
	{
		auto parent = this->userinterface->uiSystem->GetChildElementOfTag("FoundLobbiesBox").lock();
		if (parent)
		{
			if (parent->GetType() == ui::UIElementType::Image)
			{
				
				auto children = parent->GetChildren();
				if (!children.empty())
				{
					for (auto child : children)
					{
						this->userinterface->uiSystem->RemoveElement(child);
					}
				}

				
				auto previousButtonRef = this->userinterface->uiSystem->GetChildElementOfTag("PreviousLobbiesButton").lock();
				if (previousButtonRef) { previousButtonRef->SetVisible(false); }

				auto nextButtonRef = this->userinterface->uiSystem->GetChildElementOfTag("NextLobbiesButton").lock();
				if (nextButtonRef) { nextButtonRef->SetVisible(false); }

				
				auto refreshButtonRef = this->userinterface->uiSystem->GetChildElementOfTag("RefreshLobbyListButton").lock();
				if (refreshButtonRef) { refreshButtonRef->SetVisible(false); }

				auto totalLobbies = static_cast<unsigned int>(this->lobbyData->availableLobbies.size());

				if (totalLobbies >= amountToAdd)
				{
					if ((startIndex)+amountToAdd > totalLobbies)
					{
						amountToAdd = totalLobbies - amountToAdd;
					}

					
					if (totalLobbies > startIndex)
					{
						if (nextButtonRef) { nextButtonRef->SetVisible(true); }
					}

					
					if (startIndex + 1 > this->lobbyData->maximumLobbiesToDisplay)
					{
						if (previousButtonRef) { previousButtonRef->SetVisible(true); }
					}
					this->DisplayAllAvailableLobbies(parent, startIndex, amountToAdd);
				}
			}
		}
	}


	void LobbyUI::OnPressCreateLobby(ptl::weak_ptr<ui::UIElement> element)
	{
		this->userinterface->lobbyEQ->enqueue(LobbyEvent::CreateNewLobby, std::function<void* ()>());
	}

	void LobbyUI::CancelLobby()
	{
		
		this->userinterface->SwitchUiIfPossible(UserInterfaceState::LobbyRoom, UserInterfaceState::LobbySelect, "LobbyRoom.ui", "LobbySelect.ui");
		this->userinterface->SwitchUiIfPossible(UserInterfaceState::WaitingRoom, UserInterfaceState::LobbySelect, "LobbyRoom.ui", "LobbySelect.ui");
		this->userinterface->lobbyEQ->enqueue(LobbyEvent::DisconnectFromLobby, nullptr);
		this->userinterface->lobbyEQ->enqueue(LobbyEvent::RequestLobbyList, nullptr);
		this->userinterface->currentUserInterfaceState = UserInterfaceState::LobbySelect;
		this->userinterface->timer.Start();
	}

	void LobbyUI::DisplayAllAvailableLobbies(ptl::shared_ptr<ui::UIElement> parent, const unsigned int startLobbyIndex, const unsigned int amountToAdd)
	{
		auto* uiSystem = this->userinterface->uiSystem;

		auto lobbyText = uiSystem->GetChildElementOfTag("LobbyStatus").lock();
		if (lobbyText)
		{
			if (lobbyText->GetType() == ui::UIElementType::Text)
			{
				auto text = std::static_pointer_cast<ui::UIText>(lobbyText);
				text->SetText("Fetching available lobbies...");
			}
		}

		auto parentImage = std::static_pointer_cast<ui::UIImage>(parent);
		auto parentSize = parentImage->GetSize();

		const ptl::string imageTexture = "Lobby_NameDisplayBackground.png";
		const ptl::string buttonTexture = "Lobby_ButtonJoin.png";
		const unsigned int totalIndex = startLobbyIndex + amountToAdd;
		unsigned int count = 0;
		const unsigned int imageHeight = parentSize.y / 6;
		for (size_t i = startLobbyIndex; i < totalIndex; ++i)
		{
			
			auto image = ptl::make_shared<ui::UIImage>();
			image->SetSize(glm::vec2(parentSize.x - 50.0f, imageHeight * 0.75f));
			image->SetTextureId(imageTexture);
			image->SetPosition(glm::vec2(25.0f, (parentSize.y - imageHeight) - (count * (imageHeight)) + 10.0f));
			image->SetInteractable(false);
			image->SetInitialDepth(0.2f);
			uiSystem->AddElement(parent, image);

			
			const glm::vec2 imageSize = image->GetSize();
			auto name = ptl::make_shared<ui::UIText>();
			const ptl::string lobbyName = this->lobbyData->availableLobbies[i].ownerName;
			name->SetFontSize(32);
			name->SetFontPath("Acme");
			name->SetColor(glm::vec4(1, 1, 1, 1));
			name->SetText(lobbyName);
			name->SetInitialDepth(0.35f);
			name->SetSize(glm::vec2(imageSize.x / 2, imageSize.y));
			name->SetPosition(glm::vec2(50.0f, -18.0f));
			uiSystem->AddElement(image, name);

			
			auto hightlight = ptl::make_shared<ui::UIText>();
			hightlight->SetFontSize(32);
			hightlight->SetFontPath("Acme");
			hightlight->SetColor(glm::vec4(0, 0, 0, 1));
			hightlight->SetText(lobbyName);
			hightlight->SetInitialDepth(0.3f);
			hightlight->SetSize(glm::vec2(imageSize.x / 2, imageSize.y));
			hightlight->SetPosition(glm::vec2(52.0f, -20.0f));
			uiSystem->AddElement(image, hightlight);

			ui::UIEvent newEvent;
			newEvent.m_type = "OnButtonReleased";
			newEvent.m_action = "join_lobby";
			newEvent.m_data = ptl::to_string(this->lobbyData->availableLobbies[i].lobbyID);

			auto button = ptl::make_shared<ui::UIImage>();
			button->SetTextureId(buttonTexture);
			button->SetSize(glm::vec2( (imageSize.y + 25.f), (imageSize.y - 15.0f) ));
			button->SetPosition(glm::vec2( imageSize.x - (imageSize.y * 2), 7.5f ));
			button->SetInteractable(true);
			button->SetInitialDepth(0.35f);
			button->AddEvent(newEvent);

			
			newEvent.m_type = "OnFocussed";
			newEvent.m_action = "setImage";
			newEvent.m_data = "Lobby_ButtonJoin-Hovered.png";
			button->AddEvent(newEvent);

			newEvent.m_type = "OnFocusLost";
			newEvent.m_data = "Lobby_ButtonJoin.png";
			button->AddEvent(newEvent);

			uiSystem->AddElement(image, button);
			count++;
		}

		if (count != 0)
		{
			auto lobbyStatus = uiSystem->GetChildElementOfTag("LobbyStatus").lock();
			if (lobbyStatus)
			{
				if (lobbyStatus->GetType() == ui::UIElementType::Text)
				{
					auto text = std::static_pointer_cast<ui::UIText>(lobbyStatus);
					text->SetText("");
				}
			}
		}
		else if (count == 0)
		{
			auto lobbyStatus = uiSystem->GetChildElementOfTag("LobbyStatus").lock();
			if (lobbyStatus)
			{
				if (lobbyStatus->GetType() == ui::UIElementType::Text)
				{
					auto text = std::static_pointer_cast<ui::UIText>(lobbyStatus);
					text->SetText("No lobbies found!");
				}
			}
		}
	}

	void LobbyUI::OnPushDecks(const unsigned int startIndex, unsigned int amountToAdd)
	{
		auto parent = userinterface->uiSystem->GetChildElementOfTag("LobbyRoom").lock();
		if (parent)
		{
			if (parent->GetType() == ui::UIElementType::Image)
			{
				
				auto previousButtonRef = userinterface->uiSystem->GetChildElementOfTag("PreviousDecksButton").lock();
				auto previousButtonImage = std::static_pointer_cast<ui::UIImage>(previousButtonRef);
				previousButtonImage->SetInteractable(false);
				previousButtonImage->SetPosition(glm::vec2(-9999.0f, -9999.0f));
				auto nextButtonRef = userinterface->uiSystem->GetChildElementOfTag("NextDecksButton").lock();
				auto nextButtonImage = std::static_pointer_cast<ui::UIImage>(nextButtonRef);
				nextButtonImage->SetInteractable(false);
				nextButtonImage->SetPosition(glm::vec2(-9999.0f, -9999.0f));

				auto totalDecks = static_cast<unsigned int>(lobbyData->availableDecks.size());

				if (totalDecks >= amountToAdd)
				{

					if ((startIndex)+amountToAdd > totalDecks)
					{
						amountToAdd = totalDecks - startIndex;
					}

					
					if (startIndex + amountToAdd < totalDecks)
					{
						nextButtonImage->SetInteractable(true);
					}

					
					if ((startIndex + amountToAdd) > lobbyData->maximumDecksToDisplay)
					{
						previousButtonImage->SetInteractable(true);
					}
					this->DisplayCurrentAvailableDecks(startIndex, amountToAdd);
				}
			}
		}
	}

	void LobbyUI::LobbySelectScreen()
	{
		auto playButtonRef = this->userinterface->uiSystem->GetChildElementOfTag("MainMenuPlayButton").lock();
		if (playButtonRef)
		{
			auto button = std::static_pointer_cast<ui::UIImage>(playButtonRef);
			button->SetInteractable(false);
		}
		auto creditsButtonRef = this->userinterface->uiSystem->GetChildElementOfTag("MainMenuCreditsButton").lock();
		if (creditsButtonRef)
		{
			creditsButtonRef->SetVisible(false);
		}

		this->userinterface->uiSystem->PushUI("LobbySelect.ui");

		this->userinterface->currentUserInterfaceState = UserInterfaceState::LobbySelect;
		this->userinterface->lobbyEQ->enqueue(LobbyEvent::RequestLobbyList, nullptr);
		this->userinterface->timer.Start();
	}

	void LobbyUI::JoinLobby(unsigned int attachedLobbyID)
	{
		if (this->userinterface->currentUserInterfaceState == UserInterfaceState::LobbySelect)
		{
			OnPressJoinLobby(attachedLobbyID, 0);
		}
	}

	void LobbyUI::DisplayCurrentAvailableDecks(const unsigned int startDeckIndex, const unsigned int amountToAdd)
	{
		const unsigned int totalIndex = startDeckIndex + amountToAdd;
		unsigned int count = 0;
		
		ptl::shared_ptr<ui::UIElement> currentCard = nullptr;
		ptl::shared_ptr<ui::UIElement> currentText = nullptr;
		if (amountToAdd == 1)
		{

			currentCard = this->userinterface->uiSystem->GetChildElementOfTag("Deck2_Box").lock();
			currentText = this->userinterface->uiSystem->GetChildElementOfTag("DeckName2").lock();
			if (currentCard && currentText)
			{
				auto cardElement = std::static_pointer_cast<ui::UIImage>(currentCard);
				cardElement->SetVisible(false);
				auto textElement = std::static_pointer_cast<ui::UIText>(currentText);
				textElement->SetText("");
			}
			else
			{
				std::cout << "[UserInterface] Dynamic decks cannot be set, requested element not found!!" << std::endl;
			}
		}

		for (size_t i = startDeckIndex; i < totalIndex; i++)
		{
			if (count == 0)
			{
				currentCard = this->userinterface->uiSystem->GetChildElementOfTag("Deck1_Box").lock();
				currentText = this->userinterface->uiSystem->GetChildElementOfTag("DeckName1").lock();
			}
			else
			{
				currentCard = this->userinterface->uiSystem->GetChildElementOfTag("Deck2_Box").lock();
				currentText = this->userinterface->uiSystem->GetChildElementOfTag("DeckName2").lock();
				currentCard->SetVisible(true);
			}
			if (currentCard && currentText)
			{
				const auto deckInfo = &this->lobbyData->availableDecks[i];

				
				auto cardElement = std::static_pointer_cast<ui::UIImage>(currentCard);

				ptl::string cardFilePath = "Card_" + deckInfo->coverCardId + ".png";
				ptl::string cardFilePathGray = "Card_" + deckInfo->coverCardId + "_Grey.png";
				cardFilePath.erase(remove_if(cardFilePath.begin(), cardFilePath.end(), isspace), cardFilePath.end());
				cardFilePathGray.erase(remove_if(cardFilePathGray.begin(), cardFilePathGray.end(), isspace), cardFilePathGray.end());

				if (!deckInfo->coverCardId.empty())
				{
					cardElement->SetTextureId(cardFilePathGray);
				}
				else
				{
					cardElement->SetTextureId("NoGoodBike.jpg");
				}

				ui::UIEvent newEvent{};
				newEvent.m_type = "OnButtonReleased";
				newEvent.m_action = "selecting_deck";
				newEvent.m_data = ptl::to_string(this->lobbyData->availableDecks[i].deckID);
				cardElement->AddEvent(newEvent);

				newEvent.m_type = "OnFocussed";
				newEvent.m_action = "setImage";
				newEvent.m_data = cardFilePath;
				cardElement->AddEvent(newEvent);

				newEvent.m_type = "OnFocusLost";
				newEvent.m_action = "setImage";
				newEvent.m_data = cardFilePathGray;
				cardElement->AddEvent(newEvent);

				
				auto textElement = std::static_pointer_cast<ui::UIText>(currentText);
				if (!deckInfo->deckName.empty())
				{
					textElement->SetText(deckInfo->deckName);
				}
				else
				{
					textElement->SetText("Unnamed Deck");
				}
				count++;
			}
			else
			{
				std::cout << "[UserInterface] Dynamic decks cannot be set, requested elements not found!!" << std::endl;
			}
		}
	}

	void LobbyUI::ShowNextDecks()
	{
		const auto indexCount = this->lobbyData->currentMaxDeckIndex + this->lobbyData->maximumDecksToDisplay;
		if (indexCount <= this->lobbyData->availableDecks.size())
		{
			this->lobbyData->currentMaxDeckIndex += this->lobbyData->maximumDecksToDisplay;
			this->OnPushDecks(this->lobbyData->currentMaxDeckIndex, this->lobbyData->maximumDecksToDisplay);
		}
	}

	void LobbyUI::ShowPreviousDecks()
	{
		const auto indexCount = static_cast<int>(this->lobbyData->currentMaxDeckIndex - this->lobbyData->maximumDecksToDisplay);
		if (indexCount >= 0)
		{
			this->lobbyData->currentMaxDeckIndex -= this->lobbyData->maximumDecksToDisplay;
			this->OnPushDecks(this->lobbyData->currentMaxDeckIndex, this->lobbyData->maximumDecksToDisplay);
		}
	}

	void LobbyUI::TempHideStatusText()
	{
		
		auto statusBox = this->userinterface->uiSystem->GetChildElementOfTag("WaitingForPlayerBox").lock();
		if (statusBox) { statusBox->SetVisible(false); }

		auto ptext = this->userinterface->uiSystem->GetChildElementOfTag("WaitingForPlayerText").lock();
		if (ptext) { ptext->SetVisible(false); }

		auto playstatus1 = this->userinterface->uiSystem->GetChildElementOfTag("Player1StatusText").lock();
		if (playstatus1) { playstatus1->SetVisible(false); }

		auto playstatus2 = this->userinterface->uiSystem->GetChildElementOfTag("Player2StatusText").lock();
		if (playstatus2) { playstatus2->SetVisible(false); }

	}

	void LobbyUI::DisableLobbyCardSelecting()
	{
		ui::UIEvent dummyEvent{};

		auto cardBox1 = this->userinterface->uiSystem->GetChildElementOfTag("Deck1_Box").lock();
		if (cardBox1)
		{
			cardBox1->SetVisible(false);
			cardBox1->SetEvent(dummyEvent);
		}

		auto cardBox2 = this->userinterface->uiSystem->GetChildElementOfTag("Deck2_Box").lock();
		if (cardBox2)
		{
			cardBox2->SetVisible(false);
			cardBox2->SetEvent(dummyEvent);
		}
		auto cardText1 = this->userinterface->uiSystem->GetChildElementOfTag("DeckName1").lock();
		if (cardText1) 
		{
			auto cardTextElemennt1 = std::static_pointer_cast<ui::UIText>(cardText1);
			if (cardTextElemennt1) { cardTextElemennt1->SetText(""); }
		}
		auto cardText2 = this->userinterface->uiSystem->GetChildElementOfTag("DeckName2").lock();
		if (cardText2)
		{
			auto cardTextElemennt2 = std::static_pointer_cast<ui::UIText>(cardText2);
			if (cardTextElemennt2) { cardTextElemennt2->SetText(""); }
		}
	}


	void LobbyUI::SetupWaitingScreen()
	{
		auto ptext = this->userinterface->uiSystem->GetChildElementOfTag("WaitingForPlayerText").lock();
		if (ptext) { ptext->SetVisible(true); }

		
		ptl::shared_ptr<ui::UIElement> currentPlayer = nullptr;
		ptl::shared_ptr<ui::UIElement> currentStatus = nullptr;
		ptl::string currentPlayerString{};
		bool currentStatusBool = false;
		
		for (size_t i = 0; i < 1; ++i)
		{
			if (i == 0)
			{
				currentPlayerString = lobbyData->currentLobby.ownerName;
				currentStatusBool = lobbyData->currentLobby.ownerReady;
				currentPlayer = this->userinterface->uiSystem->GetChildElementOfTag("PlayerWaiting1Text").lock();
				currentStatus = this->userinterface->uiSystem->GetChildElementOfTag("Player1StatusText").lock();
			}
			else
			{
				currentPlayerString = lobbyData->currentLobby.opponentName;
				currentStatusBool = lobbyData->currentLobby.opponentReady;
				currentPlayer = this->userinterface->uiSystem->GetChildElementOfTag("PlayerWaiting2Text").lock();
				currentStatus = this->userinterface->uiSystem->GetChildElementOfTag("Player2StatusText").lock();
			}

		}

		

		if (currentPlayer)
		{
			currentPlayer->SetVisible(true);
			auto currentPlayerText = std::static_pointer_cast<ui::UIText>(currentPlayer);
			if (currentPlayerText)
			{
				if (currentPlayerString.empty())
				{
					currentPlayerText->SetText("No opponent");
				}
				else
				{
					currentPlayerText->SetText(currentPlayerString);
				}
			}
		}
		if (currentStatus)
		{
			currentStatus->SetVisible(true);
			auto currentStatusText = std::static_pointer_cast<ui::UIText>(currentStatus);
			if (currentStatusText)
			{
				ptl::string status{};
				if (currentStatusBool)
				{
					status = "Ready";
				}
				else
				{
					status = "Not Ready";
				}
				currentStatusText->SetText(status);
			}
		}
	}


	void LobbyUI::SetupCurrentLobbyInfo(CurrentLobbyUpdateInfo whatToUpdate, const ptl::string updateText, const bool updateState)
	{
		switch (whatToUpdate)
		{
			case CurrentLobbyUpdateInfo::OwnerName:
			{
				lobbyData->currentLobby.ownerName = updateText;
				lobbyData->currentLobby.isOwnerOfLobby = true;
				break;
			}
			case CurrentLobbyUpdateInfo::OwnerReady:
			{
				lobbyData->currentLobby.ownerReady = updateState;
				std::cout << "UpdatedCurrentInfo OwnerReady " << std::endl;
				break;
			}
			case CurrentLobbyUpdateInfo::OpponentName:
			{
				lobbyData->currentLobby.opponentName = updateText;
				lobbyData->currentLobby.isOwnerOfLobby = false;
			}
			case CurrentLobbyUpdateInfo::OpponentReady:
			{
				lobbyData->currentLobby.opponentReady = updateState;
				break;
			}
			case CurrentLobbyUpdateInfo::OwnerIsWaitingForOpponent:
			case CurrentLobbyUpdateInfo::OpponentLeft:
			{
				if (lobbyData->displayWaitingInfo)
				{
					auto ptext = this->userinterface->uiSystem->GetChildElementOfTag("WaitingForPlayerText").lock();
					if (ptext) {
						auto text = std::static_pointer_cast<ui::UIText>(ptext);
						text->SetVisible(true);
						text->SetText("Waiting for players 1/2");
					}
				}
				break;

			}
			case CurrentLobbyUpdateInfo::OwnerIsWaitingForOpponentDeck:
			case CurrentLobbyUpdateInfo::OpponentIsWaitingForOwnerDeck:
			{
				if (lobbyData->displayWaitingInfo)
				{
					auto ptext = this->userinterface->uiSystem->GetChildElementOfTag("WaitingForPlayerText").lock();
					if (ptext) {
						auto text = std::static_pointer_cast<ui::UIText>(ptext);
						text->SetVisible(true);
						text->SetText("Opponent is selecting a deck.");
					}
					break;
				}
			}
			case CurrentLobbyUpdateInfo::StartingLobby:
			{
				if (lobbyData->displayWaitingInfo)
				{
					auto ptext = this->userinterface->uiSystem->GetChildElementOfTag("WaitingForPlayerText").lock();
					if (ptext) {
						auto text = std::static_pointer_cast<ui::UIText>(ptext);
						text->SetVisible(true);
						text->SetText("Starting Game...");
					}
				}
				break;
			}
		}

	}

	void LobbyUI::ForceBackToLogin()
	{
		userinterface->uiSystem->Clear();
		userinterface->uiSystem->PushUI("Login.ui");
		userinterface->currentUserInterfaceState = UserInterfaceState::Login;
	}

	void LobbyUI::CheckForDeckConfirmation()
	{
		if (lobbyData->ownerDeckConfirmed && lobbyData->opponentDeckConfirmed)
		{
			this->SetupCurrentLobbyInfo(CurrentLobbyUpdateInfo::StartingLobby);
			userinterface->lobbyEQ->enqueue(LobbyEvent::RequestToStartGame, nullptr);
		}
		else
		{
			std::cout << "Cannot start game, 1 remaining deck select." << std::endl;
		}
	}

	void LobbyUI::OnPressJoinLobby(unsigned int lobbyID, unsigned int deckNR)
	{
		if (lobbyID != 0)
		{
			this->userinterface->lobbyEQ->enqueue(LobbyEvent::JoinExistingLobby, [lobbyID]() mutable
			{
				return static_cast<void*>(&lobbyID);
			});
		}
	}
}
