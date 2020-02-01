#define WIN32_LEAN_AND_MEAN
#include "LoggingFunction.h"
#ifndef __UNITTEST__

#include "GameServer.h"

#include "Net/NetUtils.h"

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

	tbsg::GameServer server{ cof::basic_logger::s_logger, GetHostname() };
	server.Initialize(argc, argv);
#ifdef _WIN32
	tbsg::Config config = tbsg::Config::Get();
	SetConsoleTitleA(("GameServer (" + config.address + ":" + std::to_string(server.GetPort()) + ")").c_str());
#endif
	server.Run();

    cof::basic_logger::Deint();
	return 0;
}
#endif 
