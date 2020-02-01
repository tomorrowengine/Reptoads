#include "Game.h"
#include "audio/AudioSystem.h"
#include "GameDatabase.h"
#include "EffectChangeHandler.h"
#include "SceneHandler.h"
#include "gameplay/Transform.h"


Game::Game(ptl::shared_ptr<UIEventQueue> uiEventQueue, tbsg::SceneHandler* sceneSystem,
           tbsg::ClientMatchState* clientMatch, audio::AudioSystem* audio, GameDatabase* gameDatabase,
           gfx::Camera* camera,
           core::DeltaTime& deltaTime): sceneHandler(sceneSystem), clientMatchState(clientMatch), audioSystem(audio),
                                        database(gameDatabase), camera(camera),
                                        deltaTime(deltaTime), uiQueue(uiEventQueue)
{
}


void Game::Initialize(ptl::shared_ptr<GameEventQueue> evQueue, ptl::shared_ptr<LobbyEventQueue> lobbyEventQueue)
{
	gameEvents = evQueue;
	lobbyQueue = lobbyEventQueue;
	gameEvents->appendListener(GameEvent::PlayerConnected,
	                           [this](Packet&& packet) { OnPlayerConnected(std::move(packet)); });

	gameEvents->appendListener(GameEvent::LobbyDataReceived,
	                           [this](Packet&& packet) { OnLobbyDataReceived(std::move(packet)); });
	gameEvents->appendListener(GameEvent::GameHasStarted,
	                           [this](Packet&& packet) { OnGameStarted(std::move(packet)); });
	gameEvents->appendListener(GameEvent::OpponentOnCardHover,
	                           [this](Packet&& packet) { OpponentOnCardHover(std::move(packet)); });
	gameEvents->appendListener(GameEvent::OpponentOnCardSelected,
	                           [this](Packet&& packet) { OpponentOnCardSelected(std::move(packet)); });
	gameEvents->appendListener(GameEvent::OpponentOnCardsSubmitted,
	                           [this](Packet&& packet)
	                           {
		                           unsigned int amount;
		                           packet >> amount;

		                           ptl::vector<unsigned int> selectedCards{};
		                           for (unsigned int i = 0; i < amount; ++i)
		                           {
			                           unsigned int cardID;
			                           packet >> cardID;
			                           selectedCards.push_back(cardID);

			                           
			                           if (cardID != 0)
			                           {
				                           CardSelectedVisual(1, cardID);
			                           }
			                           
		                           }
	                           });
	gameEvents->appendListener(GameEvent::TurnResult,
	                           [this](Packet&& packet)
	                           {
		                           EffectChangeHandler::RemoveCard(sceneHandler, 0);
		                           EffectChangeHandler::RemoveCard(sceneHandler, 1);
		                           ProcessChanges(std::move(packet));
	                           });
	gameEvents->appendListener(GameEvent::PlayerDisconnected,
	                           [this](Packet&& packet) { ProcessChanges(std::move(packet)); });
	gameEvents->appendListener(GameEvent::OnServerDisconnect,
	                           [this](Packet&& packet)
	{
		uiQueue->enqueue(UIInteractionEvent::OnToggleLoadingScreen, nullptr);
		uiQueue->enqueue(UIInteractionEvent::OnServerDisconnect, nullptr);
		audioSystem->StopMusic();
		sceneHandler->UnloadMap();
		audioSystem->StartMusic(0);
		std::cout << "OnServerDisconnect has ben triggered\n";

	});

	
	gameEvents->appendListener(GameEvent::MatchDataReceived,
	                           [this](Packet&& packet)
	                           {
		                           this->ProcessMatchData(std::move(packet));
	                           });


	uiQueue->appendListener(UIInteractionEvent::OnSelectCard, [this](std::function<void*()> data)
	{
		auto card = *static_cast<unsigned int*>(data());
		selectedCards = card;
	});

	uiQueue->appendListener(UIInteractionEvent::OnDeselectCard, [this](std::function<void*()> data)
	{
		selectedCards = 0;
	});

	uiQueue->appendListener(UIInteractionEvent::OnPressSubmitCards, [this](std::function<void*()>)
	{
		if (selectedCards != 0)
		{
			if (!cardSubmitted)
			{

					uiQueue->enqueue(UIInteractionEvent::OnPopCard, [cardId = selectedCards]() mutable
					{
						return static_cast<void*>(&cardId);
					});

					CardSelectedVisual(0, selectedCards);
					Packet packet;
					packet << selectedCards;
					gameEvents->enqueue(GameEvent::OnCardsSubmitted, std::move(packet));
					cardSubmitted = true;
					timeOutTimer.Reset();
					timeOutTimer.Stop();
					timerTick = 0;
				}
			}
	});

	uiQueue->appendListener(UIInteractionEvent::OnPressJoinLobby, [this](std::function<void*()> data)
	{
		auto lobbyID = *static_cast<unsigned int*>(data());
		std::cout << lobbyID << std::endl;
	});

	lobbyQueue->appendListener(LobbyEvent::ClientConnected, [this](std::function<void*()>)
	{
		Packet packet{};
		OnPlayerConnected(std::move(packet));
	});
}

