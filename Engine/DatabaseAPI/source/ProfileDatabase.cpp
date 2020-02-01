#include "ProfileDatabase.h"

#include "Payloads.h"

#include "DatabaseAPI.h"
#include "Net/Connection.h"
#include "LoggingFunction.h"

void tbsg::ProfileDatabase::Initialize(db::DatabaseAPI* api)
{
	this->api = api;
}

tbsg::SessionTokenResponse tbsg::ProfileDatabase::CheckSessionToken(net::Connection* connection, ptl::string token)
{
	unsigned int profileId = api->GetProfileId(token);

	if (profileId == 0)
	{
		return SessionTokenResponse::Invalid;
	}

	for(const auto& pair : profiles)
	{
		if(pair.second.id != profileId)
		{
			continue;
		}
		if(pair.second.connectionId != 0)
		{
			cof::Info("{} Connection {} tried to connect with profile {} that is already in use.", prefix, static_cast<int>(connection->GetConnectionId()), static_cast<int>(profileId));
			return SessionTokenResponse::InUse;
		}
	}

	Profile profile = api->GetProfile(profileId);
	if (profile.id == 0)
	{
		return SessionTokenResponse::Invalid;
	}
	profile.connectionId = connection->GetConnectionId();
	profiles[profile.id] = profile;
	return SessionTokenResponse::Success;
}

bool tbsg::ProfileDatabase::LoadProfile(unsigned int profileId)
{
	const auto itr = std::find_if(profiles.begin(), profiles.end(), [profileId](const auto& obj) { return obj.second.id == profileId; });
	if (itr != profiles.end()) return true;

	const Profile profile = api->GetProfile(profileId);
	if (profile.id == 0)
	{
		return false;
	}
	profiles[profile.id] = profile;
	return true;
}

const std::unordered_map<unsigned int, tbsg::Profile>& tbsg::ProfileDatabase::GetProfiles() const
{
	return profiles;
}

tbsg::Profile* tbsg::ProfileDatabase::GetProfile(unsigned int profileId)
{
	auto it = profiles.find(profileId);
	if (it != profiles.end())
	{
		return &it->second;
	}
	return nullptr;
}

tbsg::Profile* tbsg::ProfileDatabase::GetProfileUsingConnection(unsigned connectionId)
{
	for (auto& pair : profiles)
	{
		if (pair.second.connectionId == connectionId)
		{
			return &pair.second;
		}
	}
	cof::Info("Did not find any profiled based on connectionId {}", connectionId);
	return nullptr;
}

tbsg::Profile* tbsg::ProfileDatabase::GetProfile(net::Connection* connection)
{
	return this->GetProfileUsingConnection(connection->GetConnectionId());
}
