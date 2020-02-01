#include "utils/LoginHandler.h"
#include "GameDatabase.h"
#include <iostream>
#include <fstream>
#include "core/Config.h"
#include "net/WebRequestHelper.h"
#include "memory/Containers.h"
#include "resource_loading/RestApiParsing.h"
#include "core/Assertion.h"
#include "rendering/IResourceManager.h"

bool LoginHandler::CheckForLocalAccessToken()
{
#ifdef _DEBUG
	debugMode = true;
#endif
	config = tbsg::Config::Get();
	ptl::string currentLineRead{};
	ptl::string accessToken{};
	ptl::string username{};
	unsigned int profileID = 0;
	std::ifstream tokenFile;
	ptl::string prefix{};

	if (debugMode)
	{
		tokenFile.open( (prefix+config.apiTokenFile).c_str() );
	}
	else
	{
		tokenFile.open((prefix + config.apiTokenFile).c_str(), std::ios::binary);
	}
	if (!tokenFile.is_open())
	{
		std::cout << "[LoginHandler] No access token found, proceeding.." << std::endl;
		return false;
	}
	else
	{
		unsigned int count = 0;
		while (getline(tokenFile, currentLineRead))
		{
			if (count == 0)
			{
				accessToken = currentLineRead;
				std::cout << "[LoginHandler] Read access token on disk: " << std::endl;
			}
			if (count == 1)
			{
				username = currentLineRead;
				std::cout << "[LoginHandler] Read username on disk" << std::endl;
			}
			if (count == 2)
			{
				profileID = ptl::stoul(currentLineRead);
				std::cout << "[LoginHandler] Read profile id on disk" << std::endl;
			}
			count++;
		}
		tokenFile.close();

		if (!accessToken.empty() && profileID != 0 && !username.empty())
		{
			currentAccessToken = accessToken;
			currentProfileId = profileID;
			currentUserName = username;
			return true;
		}
	}
	return false;
}

bool LoginHandler::CheckIfAccessTokenValid()
{
	if (currentAccessToken.empty())
	{
		std::cout << "[LoginHandler] CheckIfAccessTokenValid() couldn't check as currentAccessToken is emtpy" << std::endl;
		return false;
	}
	else
	{
		config = tbsg::Config::Get();
		ptl::string url = config.apiHost;

		return apiParser.CheckIfSessionIsValid(url, "/v1/checkSession.php", tbsg::WebMethod::POST, ptl::vector<tbsg::postDataPair> { {"token", currentAccessToken} });
	}
}

bool LoginHandler::WriteNewAccessToken(const ptl::string& newAccessToken, const unsigned int newProfileID, const ptl::string& newUsername)
{
	if (newAccessToken.empty() && currentAccessToken.empty() && newUsername.empty())
	{
		return false;
	}

	if (newProfileID == 0 && currentProfileId == 0)
	{
		return false;
	}

	if (!newAccessToken.empty())
	{
		currentAccessToken = newAccessToken;
	}

	if (newProfileID != 0)
	{
		currentProfileId = newProfileID;
	}

	if (!newUsername.empty())
	{
		currentUserName = newUsername;
	}
	ptl::string prefix{};
	std::ofstream accesTokenFile;
	if (debugMode)
	{
		accesTokenFile.open((prefix + config.apiTokenFile).c_str(), std::ios::trunc);
	}
	else
	{
		accesTokenFile.open((prefix + config.apiTokenFile).c_str(), std::ios::trunc | std::ios::binary);

	}
	if (!accesTokenFile.is_open())
	{
		std::cout << "[LoginHandler] Could not open local file!" << std::endl;
		return false;
	}
	else
	{
		accesTokenFile << currentAccessToken << std::endl;
		accesTokenFile << currentUserName << std::endl;
		accesTokenFile << currentProfileId;
		accesTokenFile.close();
		std::cout << "[LoginHandler] stored new access token in file" << std::endl;
		LoggedIn = true;
		return true;
	}
	return false;
}

bool LoginHandler::LoginUser(const ptl::string& username, const ptl::string& password)
{
	std::cout << "[LoginHandler] Requesting access token..." << std::endl;
	
	config = tbsg::Config::Get();
	ptl::string url = config.apiHost;

	auto loginResult = apiParser.GetNetworkAccessToken(url, "/v1/login.php", tbsg::WebMethod::POST, ptl::vector<tbsg::postDataPair> { {"username", username}, { "password", password }});
	if (!loginResult.accessToken.empty())
	{
		currentAccessToken = loginResult.accessToken;
		currentProfileId = loginResult.profileId;
		currentUserName = username;
		return true;
	}
	return false;
}

