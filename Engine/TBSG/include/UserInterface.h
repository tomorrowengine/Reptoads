#pragma once
#include "memory/smart_ptr.h"
#include "utils/EventQueues.h"
#include "ui/UISystemInterface.h"
#include "utils/GameTimer.h"
#include "ui/LobbyUI.h"

namespace tbsg {
	enum class InputAction;
}

namespace ui
{
	struct UIEvent;
	class UISystem;
}

enum class UserInterfaceState
{
	None = 0,
	SplashScreen_1,
	SplashScreen_2,
	SplashScreen_3,
	Login,
	MainMenu,
	Collections,
	Store,
	Settings,
	LobbySelect,
	LobbyRoom,
	LoadingScreen,
	GameScreen,
	WinScreen,
	LoseScreen,
	RegisterScreen,
	EULAScreen,
	WaitingRoom,
	Credits
};

struct UICard
{
	std::shared_ptr<ui::UIImage> image;
	int id;
	bool selected;
};


class UserInterface : public ui::UISystemInterface
{
public:
	UserInterface() = default;
	~UserInterface() = default;

	void Initialize(ptl::shared_ptr<LobbyEventQueue> lobbyEventQueue, ptl::shared_ptr<UIEventQueue> uiEventQueue, ui::UISystem* uiSystem);

	

	void OnElementReleased(ptl::weak_ptr<ui::UIElement> element) override;
	void OnElementFocussed(ptl::weak_ptr<ui::UIElement> element) override;
	void OnElementFocusLost(ptl::weak_ptr<ui::UIElement> element) override;

	void OnEvent(ptl::weak_ptr<ui::UIElement> element, ui::UIEvent event) override;

	

	void OnStartUp();


	void OnCheckForLoginScreen();

	void OnLoginScreenStartup();

	void OnPressMultiplayer(ptl::weak_ptr<ui::UIElement> element);

	void OnPressReady(ptl::weak_ptr<ui::UIElement> element);

	void OnPressStartGame(ptl::weak_ptr<ui::UIElement> element);

	

	void OnPressSubmitCards(ptl::weak_ptr<ui::UIElement> element);

	void SetConfirmEnabled(bool enabled);
	void SetConfirmAllowed(bool allowed) { this->confirmAllowed = allowed; }
	

	void OnUpdateHealthPlayer(int health, int maxHealth);

	void OnUpdateArmorPlayer(int armor);

	void OnUpdateNamePlayer(ptl::string name);

	void OnUpdateIconPlayer(ptl::string image);

	void OnUpdateDeckPlayer(int amount);

	void OnUpdateDiscardPlayer(int amount);

	

	void OnUpdateHealthOpponent(int health, int maxHealth);

	void OnUpdateArmorOpponent(int armor);

	void OnUpdateNameOpponent(ptl::string name);

	void OnUpdateIconOpponent(ptl::string image);

	

	void OnUpdateHealthMonster(int health, int maxHealth);

	void OnUpdateArmorMonster(int armor);

	void OnUpdateNameMonster(ptl::string name);

	void OnUpdateIconMonster(ptl::string image);

	

	void OnPushCard(int cardId, ptl::string texture);

	void OnPopCard(int cardId);

	void OnClearDeck();

	void OnShowReward();

	void OnConfirmReward(ptl::weak_ptr<ui::UIElement> element);

	void UpdatePlayersHandSymbol(bool isPlayer, unsigned int number);

	

	void OnWinScreen();

	void OnLoseScreen();

	void OnUpdateGameTimer(unsigned int time);

	void OnLeaveMatch(ptl::weak_ptr<ui::UIElement> element);

	void OnToggleLoadingScreen();

	void OnSwitchToMainMenu();

	void OnLoginStatusUpdate(const ptl::string& message);

	bool OnPauseScreen(tbsg::InputAction action);

	void UpdateLoadingScreenText(const ptl::string& message);

	bool SkipSplashScreen(tbsg::InputAction a_action);

	void SwitchUI(const ptl::string& toUnload, const ptl::string& toLoad);
	bool SwitchUiIfPossible(UserInterfaceState currentState, UserInterfaceState newCurrentState, const ptl::string& toUnload, const ptl::string& toLoad);

	void Update(double deltaTime);

	void SetCurrentUserName(const ptl::string& newUserName) { lobbyData.currentUserName = newUserName; }
private:
	void SetHealthBarValue(const ptl::string & textTag, const ptl::string & barTag, int health, int maxHealth, bool horizontal, bool inverted);

	void ReorderCards();

	unsigned int GetIdBasedOnImage(ptl::weak_ptr<ui::UIImage> image);

	void RemoveSelectedCardsFromHand();

	bool HasCardsSelected();

	glm::vec2 GetCardSize();
	glm::vec2 GetCardPosition(int index);

private:
	const int normalHeight{ 10 };
	const int focussedHeight{ 30 };
	const int selectedHeight{ 300 };
	const int selectedFocussedHeight{ 310 };

	bool showLoadingScreen{ false };

	glm::vec2 initialOkPosition;
	bool confirmAllowed{ true };

	bool skipLogin{ false };

	UserInterfaceState currentUserInterfaceState{ UserInterfaceState::Login };

	ptl::shared_ptr<UIEventQueue> uiEQ{};
	ptl::shared_ptr<LobbyEventQueue> lobbyEQ{};
	ui::UISystem* uiSystem{ nullptr };


	ptl::map<ptl::string, float> healthSizeInitial{};
	ptl::map<ptl::string, float> healthPosInitial{};

	ptl::vector<UICard> cardsInHand{};

	ui::LobbyUI lobbyUI{};
	ui::LobbyUIData lobbyData{};

	GameTimer timer{};

	friend class ui::LobbyUI;
};
