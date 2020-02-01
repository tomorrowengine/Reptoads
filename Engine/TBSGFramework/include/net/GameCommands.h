#pragma once


enum class ClientGameCommands
{
	ClientCanStart,
	OnCardHover,
	OnCardSelect,
	OnCardsSubmit
};

inline std::string GetName(ClientGameCommands command)
{
	switch (command)
	{
	case ClientGameCommands::ClientCanStart: return "ClientCanStart";
	case ClientGameCommands::OnCardHover: return "OnCardHover";
	case ClientGameCommands::OnCardSelect: return "OnCardSelect";
	case ClientGameCommands::OnCardsSubmit: return "OnCardsSubmit";
	}
	assert(false && "Enum type not implemented! GetName(ClientGameCommands)");
	return "Enum type not implemented! GetName(ClientGameCommands)";
}

enum class ServerGameCommands
{
	SendLobbyData,
	SendMatchData,
	GameHasStarted,
	GameHasEnded,
	OpponentOnCardHover,
	OpponentOnCardSelect,
	OpponentOnCardsSubmit,
	TurnResult,
	GameServerHasAnError
};

inline std::string GetName(ServerGameCommands command)
{
	switch (command)
	{
	case ServerGameCommands::SendLobbyData: return "SendLobbyData";
	case ServerGameCommands::SendMatchData: return "SendMatchData";
	case ServerGameCommands::GameHasStarted: return "GameHasStarted";
	case ServerGameCommands::GameHasEnded: return "GameHasEnded";
	case ServerGameCommands::OpponentOnCardHover: return "OpponentOnCardHover";
	case ServerGameCommands::OpponentOnCardSelect: return "OpponentOnCardSelect";
	case ServerGameCommands::OpponentOnCardsSubmit: return "OpponentOnCardsSubmit";
	case ServerGameCommands::TurnResult: return "TurnResult";
	case ServerGameCommands::GameServerHasAnError: return "GameServerHasAnError";
	}
	assert(false && "Enum type not implemented! GetName(ServerGameCommands)");
	return "Enum type not implemented! GetName(ServerGameCommands)";
}
