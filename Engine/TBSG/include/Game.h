#pragma once
#include <rapidjson/stringbuffer.h>
#include "memory/Containers.h"
#include "memory/String.h"
#include "utils/EventQueues.h"
#include "memory/smart_ptr.h"
#include "Net/Packet.h"
#include "Payloads.h"
#include "rendering/Camera.h"
#include "core/DeltaTime.h"

#include "SceneHandler.h"
#include <utils/GameTimer.h>

namespace tbsg
{
	
}

namespace audio
{
    class AudioSystem;
}

class GameDatabase;
class Game
{
public:
    Game(ptl::shared_ptr<UIEventQueue> uiEventQueue, tbsg::SceneHandler* sceneSystem, tbsg::ClientMatchState* clientMatch, audio::AudioSystem* audio, GameDatabase* gameDatabase, gfx::Camera* camera, core::DeltaTime& deltaTime);
	~Game() = default;

    
    void Initialize(ptl::shared_ptr<GameEventQueue> evQueue, ptl::shared_ptr<LobbyEventQueue> lobbyEventQueue);
	void Update(double deltaTime);


private:

	void OnPlayerConnected(Packet&& packet);
	void OnLobbyDataReceived(Packet&& packet);
	void OnGameStarted(Packet&& packet);
	void OpponentOnCardHover(Packet&& packet);
	void OpponentOnCardSelected(Packet&& packet);
	void OpponentOnCardsSubmitted(Packet&& packet);
	void ProcessChanges(Packet&& packet);
	void ProcessMatchData(Packet&& packet);
	void OnPlayerDisconnected(Packet&& packet);
	void LoadProvidedMapID(Packet&& packet);

	void CardSelectedVisual(unsigned playerIndex, unsigned int cardId);

 

	ptl::shared_ptr<GameEventQueue> gameEvents;
	ptl::shared_ptr<UIEventQueue> uiQueue;
	ptl::shared_ptr<LobbyEventQueue> lobbyQueue;

	tbsg::SceneHandler* sceneHandler;

	
	ptl::queue<tbsg::EffectEvent*> effectEvents;
	bool isEventDone = false;
	bool isCapableOfUpdate = false;
	bool cardSubmitted = false;

	tbsg::EffectEvent* GetCurrentEvent();

	unsigned int selectedCards{0};

    tbsg::ClientMatchState* clientMatchState;
    audio::AudioSystem* audioSystem;
    GameDatabase* database;
    gfx::Camera* camera;

	tbsg::Match lobbyData{};

    core::DeltaTime& deltaTime;

	GameTimer timeOutTimer{  };

	unsigned int timerTick{ 0 };
    unsigned int playerID{};
	
};


