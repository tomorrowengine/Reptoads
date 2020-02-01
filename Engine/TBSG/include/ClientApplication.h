#pragma once
#include "IApplication.h"

#include "rendering/Camera.h"
#include "GameDatabase.h"
#include "SceneHandler.h"
#include "audio/AudioSystem.h"

#include "utils/EventQueues.h"
#include "LobbyClient.h"
#include "core/DeltaTime.h"
#include <chrono>
#include "GameClient.h"
#include "Game.h"
#include "UserInterface.h"
#include "core/Input.h"
#include "utils/LoginHandler.h"

#include "gameplay/WaypointSystem.h"
#include "rendering/IParticleRenderer.h"

namespace tbsg
{
    enum class InputAction;
}

namespace core
{
    class Transform;
}

namespace ui
{
	class UISystem;
}

class ClientApplication : public tbsg::IApplication
{
public:
    ClientApplication() {}

    ~ClientApplication() override = default;
    bool IsConnectedToGameServer();
    ClientApplication(const ClientApplication& other) = delete;
    ClientApplication(ClientApplication&& other) noexcept = delete;
    ClientApplication& operator=(const ClientApplication& other) = delete;
    ClientApplication& operator=(ClientApplication&& other) noexcept = delete;

    void Initialize(int argc, char *argv[]) override;
    void Run() override;
    void Shutdown() override;

	void ConnectToLobbyServer();
	void ConnectToGameServer(const ptl::string& ip, unsigned int port);

private:
    void SetUpRenderingPipeline();
	void BindUIControlls();
	void BindLoginEvents();

	gfx::Camera camera;

	ptl::unique_ptr<gfx::IParticleRenderer> particleRenderer;

    GameDatabase database;
    tbsg::SceneHandler sceneHandler{ database };
    audio::AudioSystem audioSystem{};
	
	ptl::unique_ptr<LoginHandler> loginHandler = ptl::make_unique<LoginHandler>();

    
    GameClient gameClient{};
    LobbyClient lobbyClient{};

    
    ptl::shared_ptr<GameEventQueue> gameEQ = ptl::make_shared<GameEventQueue>();
    ptl::shared_ptr<LobbyEventQueue> lobbyEQ = ptl::make_shared<LobbyEventQueue>();
    ptl::shared_ptr<UIEventQueue> uiEQ = ptl::make_shared<UIEventQueue>();

    
    core::DeltaTime deltaTime{};

    
    UserInterface ui{};
    ui::UISystem* uiSystem{ nullptr };

	
	Game game{uiEQ, &sceneHandler, &clientMatchState,&audioSystem,&database,&camera,deltaTime };

    tbsg::ClientMatchState clientMatchState;

	ptl::vector<int> testSelectedCards;
	ptl::string skyboxFile;
};
