#pragma once
#include "core/Config.h"
#include "resource_loading/RestApiParsing.h"
#include "utils/GameDataLoader.h"

class GameDatabase;
namespace gfx
{
	class IResourceManager;
}

class LoginHandler
{
public:
	LoginHandler() = default;
	~LoginHandler() = default;

	bool CheckForLocalAccessToken();
	bool CheckIfAccessTokenValid();
	bool WriteNewAccessToken(const ptl::string& newAccessToken = {}, const unsigned int newProfileID = 0, const ptl::string & newUsername = {});
	bool LoginUser(const ptl::string& username, const ptl::string& password);
	bool LoadGameData();
	bool GetUsersDecks();
	void Initialize(GameDatabase* databasePointer = nullptr, gfx::IResourceManager* resourceManager = nullptr);
	ptl::string& GetCurrentAccessToken() { return currentAccessToken; }
	ptl::string& GetCurrentUserName() { return currentUserName; }
private:
	bool ParseGameData(const ptl::string& gameDataString = {});
	bool ParseDeckData(const ptl::string& deckDataString = {});

	bool LoggedIn = false;
	bool debugMode = false;
	ptl::string currentAccessToken{};
	ptl::string currentUserName{};
	unsigned int currentProfileId{};
	tbsg::Config config;
	tbsg::RestApiParsing apiParser{};
	GameDatabase* database = nullptr;
	GameDataLoader dataDataLoader{};
	gfx::IResourceManager* gfxResourceManager = nullptr;
};