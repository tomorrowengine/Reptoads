#pragma once
#include "memory/String.h"
#include "memory/Containers.h"
#include "memory/smart_ptr.h"
#include <glm/vec2.hpp>

class UserInterface;
class GameTimer;

namespace core
{
	class DeltaTime;
};

namespace ui
{

	class UIElement;

	struct LobbyDisplayInformation
	{
		ptl::string ownerName{};
		unsigned int lobbyID{};
	};


	struct DeckDisplayInformation
	{
		ptl::string deckName{};
		unsigned int deckID{};
		ptl::string coverCardId{};
	};

	struct CurrentLobbyInformation
	{
		bool isOwnerOfLobby = true;
		bool ownerReady = false;
		bool opponentReady = false;
		ptl::string ownerName{};
		ptl::string opponentName{};
	};

	enum class CurrentLobbyUpdateInfo
	{
		OwnerName,
		OpponentName,
		OwnerReady,
		OpponentReady,
		OwnerIsWaitingForOpponent,
		OwnerIsWaitingForOpponentDeck,
		OpponentIsWaitingForOwnerDeck,
		OpponentLeft,
		StartingLobby
	};

	struct LobbyUIData
	{
		
		bool secondEntry{ false };
		ptl::vector<LobbyDisplayInformation> availableLobbies{};
		const unsigned int maximumLobbiesToDisplay{ 6 };
		unsigned int currentMaxLobbyIndex{ 0 };
		bool refreshLobbyView{ true };
		bool startedGame{ false };

		
		ptl::vector<DeckDisplayInformation> availableDecks{};
		const unsigned int maximumDecksToDisplay = 2;
		unsigned int currentMaxDeckIndex = 0;

		ptl::string currentUserName{};
		CurrentLobbyInformation currentLobby{};
		bool displayWaitingInfo = false;
		bool ownerDeckConfirmed = false;
		bool opponentDeckConfirmed = false;
	};


	class LobbyUI
	{
	public:
		void Initialize(UserInterface* ui, LobbyUIData* lobbydata);

		void ShowNextLobbies();
		void ShowPreviousLobbies();
		void DisplayAllAvailableLobbies(ptl::shared_ptr<ui::UIElement> toAddTowards, const unsigned int startLobbyIndex, const unsigned int endLobbyIndex);


		void LobbySelectScreen();
		void JoinLobby(unsigned int attachedLobbyID);
		void OnPressJoinLobby(unsigned int lobbyID, unsigned int deckNR);
		void OnPushLobbies(const unsigned int startIndex, unsigned int amountToAdd);
		void OnPressCreateLobby(ptl::weak_ptr<ui::UIElement> element);
		void CancelLobby();

		
		void OnPushDecks(const unsigned int startIndex, unsigned int amountToAdd);
		void DisplayCurrentAvailableDecks(const unsigned int startDeckIndex, const unsigned int amountToAdd);
		void ShowNextDecks();
		void ShowPreviousDecks();
		
		void TempHideStatusText();
		void DisableLobbyCardSelecting();
		void SetupWaitingScreen();
		void SetupCurrentLobbyInfo(CurrentLobbyUpdateInfo whatToUpdate, const ptl::string updateText = {}, const bool updateState = false);

		void ForceBackToLogin();
		void CheckForDeckConfirmation();
	private:
		UserInterface* userinterface{nullptr};
		LobbyUIData* lobbyData{nullptr};

		friend class UserInterface;
	};
}
