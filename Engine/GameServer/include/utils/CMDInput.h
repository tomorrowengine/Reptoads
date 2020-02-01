#pragma once
#include <atomic>
#include <mutex>
#include <functional>
#include <thread>

class Server;
namespace tbsg
{
    class GameServer;
}
struct RTSARG;

class CMDInput
{
public:
	void HandleParsedAndHandleCommands(tbsg::GameServer* server);
	void Run();
	void Release();

	bool IsRunning();

private:
	static void ReadCin(std::atomic<bool>& run, std::mutex& mutex, std::vector<std::string>& msgQueue);
	void HandleCommands(RTSARG& args, tbsg::GameServer* server);

	std::vector<std::string> m_msgQueue;
	std::mutex m_mutex;
	std::atomic<bool> m_run;
	std::thread m_inputThread;

};