void Game::Update(double deltaTime)
{
	

	if (timeOutTimer.timePassed > 30)
	{
		timerTick = 0;

		if (selectedCards == 0)
		{
			selectedCards = clientMatchState->playerHands.at(0).at(0);
		}


		uiQueue->enqueue(UIInteractionEvent::OnPopCard, [card = selectedCards]() mutable
		{
			return static_cast<void*>(&card);
		});

		Packet packet{};
		packet << selectedCards;
		gameEvents->enqueue(GameEvent::OnCardsSubmitted, std::move(packet));
		cardSubmitted = true;

		timeOutTimer.Reset();
		timeOutTimer.Stop();
	}


	
	if (!isEventDone)
	{
		auto* currentEvent = GetCurrentEvent();
		if (currentEvent == nullptr)
		{
			isCapableOfUpdate = false;
		}
		else
		{
			isEventDone = currentEvent->eventDelegate();
			if (isEventDone)
			{
				if (!effectEvents.empty())
				{
					effectEvents.pop();
					isCapableOfUpdate = true;

					if (effectEvents.empty())
					{
						if (!timeOutTimer.HasStarted())
						{
							timeOutTimer.Start();
						}
					}
				}
				isEventDone = false;
			}
		}
	}
}


void Game::OnPlayerConnected(Packet&& packet)
{
	audioSystem->StopMusic();
	audioSystem->StartMusic(0);

	
}

void Game::OnLobbyDataReceived(Packet&& packet)
{
	packet >> lobbyData.id;
	packet >> lobbyData.ownerProfileId;
	packet >> lobbyData.ownerDeckId;
	packet >> lobbyData.opponentProfileId;
	packet >> lobbyData.opponentDeckId;
	packet >> lobbyData.serverId;
}



void Game::OnGameStarted(Packet&& packet)
{
	sceneHandler->EnableScene(0);

	for (auto& currentWaypoint : sceneHandler->playerWaypoints.waypoints)
	{
		std::reverse(currentWaypoint.begin(), currentWaypoint.end());
	}
	for (auto& currentWaypoint : sceneHandler->opponentWaypoints.waypoints)
	{
		std::reverse(currentWaypoint.begin(), currentWaypoint.end());
	}
	for (auto& currentWaypoint : sceneHandler->cameraWaypoints.waypoints)
	{
		std::reverse(currentWaypoint.begin(), currentWaypoint.end());
	}

	if (!sceneHandler->monsterSpawnPoints.empty())
	{
		for (int i = 0; i < clientMatchState->monsterCards.size(); ++i)
		{
			if (sceneHandler->monsterSpawnPoints[i] != nullptr)
			{
				
				auto transform = core::MakeDefaultTransform();
				auto currentMonster = sceneHandler->AddModel("Char_Enem_" + clientMatchState->monsterCards[i].meta.name,
				                                             transform, sceneHandler->monsterSpawnPoints[i]);
				currentMonster->name = "monster" + ptl::to_string(i);
			}
		}
	}

	core::Transform transformPlayer{
		sceneHandler->playerWaypoints.waypoints[0][0].GetPos(), sceneHandler->playerWaypoints.waypoints[0][0].GetRot(),
		glm::vec3{1, 1, 1}
	};
	core::Transform transformOpponent{
		sceneHandler->opponentWaypoints.waypoints[0][0].GetPos(),
		sceneHandler->opponentWaypoints.waypoints[0][0].GetRot(), glm::vec3{1, 1, 1}
	};

	ptl::string playerModelName = playerID == 0 ? "elenichar" : "Char_Player02";
	ptl::string opponentModelName = playerID == 1 ? "elenichar" : "Char_Player02";

	auto player1Model = sceneHandler->AddModel(playerModelName, transformPlayer);
	player1Model->name = "player";

	auto player2Model = sceneHandler->AddModel(opponentModelName, transformOpponent);
	player2Model->name = "opponent";


	if (!sceneHandler->cameraWaypoints.waypoints.empty())
	{
		camera->m_Translation = sceneHandler->cameraWaypoints.waypoints[0][0].GetPos();
		camera->m_Rotation = sceneHandler->cameraWaypoints.waypoints[0][0].GetRot();
		sceneHandler->cameraWaypoints.isDone = false;
	}

	audioSystem->StopMusic();
	audioSystem->StartMusic(2);
	
	core::Transform startTransform = core::MakeDefaultTransform();
	core::Transform endTransform = core::MakeDefaultTransform();

	endTransform.SetRot(glm::quat{{0, 180, 0}});

	ptl::vector<core::Transform> cardRevealPoints;
	cardRevealPoints.push_back(startTransform);
	cardRevealPoints.push_back(endTransform);

	sceneHandler->cardWaypoints.waypoints.push_back(cardRevealPoints);

	timeOutTimer.Start();
	bool enabled = true;
	uiQueue->enqueue(UIInteractionEvent::OnSetConfirmEnabled, [enabled]() mutable
	{
		return static_cast<void*>(&enabled);
	});

	uiQueue->enqueue(UIInteractionEvent::OnToggleLoadingScreen, nullptr);
}

