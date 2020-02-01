#include "glm/gtx/euler_angles.hpp"
#include "glm/glm.hpp"
#include "ClientApplication.h"

#include "resource_loading/UILoader.h"
#include "resource_loading/GLTFLoader.h"
#include "rendering/Shader.h"
#include "rendering/dx/DX12Renderer.h"
#include "rendering/dx/DX12Application.h"
#include "brofiler/Brofiler.h"
#include "rendering/dx/render_passes/DxForwardPass.h"
#include "rendering/unified_shader_passes/ParticlePass.h"
#include "rendering/dx/render_passes/DxSkyboxPass.h"
#include "rendering/dx/render_passes/DxVignettePass.h"
#include "rendering/dx/render_passes/DxMipmapPass.h"
#include "rendering/dx/DxParticleRenderer.h"
#include "rendering/dx/render_passes/DxBlurPass.h"
#include "rendering/dx/render_passes/DxMergeBloomMipsPass.h"
#include "rendering/Camera.h"

#include "utils/EncryptUtils.h"
#include "ui/UISystem.h"
#include "resource_loading/RestApiParsing.h"

#include "gameplay/WaypointSystem.h"
#include "EffectChangeHandler.h"
#include "resource_loading/ArgumentParsing.h"
#include "rendering/dx/DxParticleRenderer.h"
#include "rendering/unified_shader_passes/ParticlePass.h"
#include "rendering/GeometricShapes.h"


void ClientApplication::Initialize(int argc, char *argv[])
{
	skyboxFile = "OutputCube_1024.dds";

	IApplication::Initialize(argc, argv);
	loginHandler->Initialize(&database, &m_renderer->GetGfxResourceManager());
	game.Initialize(gameEQ, lobbyEQ);
	gameClient.Initialize(gameEQ);
	lobbyClient.Initialize(lobbyEQ, this);

	audioSystem.Initialize();
	uiSystem = new ui::UISystem(m_renderer->GetUIRenderer(), m_renderer->GetGfxResourceManager(), audioSystem, [this](ptl::shared_ptr<ui::UIText> text)
		{
			m_input->OpenInputDialog(text->ObsecureText(), text->GetRawText());
		});

	ui.Initialize(lobbyEQ, uiEQ, uiSystem);

	deltaTime.Initialize();
	SetUpRenderingPipeline();
	camera.set_Projection(32.5, 1280.f / 720.f, 1.0f, 2000.f);

	sceneHandler.Initialize();
	sceneHandler.SetRenderer(m_renderer);

	BindUIControlls();
	BindLoginEvents();

	uiEQ->appendListener(UIInteractionEvent::Quit, [](std::function<void* ()>)
		{
			IApplication::m_ShouldStop = true;
		});

	uiEQ->appendListener(UIInteractionEvent::OnAudioToggle, [this](std::function<void* ()>)
		{
			if (audioSystem.IsPaused())
			{
				uiSystem->SetImageForElementsOfTag("MusicButton.png", "MusicButton");
				audioSystem.ResumeMusic();
			}
			else
			{
				uiSystem->SetImageForElementsOfTag("MusicButton-Clicked.png", "MusicButton");
				audioSystem.PauseMusic();
			}
		});

}

void ClientApplication::BindUIControlls()
{
	m_input->AddListener(BINDACTION(&ui::UISystem::OnClick, uiSystem), tbsg::InputType::EMouse_Left);
	m_input->AddListener(BINDACTION(&ui::UISystem::OnDown, uiSystem), tbsg::InputType::Down);
	m_input->AddListener(BINDACTION(&ui::UISystem::OnUp, uiSystem), tbsg::InputType::Up);
	m_input->AddListener(BINDACTION(&ui::UISystem::OnLeft, uiSystem), tbsg::InputType::Left);
	m_input->AddListener(BINDACTION(&ui::UISystem::OnRight, uiSystem), tbsg::InputType::Right);
	m_input->AddListener(BINDACTION(&ui::UISystem::OnClick, uiSystem), tbsg::InputType::Enter);
	m_input->AddListener(BINDACTION(&ui::UISystem::OnBackspace, uiSystem), tbsg::InputType::Back);

	m_input->AddListener(BINDACTION(&UserInterface::SkipSplashScreen, &ui), tbsg::InputType::Space);
	m_input->AddListener(BINDACTION(&UserInterface::SkipSplashScreen, &ui), tbsg::InputType::EGamepad_Face_Button_Down);
	m_input->AddListener(BINDACTION(&UserInterface::OnPauseScreen, &ui), tbsg::InputType::Escape);
	m_input->AddListener(BINDACTION(&UserInterface::OnPauseScreen, &ui), tbsg::InputType::EGamepad_Special_Right);

	m_input->AddListener(BINDACTION(&ui::UISystem::OnDown, uiSystem), tbsg::InputType::EGamepad_DPad_Down);
	m_input->AddListener(BINDACTION(&ui::UISystem::OnUp, uiSystem), tbsg::InputType::EGamepad_DPad_Up);
	m_input->AddListener(BINDACTION(&ui::UISystem::OnLeft, uiSystem), tbsg::InputType::EGamepad_DPad_Left);
	m_input->AddListener(BINDACTION(&ui::UISystem::OnRight, uiSystem), tbsg::InputType::EGamepad_DPad_Right);
	m_input->AddListener(BINDACTION(&ui::UISystem::OnClick, uiSystem), tbsg::InputType::EGamepad_Face_Button_Down);

	m_input->AddListener(BINDAXIS(&ui::UISystem::OnAxisHorizontal, uiSystem), tbsg::InputType::EGamepad_Left_Thumb_X);
	m_input->AddListener(BINDAXIS(&ui::UISystem::OnAxisVertical, uiSystem), tbsg::InputType::EGamepad_Left_Thumb_Y);

	m_input->AddListener(BINDCHARACTER(&ui::UISystem::OnCharacterTyped, uiSystem));
	m_input->AddListener(BINDTEXTDIALOG(&ui::UISystem::OnTextDialogResult, uiSystem));

}

