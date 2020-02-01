#include "GameDatabase.h"
#include "Net/Packet.h"
#include "core/StrHash.h"
#include "gameplay/scripting/LuaContextHelper.h"
#include "core/Config.h"
#include "utils/GameDataLoader.h"

void GameDatabase::SetNetworkId(unsigned int netID)
{
    m_NetworkID = netID;
}

void GameDatabase::InitMaps(std::size_t size)
{
	m_maps.reserve(size);
}

void GameDatabase::AddMaps(Packet&& packet)
{
	unsigned int mapSequenceID = 0;
	packet >> mapSequenceID;

}

void GameDatabase::AddMaps(unsigned id, ptl::vector<ptl::string>&& maps)
{
	m_maps.insert(id, std::move(maps));
}

tbsg::Card* GameDatabase::GetCard(unsigned id)
{
	for (auto& card : cards)
	{
		if (card.id == id)
		{
			return &card;
		}
	}
	return nullptr;
}

const ptl::vector<ptl::string>& GameDatabase::GetMap(unsigned int id)
{
	return m_maps.at(id);
}