void Game::OpponentOnCardHover(Packet&& packet)
{
	unsigned int cardID;
	packet >> cardID;

	
}

void Game::OpponentOnCardSelected(Packet&& packet)
{
	unsigned int cardID;
	packet >> cardID;

	
}

void Game::OpponentOnCardsSubmitted(Packet&& packet)
{
}

void Game::ProcessChanges(Packet&& packet)
{
	std::cout << "Process CHanges\n";
	ptl::vector<unsigned int> playedCards;
	unsigned int sizeOfPlayedCard = 0;
	packet >> sizeOfPlayedCard;

	for (unsigned int i = 0; i < sizeOfPlayedCard; ++i)
	{
		unsigned int playedCard = 0;
		packet >> playedCard;
		playedCards.push_back(playedCard);
	}

	ptl::vector<tbsg::Change> changes{};
	unsigned int changeCount;
	packet >> changeCount;

	for (unsigned int i = 0; i < changeCount; ++i)
	{
		tbsg::Change change{};

		unsigned int changeType;
		packet >> changeType;
		change.changeType = static_cast<tbsg::EffectChange>(changeType);

		packet >> change.change;
		packet >> change.index;

		changes.push_back(change);
	}

	isEventDone = false;
	cardSubmitted = false;

	
	
	
	

	EffectChangeHandler::ProcessChanges(changes, effectEvents, uiQueue, *sceneHandler, *clientMatchState, *audioSystem,
	                                    *database, *camera, deltaTime.deltaTime, playerID);


	bool enabled = true;
	uiQueue->enqueue(UIInteractionEvent::OnSetConfirmEnabled, [enabled]() mutable
	{
		return static_cast<void*>(&enabled);
	});

	timeOutTimer.Reset();
}