void ClientApplication::BindLoginEvents()
{
	
	if (loginHandler->CheckForLocalAccessToken())
	{
		if (loginHandler->CheckIfAccessTokenValid())
		{
			lobbyClient.SetCurrentUserName(loginHandler->GetCurrentUserName());
			uiEQ->enqueue(UIInteractionEvent::OnValidTokenFound, nullptr);
			ui.SetCurrentUserName(loginHandler->GetCurrentUserName());
			uiEQ->enqueue(UIInteractionEvent::OnValidTokenFound, nullptr);
		}
	}


	
	uiEQ->appendListener(UIInteractionEvent::OnLoginStatus, [this](std::function<void* ()> data)
		{
			auto pair = *static_cast<std::pair<ptl::string, ptl::string>*>(data());

			if (loginHandler->LoginUser(pair.first, pair.second))
			{
				lobbyClient.SetCurrentUserName(pair.first);
				ui.SetCurrentUserName(pair.first);
				loginHandler->WriteNewAccessToken();
				uiEQ->enqueue(UIInteractionEvent::OnLoadGameData, nullptr);
			}
			else
			{

				uiEQ->enqueue(UIInteractionEvent::OnToggleLoadingScreen, nullptr);
				uiSystem->Dialog("Invalid email or password.\nPlease try again!");
				uiSystem->PushUI("Login.ui");
			}
		});

	
	uiEQ->appendListener(UIInteractionEvent::OnLoadGameData, [this](std::function<void* ()>)
		{
			lobbyClient.SetNetworkAccessToken(loginHandler->GetCurrentAccessToken());
			gameClient.SetNetworkAccessToken(loginHandler->GetCurrentAccessToken());
			if (loginHandler->LoadGameData())
			{
				if (loginHandler->GetUsersDecks())
				{
					this->ConnectToLobbyServer();
					uiEQ->enqueue(UIInteractionEvent::OnSwitchToMainMenu, nullptr);
#ifdef _WIN32

					SetConsoleTitleA(("(User: " + loginHandler->GetCurrentUserName() + ")").c_str());
#endif

				}
				else
				{
					uiSystem->PushUI("Login.ui");
					uiEQ->enqueue(UIInteractionEvent::OnToggleLoadingScreen, nullptr);

					uiSystem->Dialog("Failed to process required Game Data, check your internet connection");
				}
			}
			else
			{
				uiSystem->PushUI("Login.ui");
				uiEQ->enqueue(UIInteractionEvent::OnToggleLoadingScreen, nullptr);

				uiSystem->Dialog("Failed to process required Game Data, check your internet connection");
			}
		});

	uiEQ->appendListener(UIInteractionEvent::OnRequestDeckData, [this](std::function<void* ()>)
		{
			auto decks = database.GetDeckList();
			ptl::vector<ui::DeckDisplayInformation> deckDisplayItems{};
			for (auto& deck : decks)
			{
				if (!deck.cards.empty())
				{
					ui::DeckDisplayInformation deckInfo;
					deckInfo.deckName = deck.name;
					deckInfo.deckID = deck.id;
					deckInfo.coverCardId = deck.cards[0]->meta.name;
					deckDisplayItems.push_back(deckInfo);
				}
			}
			
			uiEQ->enqueue(UIInteractionEvent::OnParseDecksData, [deckDisplayItems]() mutable
				{
					return static_cast<void*>(&deckDisplayItems);
				});
		});


	gameEQ->appendListener(GameEvent::OnServerConnect, [this](Packet)
		{
			if (lobbyClient.IsConnected())
			{
				printf("Disconnecting from LobbyServer...\n");
				lobbyClient.Disconnect();
			}

		});

	lobbyEQ->appendListener(LobbyEvent::OnServerConnect, [this](std::function<void* ()>)
		{
			if (gameClient.IsConnected())
			{
				printf("Disconnecting from GameServer...\n");
				gameClient.Disconnect();
			}
		});

}

