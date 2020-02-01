#define WIN32_LEAN_AND_MEAN
#include "LoggingFunction.h"
#include "LobbyServer.h"
#include "core/Config.h"

std::string GetHostname()
{
	std::string hostname{};
#ifdef _WIN32
	hostname.resize(MAX_COMPUTERNAME_LENGTH + 1, ' ');
	DWORD size = hostname.size();
	if (GetComputerNameA(&hostname[0], &size))
	{
		hostname.resize(size);
		hostname.shrink_to_fit();
	}
	else
	{
		printf("%lu\n", GetLastError());
	}
#endif

	return hostname;
}

int main(int argc, char* argv[])
{
#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD consoleMode;	
	GetConsoleMode(hConsole, &consoleMode);
	consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hConsole, consoleMode);
#endif
	cof::basic_logger::InitStdOut();

	LobbyServer server{ cof::basic_logger::s_logger, GetHostname() };
	server.Initialize();
#ifdef _WIN32
	tbsg::Config config = tbsg::Config::Get();
	SetConsoleTitleA(("LobbyServer (" + config.address + ":" + std::to_string(server.GetPort()) + ")").c_str());
#endif
	server.Run();

	cof::basic_logger::Deint();
}
