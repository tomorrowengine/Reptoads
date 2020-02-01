#pragma once
namespace tbsg {

	
	enum class ServerLobbyCommands
	{
		
		LobbiesListed,

		
		LobbyCreated,

		
		LobbyCreationFailed,

		
		LobbyDestroyed,

		
		LobbyLeft,

		
		LobbyLeftFailed,

		
		LobbyPlayerLeft,

		
		LobbyOwnershipTransferred,

		
		JoinLobbyFailed,

		
		JoinLobbySuccess,

		
		LobbyPlayerJoined,

		SelectedDeck,
		OpponentSelectedDeck,
		SelectingDeckFailed,

		
		LobbyGameStarted,

		
		LobbyGameStartFailed,

		
		LobbyGameStopped,

		
		ProfileName
	};

	inline std::string GetName(ServerLobbyCommands command)
	{
		switch (command)
		{
			case ServerLobbyCommands::LobbiesListed: return "LobbiesListed";
			case ServerLobbyCommands::LobbyCreated: return "LobbyCreated";
			case ServerLobbyCommands::LobbyDestroyed: return "LobbyDestroyed";
			case ServerLobbyCommands::LobbyLeft: return "LobbyLeft";
			case ServerLobbyCommands::LobbyLeftFailed: return "LobbyLeftFailed";
			case ServerLobbyCommands::LobbyPlayerLeft: return "LobbyPlayerLeft";
			case ServerLobbyCommands::LobbyOwnershipTransferred: return "LobbyOwnershipTransferred";
			case ServerLobbyCommands::JoinLobbyFailed: return "JoinLobbyFailed";
			case ServerLobbyCommands::JoinLobbySuccess: return "JoinLobbySuccess";
			case ServerLobbyCommands::LobbyPlayerJoined: return "LobbyPlayerJoined";
			case ServerLobbyCommands::LobbyGameStarted: return "LobbyGameStarted";
			case ServerLobbyCommands::LobbyGameStopped: return "LobbyGameStopped";
			case ServerLobbyCommands::LobbyGameStartFailed: return "LobbyGameStartFailed";
			case ServerLobbyCommands::SelectedDeck: return "SelectedDeck";
			case ServerLobbyCommands::OpponentSelectedDeck: return "OpponentSelectedDeck";
			case ServerLobbyCommands::SelectingDeckFailed: return "SelectingDeckFailed";
			case ServerLobbyCommands::ProfileName: return "ProfileName";
			case ServerLobbyCommands::LobbyCreationFailed: return "LobbyCreationFailed";
			default: ;
		}
		assert(false && "Enum type not implemented! GetName(ServerLobbyCommands)");
		return "Enum type not implemented! GetName(ServerLobbyCommands)";
	}

	
	enum class ClientLobbyCommands
	{
		
		ListLobbies,

		
		CreateLobby,

		
		JoinLobby,

		
		LeaveLobby,

		
		SelectDeck,

		
		LobbyGameStart,

		
		LobbyGameStop,

		
		AIGameStart,

		
		GetProfileName
	};

	inline std::string GetName(ClientLobbyCommands command)
	{
		switch (command)
		{
			case ClientLobbyCommands::ListLobbies: return "ListLobbies";
			case ClientLobbyCommands::CreateLobby: return "CreateLobby";
			case ClientLobbyCommands::JoinLobby: return "JoinLobby";
			case ClientLobbyCommands::LeaveLobby: return "LeaveLobby";
			case ClientLobbyCommands::SelectDeck: return "SelectDeck";
			case ClientLobbyCommands::LobbyGameStart: return "LobbyGameStart";
			case ClientLobbyCommands::LobbyGameStop: return "LobbyGameStop";
			case ClientLobbyCommands::GetProfileName: return "GetProfileName";
			case ClientLobbyCommands::AIGameStart: return "AIGameStart";
		}
		assert(false && "Enum type not implemented! GetName(ClientLobbyCommands)");
		return "Enum type not implemented! GetName(ClientLobbyCommands)";
	}

	
	enum class LobbyError
	{
		Unknown,
		Full,
		Exist,
		Started,
		AlreadyJoined,
		OnlyTheOwnerCanStartTheGame,
		NoGameServerAvailable,
		NoDeckSelected
	};

}
