#pragma once
#include "core/Config.h"
#include "resource_loading/RestApiParsing.h"
#include "utils/GameDataLoader.h"

class GameDatabase;
namespace gfx
{
    class IResourceManager;
}

// LoginHandler - A Wrapper class that mainly uses the WebRequest class to requests and process anything related to logging in into the game

class LoginHandler
{
public:
    LoginHandler() = default;
    ~LoginHandler() = default;

    // Sets required references.
    void Initialize(GameDatabase* databasePointer = nullptr, gfx::IResourceManager* resourceManager = nullptr);

    // Login handling
    // Checks for a matching access token file based on the config settings, returns whether the token has been found and used.
    bool CheckForLocalAccessToken();
    // Requests whether the locally found token is still valid according to the server database, returns whether the token is valid according to the server.
    bool CheckIfAccessTokenValid();
    // Based on the current user information, create a new login session on disk, returns whether a file could be made.
    bool WriteNewAccessToken(const ptl::string& newAccessToken = {}, const unsigned int newProfileID = 0, const ptl::string& newUsername = {});
    // Atempt to login the user, returns whether the server confirmed the login and credentials are valid.
    bool LoginUser(const ptl::string& username, const ptl::string& password);

    // Data processing
    // Requests from the server the game data associated with the game, returns whether this data could be successfully requested and parsed.
    bool LoadGameData();
    // Requests from the server the decks from the current user, returns whether this data could be successfully requested and parsed.
    bool GetUsersDecks();


    //Public getters
    ptl::string& GetCurrentAccessToken() { return currentAccessToken; }
    ptl::string& GetCurrentUserName() { return currentUserName; }
private:
    // Parse the retrieved data from the LoadGameData() function, returns whether data could be parsed successfully.
    bool ParseGameData(const ptl::string& gameDataString = {});
    // Parse the retrieved data from the GetUsersDecks() function, returns whether data could be parsed successfully.
    bool ParseDeckData(const ptl::string& deckDataString = {});

    //Engine related pointers and helper classes.
    gfx::IResourceManager* gfxResourceManager = nullptr;
    GameDatabase* database = nullptr;
    GameDataLoader dataDataLoader{};

    //Handling and storing user info
    tbsg::Config config;
    tbsg::RestApiParsing apiParser{};
    ptl::string currentAccessToken{};
    ptl::string currentUserName{};
    unsigned int currentProfileId{};

    //State handling
    bool LoggedIn = false;
    bool debugMode = false;
};