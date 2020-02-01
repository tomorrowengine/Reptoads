#pragma once

struct LobbyEntity
{
    unsigned int LobbyID{};
    unsigned int ownerID{};
    unsigned int playerCount{};
    bool inGame = false;
};