void Game::ProcessMatchData(Packet&& packet)
{
	ptl::string msg = "Loading map...";
	uiQueue->enqueue(UIInteractionEvent::OnUpdateLoadingScreen, [msg]() mutable
	{
		return static_cast<void*>(&msg);
	});

	
	unsigned int monsterCardsSize;
	packet >> monsterCardsSize;

	for (unsigned int i = 0; i < monsterCardsSize; ++i)
	{
		tbsg::MonsterCard monsterCard{};
		
		packet >> monsterCard.id;
		
		packet >> monsterCard.meta.name;
		packet >> monsterCard.meta.description;
		packet >> monsterCard.meta.rarity;
		packet >> monsterCard.meta.type;
		
		packet >> monsterCard.data.health;
		packet >> monsterCard.data.maxHealth;
		packet >> monsterCard.data.armor;
		packet >> monsterCard.data.monsterTrait;
		
		unsigned int rewardSize;
		packet >> rewardSize;
		for (unsigned int j = 0; j < rewardSize; ++j)
		{
			tbsg::Reward* reward{};
			int rewardType;
			packet >> rewardType;
			packet >> reward->powerup;
			reward->type = static_cast<tbsg::CardRewardType>(rewardType);
			monsterCard.data.reward.push_back(reward);
		}
		clientMatchState->monsterCards.push_back(monsterCard);
	}

	
	unsigned int playerDeck1Size;
	packet >> playerDeck1Size;
	for (unsigned int i = 0; i < playerDeck1Size; ++i)
	{
		unsigned int cardID;
		packet >> cardID;
		clientMatchState->playerDecks[0].push_back(cardID);
	}

	unsigned int playerDeck2Size;
	packet >> playerDeck2Size;
	for (unsigned int i = 0; i < playerDeck2Size; ++i)
	{
		unsigned int cardID;
		packet >> cardID;
		clientMatchState->playerDecks[1].push_back(cardID);
	}

	
	unsigned int playerHand1Size;
	packet >> playerHand1Size;
	for (unsigned int i = 0; i < playerHand1Size; ++i)
	{
		unsigned int cardID;
		packet >> cardID;
		clientMatchState->playerHands[0].push_back(cardID);
	}

	unsigned int playerHand2Size;
	packet >> playerHand2Size;
	for (unsigned int i = 0; i < playerHand2Size; ++i)
	{
		unsigned int cardID;
		packet >> cardID;
		clientMatchState->playerHands[1].push_back(cardID);
	}

	
	unsigned int playerDiscard1Size;
	packet >> playerDiscard1Size;
	for (unsigned int i = 0; i < playerDiscard1Size; ++i)
	{
		unsigned int cardID;
		packet >> cardID;
		clientMatchState->playerDiscards[0].push_back(cardID);
	}

	unsigned int playerDiscard2Size;
	packet >> playerDiscard2Size;
	for (unsigned int i = 0; i < playerDiscard2Size; ++i)
	{
		unsigned int cardID;
		packet >> cardID;
		clientMatchState->playerDiscards[1].push_back(cardID);
	}

	

	tbsg::Hero hero1;
	tbsg::Hero hero2;

	packet >> hero1.health;
	packet >> hero1.maxHealth;
	packet >> hero1.resource;
	packet >> hero1.armor;
	packet >> hero1.attack;
	packet >> hero1.baseAttack;

	packet >> hero2.health;
	packet >> hero2.maxHealth;
	packet >> hero2.resource;
	packet >> hero2.armor;
	packet >> hero2.attack;
	packet >> hero2.baseAttack;

	int firstPlayerTagIndex = 0;
	packet >> firstPlayerTagIndex;
	packet >> playerID;

	std::cout << "PlayerIndex: " << playerID << '\n';

	std::array<int, 2> firstPlayerTag{static_cast<int>(firstPlayerTagIndex), static_cast<int>(0)};
	EffectChangeHandler::UpdateUIText(UIInteractionEvent::OnUpdateFirstPlayerTag, firstPlayerTag, uiQueue);

	clientMatchState->heroes.push_back(hero1);
	clientMatchState->heroes.push_back(hero2);

	
	std::array<int, 2> hero1Health{
		static_cast<int>(clientMatchState->heroes[0].health), static_cast<int>(clientMatchState->heroes[0].maxHealth)
	};
	EffectChangeHandler::UpdateUIText(UIInteractionEvent::OnUpdateHealthPlayer, hero1Health, uiQueue);
	ptl::vector<unsigned int> test;
	std::array<int, 2> hero1Armor{static_cast<int>(clientMatchState->heroes[0].armor), 0};
	EffectChangeHandler::UpdateUIText(UIInteractionEvent::OnUpdateArmorPlayer, hero1Armor, uiQueue);

	std::array<int, 2> hero1Deck{
		static_cast<int>(clientMatchState->playerDecks[0].size()),
		static_cast<int>(clientMatchState->playerDecks[0].size())
	};
	EffectChangeHandler::UpdateUIText(UIInteractionEvent::OnUpdateDeckPlayer, hero1Deck, uiQueue);

	std::array<int, 2> hero1Discard{
		static_cast<int>(clientMatchState->playerDiscards[0].size()),
		static_cast<int>(clientMatchState->playerDiscards[0].size())
	};
	EffectChangeHandler::UpdateUIText(UIInteractionEvent::OnUpdateDiscardPlayer, hero1Discard, uiQueue);

	EffectChangeHandler::UpdateUIString(UIInteractionEvent::OnUpdateNamePlayer, "You", uiQueue);


	
	std::array<int, 2> hero2Health{
		static_cast<int>(clientMatchState->heroes[1].health), static_cast<int>(clientMatchState->heroes[1].maxHealth)
	};
	EffectChangeHandler::UpdateUIText(UIInteractionEvent::OnUpdateHealthOpponent, hero2Health, uiQueue);

	std::array<int, 2> hero2Armor{static_cast<int>(clientMatchState->heroes[1].armor), 0};
	EffectChangeHandler::UpdateUIText(UIInteractionEvent::OnUpdateArmorOpponent, hero2Armor, uiQueue);

	EffectChangeHandler::UpdateUIString(UIInteractionEvent::OnUpdateNameOpponent, "Opponent", uiQueue);

	
	std::array<int, 2> monsterHealth{
		static_cast<int>(clientMatchState->monsterCards[0].data.health),
		static_cast<int>(clientMatchState->monsterCards[0].data.maxHealth)
	};
	EffectChangeHandler::UpdateUIText(UIInteractionEvent::OnUpdateHealthMonster, monsterHealth, uiQueue);

	std::array<int, 2> monsterArmor{static_cast<int>(clientMatchState->monsterCards[0].data.armor), 0};
	EffectChangeHandler::UpdateUIText(UIInteractionEvent::OnUpdateArmorMonster, monsterArmor, uiQueue);

	EffectChangeHandler::UpdateUIString(UIInteractionEvent::OnUpdateNameMonster,
	                                    clientMatchState->monsterCards[0].meta.name, uiQueue);


	
	if (playerID == 1)
	{
		EffectChangeHandler::UpdateUIString(UIInteractionEvent::OnUpdateIconPlayer, "UI_player1-Icon.png", uiQueue);
		EffectChangeHandler::UpdateUIString(UIInteractionEvent::OnUpdateIconOpponent, "UI_player2-Icon.png", uiQueue);
	}
	else
	{
		EffectChangeHandler::UpdateUIString(UIInteractionEvent::OnUpdateIconPlayer, "UI_player2-Icon.png", uiQueue);
		EffectChangeHandler::UpdateUIString(UIInteractionEvent::OnUpdateIconOpponent, "UI_player1-Icon.png", uiQueue);
	}


	sceneHandler->LoadMap(tbsg::HashString("FrogLander"));
	

	for(const auto& card : database->GetCardList())
	{
		sceneHandler->CreateCardTexture(card.id);
	}

	sceneHandler->LoadParticles();

	gameEvents->enqueue(GameEvent::ClientCanStart, Packet{});
}