bool LoginHandler::LoadGameData()
{
	tbsg::RestApiParsing apiParser;
	const ptl::string projectID = ptl::to_string(config.projectId);

	ptl::string url = config.apiHost;
	const ptl::string getGameDataString = apiParser.GetGameDataString(url, "/v1/getData.php", tbsg::WebMethod::POST, ptl::vector<tbsg::postDataPair> { {"projectId", projectID}, { "token", currentAccessToken } });
	if (!getGameDataString.empty())
	{
		std::cout << "[LoginHandler] Received gameData .json string " << std::endl;
		return this->ParseGameData(getGameDataString);
	}
	else
	{
		std::cout << "[LoginHandler] Failed to get GameData from webserver. " << std::endl;
	}
	return false;
}

bool LoginHandler::GetUsersDecks()
{
	tbsg::RestApiParsing apiParser;
	const ptl::string projectID = ptl::to_string(config.projectId);

	ptl::string url = config.apiHost;

	const ptl::string getDeckString = apiParser.GetUserDecksString(url, "/v1/getDeck.php", tbsg::WebMethod::POST, ptl::vector<tbsg::postDataPair> 
	{ 
		{"profileId", ptl::to_string(currentProfileId)}, { "token", currentAccessToken }, { "projectId", projectID }
	});
	if (!getDeckString.empty())
	{
		std::cout << "[LoginHandler] Received getDeck .json string " << std::endl;
		return this->ParseDeckData(getDeckString);
	}
	else
	{
		std::cout << "[LoginHandler] Failed to get 'getDeck' from webserver. " << std::endl;
	}
	return false;
}


void LoginHandler::Initialize(GameDatabase* databasePointer, gfx::IResourceManager* resourceManager)
{
	if (databasePointer != nullptr && resourceManager != nullptr)
	{
		database = databasePointer;
		gfxResourceManager = resourceManager;
	}
	else
	{
		std::cout << "[LoginHandler] required pointers for initialize are nullptr" << std::endl;
	}

}

bool LoginHandler::ParseGameData(const ptl::string& gameDataString)
{
	if (database == nullptr || gameDataString.empty())
	{
		std::cout << "[LoginHandler] database or string not valid " << std::endl;
		return false;
	}

	if (dataDataLoader.ParseGameData(gameDataString))
	{
		database->AddCards(std::move(dataDataLoader.GetLoadedCards()));
		database->AddCardRarities(std::move(dataDataLoader.GetLoadedCardRarities()));
		database->AddCardTypes(std::move(dataDataLoader.GetLoadedCardTypes()));
		database->AddMonsterCards(std::move(dataDataLoader.GetLoadedMonsterCards()));
		database->AddMonsterDecks(std::move(dataDataLoader.GetLoadedMonsterDecks()));
		std::cout << "[LoginHandler] Loaded all game data into the database! " << std::endl;

		auto cards = database->GetCardList();

		
		for (const auto& card : cards)
		{
			ptl::string cardString = "Card_" + card.meta.name + ".png";
			cardString.erase(remove_if(cardString.begin(), cardString.end(), isspace), cardString.end());
			if (!gfxResourceManager->ContainsTexture(cardString))
			{
				gfxResourceManager->CreateTexture(cardString);
			}
		}
		return true;
	}
	std::cout << "[LoginHandler] Failed to Parse GameData string! " << std::endl;
	return false;
}

bool LoginHandler::ParseDeckData(const ptl::string& deckDataString)
{
	if (database == nullptr || deckDataString.empty())
	{
		std::cout << "[LoginHandler] database or string not valid " << std::endl;
		return false;
	}

	if (dataDataLoader.ParseDeckData(deckDataString, database->GetCardList()))
	{
		std::cout << "[LoginHandler] Deck information parsed." << std::endl;
		database->AddDecks(std::move(dataDataLoader.GetLoadedDecks()));
		return true;
	}
	else
	{
		std::cout << "[LoginHandler] Failed to Parse DeckData" << std::endl;
	}
	return false;
}
