#pragma once
#include <string>

namespace db
{
	struct DatabaseCredentials
	{
		std::string host;
		std::string username;
		std::string password;
		std::string database;
		unsigned int port;
	};
}
