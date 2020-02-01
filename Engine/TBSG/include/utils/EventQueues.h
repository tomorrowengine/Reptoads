#pragma once

#include <eventpp/eventqueue.h>
#include "Net/Packet.h"
#include "UIInteractionEvent.h"

enum class GameEvent
{
	
	OnLoginScreenLoaded,

	
	PlayerConnected,
	PlayerDisconnected,
	LobbyDataReceived,
	MatchDataReceived,
	ParseMatchData,
	GameHasStarted,
	OpponentOnCardHover,
	OpponentOnCardSelected,
	OpponentOnCardsSubmitted,
	TurnResult,
	OnServerDisconnect,

	
	ClientCanStart,
	OnCardHover,
	OnCardSelected,
	OnCardsSubmitted,
	OnServerConnect
};

enum class LobbyEvent 
{
	ClientConnected,
	RequestLobbyList,
	ParseLobbyInfo,
	CreateNewLobby,
	SuccessfulLobbyCreated,
	JoinExistingLobby,
	JoinLobbySuccess,
	SelectDeck,
	OpponentSelectDeck,
	DisconnectFromLobby,
	LobbyDestroyed,
	LobbyLeft,
	LobbyLeftFailed,
	LobbyPlayerLeft,
	LobbyOwnershipTransferred,
	JoinLobbyFailed,
	LobbyError,
	LobbyPlayerJoined,
	LobbyGameStopped,
	OnServerDisconnect,
	OnServerConnect,
	AccountInUse,
	AuthFailure,
	RequestToStartGame,
	LobbyGameStarted,
	IsInGame,
	IsInGameResultNo
}; 

using GameEventQueue = eventpp::EventQueue<GameEvent, void(Packet)>;
using LobbyEventQueue = eventpp::EventQueue<LobbyEvent, void(std::function<void*()>)>;
using UIEventQueue = eventpp::EventQueue<UIInteractionEvent, void(std::function<void*()>)>;
