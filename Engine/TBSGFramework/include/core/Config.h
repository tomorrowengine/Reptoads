#pragma once
#include <memory/String.h>
namespace tbsg
{
    struct Config
    {
		unsigned int projectId{};
		bool production{};
        ptl::string address{};
        unsigned int port{};
		bool gameServerAutoPortIncrement{};
        unsigned int maxConnection{};
		ptl::string databaseHost{};
		unsigned int databasePort{};
		ptl::string databaseUsername{};
		ptl::string databasePassword{};
		ptl::string databaseName{};
		ptl::string apiHost{};
		ptl::string apiUsername{};
		ptl::string apiPassword{};
		ptl::string apiTokenFile{};
        ptl::string cardsFolder{};
        ptl::string scriptsFolder{};
        ptl::string profilesFolder{};
        ptl::string shadersFolder{};
        ptl::string shadersFolderBinary{};
        ptl::string texturesFolder{};
        ptl::string modelsFolder{};
		ptl::string uiFolder{};
		ptl::string audioFolder{};
		ptl::string particleFolder{};
        ptl::string username{};
        ptl::string mapsFile{};
        ptl::string aiFile{};
        ptl::string balancingDataFolder{};
        unsigned int width{};
        unsigned int height{};
        double maxSteptime{};
        unsigned int maxSteps{};
        bool enableTextRendering{ true };
        bool enableUIRendering{ true };
        bool simulateGame{ false };

	    ptl::string GetAddress() const
	    {
		    return address;
	    }

		bool IsProduction() const
	    {
			return production;
	    }

	    unsigned GetPort() const
	    {
		    return port;
	    }

	    unsigned GetMaxConnection() const
	    {
		    return maxConnection;
	    }

		ptl::string GetDatabaseHost() const
		{
			return databaseHost;
		}

		unsigned int GetDatabasePort() const
		{
			return databasePort;
		}

		ptl::string GetDatabaseUsername() const
		{
			return databaseUsername;
		}

		ptl::string GetDatabasePassword() const
		{
			return databasePassword;
		}

		ptl::string GetDatabaseName() const
		{
			return databaseName;
		}

		ptl::string GetApiHost() const
		{
			return apiHost;
		}

		ptl::string GetApiUsername() const
		{
			return apiUsername;
		}

		ptl::string GetApiPassword() const
		{
			return apiPassword;
		}
		ptl::string GetApiTokenFile() const
		{
			return apiTokenFile;
		}

	    ptl::string GetCardsFolder() const
	    {
		    return cardsFolder;
	    }

	    ptl::string GetScriptsFolder() const
	    {
		    return scriptsFolder;
	    }

	    ptl::string GetProfilesFolder() const
	    {
		    return profilesFolder;
	    }

	    const ptl::string& GetShadersFolder() const
	    {
		    return shadersFolder;
	    }

	    ptl::string GetShadersFolderBinary() const
	    {
		    return shadersFolderBinary;
	    }

	    ptl::string GetTexturesFolder() const
	    {
		    return texturesFolder;
	    }

	    ptl::string GetModelsFolder() const
	    {
		    return modelsFolder;
	    }

		ptl::string GetUIFolder() const
		{
			return uiFolder;
		}

		ptl::string GetAudioFolder() const
		{
			return audioFolder;
		}

        ptl::string GetMapFile() const
        {
            return mapsFile;
        }

	    ptl::string GetUsername() const
	    {
		    return username;
	    }

	    unsigned GetWidth() const
	    {
		    return width;
	    }

	    unsigned GetHeight() const
	    {
		    return height;
	    }

        ptl::string GetBalancingData() const
        {
            return balancingDataFolder;
        }

        inline ptl::string MakeShaderPath(const ptl::string& shader,unsigned int flag = 0) const
        {
            if(flag == 0)
                return shadersFolder + shader;
            else
                return shadersFolderBinary+shader;
        }
        inline ptl::string MakeShaderPath(const char * shader, unsigned int flag = 0) const
        {
            if (flag == 0)
                return shadersFolder + shader;
            else
                return shadersFolderBinary + shader;
        }

        inline ptl::string MakeScriptPath(const char* type, const ptl::string& script) const
        {
            return scriptsFolder +"/"+type+"/"+ script + ".lua";
        }
        inline ptl::string MakeScriptPath(const char* type, const char* script) const
        {
            return scriptsFolder + "/" + type + "/" + script + ".lua";
        }

        inline ptl::string MakeTexturePath(const ptl::string& texture) const
        {
            return texturesFolder + texture;
        }
        inline ptl::string MakeTexturePath(const char* texture) const
        {
            return texturesFolder + texture;
        }
        inline ptl::string MakeModelPath(const ptl::string& texture) const
        {
            return modelsFolder + texture;
        }

		inline ptl::string MakeModelPath(const char* texture) const
		{
			return modelsFolder + texture;
		}

		inline ptl::string MakeUIPath(const ptl::string& ui) const
		{
			return uiFolder + ui;
		}

		inline ptl::string MakeAudioPath(const ptl::string& audio) const
		{
			return audioFolder + audio;
		}

		inline ptl::string MakeUIPath(const char* ui) const
		{
			return uiFolder + ui;
		}

		inline ptl::string MakeParticlePath(const ptl::string& particle) const
	    {
			return particleFolder + particle;
	    }

        inline ptl::string MakeMapPath(const ptl::string& map) const 
        {
            return mapsFile + map;
        }

        inline ptl::string MakeAIPath(const ptl::string& map) const
        {
            return mapsFile + map;
        }

        
        static Config& Get();
    };
}
