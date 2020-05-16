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
        tokenFile.open((prefix + config.apiTokenFile).c_str());
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
        return apiParser.CheckIfSessionIsValid(
            url,
            "/projects/reptoads/v1/checkSession.php",
            tbsg::WebMethod::POST,
            ptl::vector<tbsg::postDataPair> { {"token", currentAccessToken} }
        );
    }
}

bool LoginHandler::WriteNewAccessToken(const ptl::string& newAccessToken, const unsigned int newProfileID, const ptl::string& newUsername)
{
    // Check if all provided parameters are valid.
    if (newAccessToken.empty() && currentAccessToken.empty() && newUsername.empty())
    {
        return false;
    }

    if (newProfileID == 0 && currentProfileId == 0)
    {
        return false;
    }

    // Set all provided values.
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

    // Create an output stream to write user data on disk, debug mode is used to read the data in plain text.
    std::ofstream accessTokenFile;
    if (debugMode)
    {
        accessTokenFile.open((config.apiTokenFile).c_str(), std::ios::trunc);
    }
    else
    {
        accessTokenFile.open((config.apiTokenFile).c_str(), std::ios::trunc | std::ios::binary);

    }
    // If file could be opened, write safely the data.
    if (accessTokenFile.is_open())
    {
        // Write all data
        accessTokenFile << currentAccessToken << std::endl;
        accessTokenFile << currentUserName << std::endl;
        accessTokenFile << currentProfileId;
        accessTokenFile.close();
        std::cout << "[LoginHandler] stored new access token in file" << std::endl;
        LoggedIn = true;
        return true;
    }

    std::cout << "[LoginHandler] Could not open local file!" << std::endl;
    return false;
}

bool LoginHandler::LoginUser(const ptl::string& username, const ptl::string& password)
{
    std::cout << "[LoginHandler] Requesting access token..." << std::endl;

    // Use the host from the config file as base URL for the next request.
    config = tbsg::Config::Get();

    // Attempt to send the login information through the Api Parser and web request class.
    auto loginResult = apiParser.GetNetworkAccessToken(
        config.apiHost,
        "/projects/reptoads/v1/login.php",
        tbsg::WebMethod::POST, ptl::vector<tbsg::postDataPair> { {"username", username}, { "password", password }}
    );

    // If login access token returned successfully, set data.
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
    // Setup parsing helper
    tbsg::RestApiParsing apiParser;

    // Use the host from the config file as base URL for the next request.
    config = tbsg::Config::Get();

    const ptl::string projectID = ptl::to_string(config.projectId);

    // Request the game data from the rest api as a whole strong
    const ptl::string getGameDataString = apiParser.GetGameDataString(
        config.apiHost,
        "/projects/reptoads/v1/getData.php",
        tbsg::WebMethod::POST,
        ptl::vector<tbsg::postDataPair> { {"projectId", projectID}, { "token", currentAccessToken } }
    );

    //If info could successfully be retrieved, parse the data.
    if (!getGameDataString.empty())
    {
        std::cout << "[LoginHandler] Received gameData .json string " << std::endl;
        return this->ParseGameData(getGameDataString);
    }

    std::cout << "[LoginHandler] Failed to get GameData from webserver. " << std::endl;
    return false;
}

bool LoginHandler::GetUsersDecks()
{
    // Setup parsing helper
    tbsg::RestApiParsing apiParser;

    // Use the host from the config file as base URL for the next request.
    config = tbsg::Config::Get();

    const ptl::string projectID = ptl::to_string(config.projectId);

    // Retrieve the user data using the apiParser and the web request.
    const ptl::string getDeckString = apiParser.GetUserDecksString(
        config.apiHost,
        "/projects/reptoads/v1/getDeck.php",
        tbsg::WebMethod::POST,
        ptl::vector<tbsg::postDataPair>
    {
        {"profileId", ptl::to_string(currentProfileId)}, { "token", currentAccessToken }, { "projectId", projectID }
    }
    );

    // If data was retreived, parse the data.
    if (!getDeckString.empty())
    {
        std::cout << "[LoginHandler] Received getDeck .json string " << std::endl;
        return this->ParseDeckData(getDeckString);
    }


    std::cout << "[LoginHandler] Failed to get 'getDeck' from webserver. " << std::endl;
    return false;
}


void LoginHandler::Initialize(GameDatabase* databasePointer, gfx::IResourceManager* resourceManager)
{
    // Validate provided pointers
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
    // Only operate if required database is setup and the string has been populated
    if (database == nullptr || gameDataString.empty())
    {
        std::cout << "[LoginHandler] database or string not valid " << std::endl;
        return false;
    }

    // Parse the data with rapidjson and see whether it was valid, if so, move the data into the local database.
    if (dataDataLoader.ParseGameData(gameDataString))
    {
        database->AddCards(std::move(dataDataLoader.GetLoadedCards()));
        database->AddCardRarities(std::move(dataDataLoader.GetLoadedCardRarities()));
        database->AddCardTypes(std::move(dataDataLoader.GetLoadedCardTypes()));
        database->AddMonsterCards(std::move(dataDataLoader.GetLoadedMonsterCards()));
        database->AddMonsterDecks(std::move(dataDataLoader.GetLoadedMonsterDecks()));
        std::cout << "[LoginHandler] Loaded all game data into the database! " << std::endl;


        // Loop and create textures based on the card names
        auto cards = database->GetCardList();
        for (const auto& card : cards)
        {
            // Add the Card_ prefix and .png in between as our used file format
            ptl::string cardString = "Card_" + card.meta.name + ".png";

            // Remove spaces from names that accidently have been used in the online database to prevent crashes.
            cardString.erase(remove_if(cardString.begin(), cardString.end(), isspace), cardString.end());

            // Add Texture if not created yet.
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
    // Only operate if the required database is set and the string is populated.
    if (database == nullptr || deckDataString.empty())
    {
        std::cout << "[LoginHandler] database or string not valid " << std::endl;
        return false;
    }

    //Parse the deck data using the helper
    if (dataDataLoader.ParseDeckData(deckDataString, database->GetCardList()))
    {
        std::cout << "[LoginHandler] Deck information parsed." << std::endl;
        database->AddDecks(std::move(dataDataLoader.GetLoadedDecks()));
        return true;
    }

    std::cout << "[LoginHandler] Failed to Parse DeckData" << std::endl;
    return false;
}