void Game::OnPlayerDisconnected(Packet&& packet)
{

}

void Game::LoadProvidedMapID(Packet&& packet)
{
	
	
	
	sceneHandler->LoadMap(tbsg::HashString("FrogLander"));
}

void Game::CardSelectedVisual(unsigned playerIndex, unsigned cardId)
{
	const unsigned int index = (playerIndex) ? !playerID : playerID;
	std::cout << "index:" << index << " is fliped :" << ((playerIndex) ? "yes" : "no") << '\n';
	std::cout << "cardID: " << cardId << '\n';
	EffectChangeHandler::SpawnCard(index, cardId, *sceneHandler, *clientMatchState, true);
	auto spawnPointIndex = sceneHandler->cardSpawnPoints[playerID].size() - clientMatchState->monsterCards.size();
	spawnPointIndex = (sceneHandler->cardSpawnPoints[playerID].size() == spawnPointIndex) ? spawnPointIndex - 1 : spawnPointIndex;
	std::cout << "spawnPointIndex:" << spawnPointIndex << '\n';
	effectEvents.push(EffectChangeHandler::RegisterAPIEvent(&EffectChangeHandler::PlayParticleEffect,
	                                                        "PlayParticleEffect", sceneHandler,
	                                                        sceneHandler->cardSpawnPoints[index][spawnPointIndex]
	                                                        ->transform.GetPos(), "DeathParticle.json"));
}


tbsg::EffectEvent* Game::GetCurrentEvent()
{
	while (!effectEvents.empty())
	{
		if (effectEvents.front() != nullptr)
		{
			return effectEvents.front();
		}

		effectEvents.pop();
	}
	return nullptr;
}