void ClientApplication::Run()
{
	utils::InitEncrypt();
	IApplication::Run();

	std::function<void()> frameIndepenedentUpdate = [this]()
	{
		
	};

	static int counter = 0;
	glm::ivec2 cursorPos;
	


	while (!IApplication::m_ShouldStop)
	{
		deltaTime.Update();
		audioSystem.Update();
		deltaTime.FrameIndependentUpdate(frameIndepenedentUpdate);

		
		gameEQ->process();
		lobbyEQ->process();

		uiEQ->process();

		m_input->Update();
		m_input->GetCursorPosition(cursorPos.x, cursorPos.y);

		ui.Update(deltaTime.GetDeltaTime());
		uiSystem->Update(deltaTime, cursorPos);

		gameClient.Update();
		lobbyClient.Update();
		game.Update(deltaTime.GetDeltaTime());
		particleRenderer->UpdateParticleEmitters(static_cast<float>(deltaTime.GetTotalTime()), static_cast<float>(deltaTime.GetDeltaTime()));

		
		m_renderer->RenderWorld(camera);
		m_renderer->RenderUI(camera);
		m_renderer->Present();


	}

	Shutdown();
}

void ClientApplication::ConnectToLobbyServer()
{
	printf("Connecting to LobbyServer...\n");
	tbsg::Config config = tbsg::Config::Get();
	ptl::string ip = config.GetAddress();
	unsigned int port = config.GetPort();
	lobbyClient.Connect(ip.c_str(), port);

}

void ClientApplication::ConnectToGameServer(const ptl::string& ip, unsigned port)
{
	printf("Connecting to GameServer...\n");
	gameClient.Connect(ip.c_str(), port);

}

void ClientApplication::Shutdown()
{
	lobbyClient.Disconnect();
	gameClient.Disconnect();

	IApplication::Shutdown();

	DX12Application::Destroy();
	utils::DeinitEncrypt();
	delete uiSystem;

}

void ClientApplication::SetUpRenderingPipeline()
{
	const float vignetteRadius = 1;
	const float vignetteSmoothness = 0;

	
	auto& rm = m_renderer->GetGfxResourceManager();
	

	auto vertexShaderPath = tbsg::Config::Get().MakeShaderPath("ForwardPass_v.cso", 1);
	auto pixelShaderPath = tbsg::Config::Get().MakeShaderPath("ForwardPass_p.cso", 1);


	auto vsId = rm.CreateShader(vertexShaderPath.c_str());
	auto psId = rm.CreateShader(pixelShaderPath.c_str());
	ptl::vector<gfx::IShader*> shaders
	{
		&rm.GetShader(vsId),
		&rm.GetShader(psId)
	};

	
	{
		auto dxRenderer = static_cast<DX12Renderer*>(m_renderer);
		auto fwdPass = new DxForwardPass{ dxRenderer, shaders };
		m_renderer->AddRenderPass(new DxSkyboxPass{ dxRenderer, skyboxFile });
		m_renderer->AddRenderPass(fwdPass);
		m_renderer->AddRenderPass(new DxVignettePass{ dxRenderer , vignetteRadius, vignetteSmoothness });
		auto mipmapPass = new DxMipmapPass{ dxRenderer, &fwdPass->GetRwBloomTexture() };
		m_renderer->AddRenderPass(mipmapPass);
		m_renderer->AddRenderPass(new DxBlurPass{ dxRenderer, *mipmapPass });
		m_renderer->AddRenderPass(new DxMergeBloomMipsPass{ dxRenderer, mipmapPass->GetNonMsaaTextureToMipmap() });
		{
			const auto dxParticleRenderer = new DxParticleRenderer(dxRenderer, fwdPass->GetBloomTexture(), fwdPass->GetBloomTextureFormat());
			particleRenderer = ptl::unique_ptr<IParticleRenderer>(dxParticleRenderer);
			m_renderer->AddRenderPass(new ParticlePass(dxParticleRenderer));
			sceneHandler.SetParticleRenderer(particleRenderer.get());
		}
	}
}

