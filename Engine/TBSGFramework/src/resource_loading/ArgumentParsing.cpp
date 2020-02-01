#include "resource_loading/ArgumentParsing.h"
#include "memory/Containers.h"
#include "memory/string.h"
#include "core/Config.h"
#include "utils/container_utils.h"
#include <string>

void tbsg::ParsedArguments::AddResultStr(ptl::string argument, ptl::string value)
{
    strArguments.emplace(argument, value);
}

bool tbsg::ParsedArguments::HasResultStr(ptl::string argument)
{
    auto result = std::find_if(strArguments.begin(), strArguments.end(),[&argument](const auto& key)
    {
        return key.first == argument;
    });
    return result != strArguments.end();
}

ptl::string& tbsg::ParsedArguments::operator[](ptl::string&& argument)
{
    return strArguments[ptl::string(argument)];
}

tbsg::Config& tbsg::ArgumentParsing(Config& config, int argc,char** argv, ParsedArguments& arguments)
{
    ptl::vector<ptl::string> allArguments(argv + 1, argv + argc);
    for (size_t i = 0; i < allArguments.size(); ++i) {
		auto& command = allArguments[i];
        if (command == "--rendering-disable-text")
            config.enableTextRendering = false;
        if (command == "--rendering-disable-ui")
            config.enableUIRendering = false;
		if (command == "--load-model" && !arguments.HasResultStr("--load-model")) {
			if(vector_in_range(allArguments, i+1)) 
			{
				ptl::string pathToLoad = allArguments[i + 1];
				if(pathToLoad[0] == '-' && pathToLoad[1] == '-') {
					std::cerr << "No valid model path specified after --load-model argument. Expected model name should NOT start with two dashes '--'. "
						"Provided model name is: '" << pathToLoad << "'. \nNOTE for loading a model with a space in the name: \"Wrap the name in quotes\"." << std::endl;;
				} else {
                    arguments.AddResultStr("--load-model",pathToLoad);
				}
			} else {
				std::cout << "No model path specified after --load-model argument" << std::endl;
			}
		}
		if (command == "--simulate-game")
		{
			if (vector_in_range(allArguments, i + 5))
			{
				auto checkIfNumber = [](ptl::string & stringToCheck)
				{
					return std::all_of(stringToCheck.begin(), stringToCheck.end(), ::isdigit);
				};
				
				if (checkIfNumber(allArguments[i + 1]) && checkIfNumber(allArguments[i + 2]) && 
					!checkIfNumber(allArguments[i + 3]) && !checkIfNumber(allArguments[i + 4]) && !checkIfNumber(allArguments[i + 5]))
				{
					config.simulateGame = true;
				}
				else
				{
					std::cout << "[ArgumentParsing] First two arguments are expected to be player numbers, third, fourth and fifth a deck name." << std::endl;
				}
			}
			else
			{ 
				std::cout << "[ArgumentParsing] Amount expected arguments not valid, e.g: --simulate-game 1 2 MonsterDeck1 Deck2 Deck5" << std::endl;
			}
		}
		if (command == "--preview-particle" && !arguments.HasResultStr("--preview-particle")) {
			if (vector_in_range(allArguments, i+1)) {
				ptl::string particleToLoad = allArguments[i + 1];
				if (particleToLoad[0] == '-' && particleToLoad[1] == '-') {
					std::cerr << "No valid model path specified after --preview-particle argument. Expected model name should NOT start with two dashes '--'. "
						"Provided model name is: '" << particleToLoad << "'. \nNOTE for loading a model with a space in the name: \"Wrap the name in quotes\"." << std::endl;;
				} else {
                    arguments.AddResultStr("--preview-particle", particleToLoad);
				}
			} else {
				std::cout << "No model path specified after '--preview-particle' argument" << std::endl;
			}
		}
        if(command == "--ui" && !arguments.HasResultStr("--ui"))
        {
            if (vector_in_range(allArguments, i + 1))
            {
                arguments.AddResultStr("--ui", allArguments[i + 1]);
            }
        }
		if (command == "--port" && !arguments.HasResultStr("--port"))
		{
			if (vector_in_range(allArguments, i + 1))
			{
				config.port = ptl::stoul(allArguments[i + 1].c_str());
				config.gameServerAutoPortIncrement = false;
				arguments.AddResultStr("--port", allArguments[i + 1]);
			}
		}
		if (command == "--username" && !arguments.HasResultStr("--username"))
		{
			if (vector_in_range(allArguments, i + 1))
			{
				config.apiUsername = allArguments[i + 1];
				arguments.AddResultStr("--username", allArguments[i + 1]);
			}
		}
		if (command == "--password" && !arguments.HasResultStr("--password"))
		{
			if (vector_in_range(allArguments, i + 1))
			{
				config.apiPassword = allArguments[i + 1];
				arguments.AddResultStr("--password", allArguments[i + 1]);
			}
		}
		if (command == "--token" && !arguments.HasResultStr("--token"))
		{
			if (vector_in_range(allArguments, i + 1))
			{
				config.apiTokenFile = allArguments[i + 1];
				arguments.AddResultStr("--token", allArguments[i + 1]);
			}
		}
    }

    return config;
}
