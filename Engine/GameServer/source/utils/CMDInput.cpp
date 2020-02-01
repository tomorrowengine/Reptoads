#include "utils/CMDInput.h"
#include "Net/Server.h"
#include "RTSARG.h"
#include "GameServer.h"
#include "core/StrHash.h"
#include "GameDataDatabase.h"
#include "model/GameLogic.h"

#include <cxxopts.hpp>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif
#include "LoggingFunction.h"
#include "gameplay/MatchHelper.h"
#include "Net/Packet.h"

void ClearScreen()
{
#ifdef _WIN32

    HANDLE hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = {0, 0};

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE)
        return;

    
    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi))
        return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    
    if (!FillConsoleOutputCharacter(hStdOut, (TCHAR)' ', cellCount, homeCoords, &count))
        return;

    
    if (!FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count))
        return;

    
    SetConsoleCursorPosition(hStdOut, homeCoords);
#else
	std::cout << "\033[2J\033[1;1H";

#endif
}

void CMDInput::ReadCin(std::atomic<bool> &run, std::mutex &mutex, std::vector<std::string> &msgQueue)
{
    while (run.load()) {
        std::string buffer;
        std::getline(std::cin, buffer);
        if (buffer == "quit") {
            run.store(false);
        }
        else if (buffer == "clear") {
            ClearScreen();
        }
        else {
            std::lock_guard<std::mutex> lock_guard(mutex);
            msgQueue.push_back(buffer);
        }
    }
}

void CMDInput::HandleCommands(RTSARG &args, tbsg::GameServer* server)
{
    try {
        char **argv = args.GetArgv();
        int argc = args.GetArgc();

        if (argc > 0) {
            std::string baseCommand(args.GetArgv()[0]);

            if (baseCommand == "help") {
                printf("Commands:\n");
                printf("quit\tstops the server\n");
                printf("help\tdisplays this info\n");
                printf("clear\tclears screen\n");
                printf("debug, --on, --off\ttoggle the server to print client commands\n");
                printf("lobby\n\t-l,--list\tlist all lobbies.\n\t-s,--stop\tstops the specified lobby.\n\t-g,--ingame\tsets the ingame state of the lobby\n");
                printf("clients\n\t-l,--list\t lists all clients\n");
                printf("profiles\n\t-l,--list\t lists all profiles the system knows\n\t");
                std::cout << "-g,--get\t get profile by profile information name \n\t"
                          << "-n,--name\t gets the basic information about the profile. Example:-g -n UserNamex64 \n\t"
                          << "-c,--cards\t gets a list of all the cards the player owns.\n";
                printf("profile\n\t-a,--a,-p,-player,-b,-bot,-n,--name\t Add player/bot\n\t");
                printf("matches\n\t-l,--list\t lists all matches the system knows\n\t\n");
                printf("match\n\t-s,--simulate\t Simulate game with cards\n\t");
            }
            else if (baseCommand == "debug") {
                cxxopts::Options options(argv[0], "Example: debug --on");

                options.allow_unrecognised_options().add_options()("on", "On")("off", "Off");

                auto result = options.parse(argc, argv);

                if (result.count("on")) {
                    printf("Debug mode now on\n");
					server->SetDebug(true);
                }
                else if (result.count("off")) {
                    printf("Debug mode now off\n");
					server->SetDebug(false);
                }
                else {
                    printf(options.help().c_str());
                }
            }
            else if (baseCommand == "connections") {
                cxxopts::Options options(argv[0], "Example: clients --list");
                options.allow_unrecognised_options().add_options()("l,list", "List clients");
                auto result = options.parse(argc, argv);

                if (result.count("l")) {
                    auto connections = server->GetConnections();
                    if (!connections.empty()) {
                        std::cout << "List of clients:\n";
                        for (const auto& connection : connections) {
                            std::cout << "- ClientID: #" << connection.GetConnectionId() << " Address: " << NetUtils::EnetAddressToString(connection.GetPeer()->address)
                                      << "\n";
                        }
                    }
                    else {
                        std::cout << "No clients are connected\n";
                    }
                }
            }
            
            
            
            
            
            

            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            

            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
   
   
   
   

   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
			
			
			
   
   
   
   
   
   
   
			
			
			
			
			
			
			
			
   
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
   
			
   
   
            else {
				cof::Warn("[CMDINPUT] Unknown command. Use 'help' to see available commands.");
            }
        }
        else {
			cof::Warn("[CMDINPUT] Unknown command. Use 'help' to see available commands.");
        }
    }
    catch (const cxxopts::OptionException &e) { std::cout << "Error invalid option: " << e.what() << std::endl; }
}

void CMDInput::HandleParsedAndHandleCommands(tbsg::GameServer* server)
{
    std::lock_guard<std::mutex> lock_guard(m_mutex);

    auto it = m_msgQueue.begin();
    while (it != m_msgQueue.end()) {

        RTSARG args = RTSARG(*it);

        HandleCommands(args, server);

        it = m_msgQueue.erase(it);
    }
}

void CMDInput::Run()
{
    m_run.store(true);
    m_inputThread = std::thread(CMDInput::ReadCin, std::ref(m_run), std::ref(m_mutex), std::ref(m_msgQueue));
}

void CMDInput::Release()
{
    m_run.store(false);
    m_inputThread.join();
}

bool CMDInput::IsRunning() { return m_run.load(); }
