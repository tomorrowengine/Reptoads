#include "audio/AudioSystem.h"
#include <brofiler/Brofiler.h>
#include <fstream>
#include <rapidjson/error/en.h>

namespace audio 
{
	AudioSystem::AudioSystem() 
	{
		if (FMOD::System_Create(&system) != FMOD_OK)
		{
			std::runtime_error("Failed to initialize FMOD");
			return;
		}

        int driverCount = 0;
        system->getNumDrivers(&driverCount);

        if (driverCount == 0)
        {
            std::runtime_error("Failed to initialize FMOD");
            return;
        }

        system->init(36, FMOD_INIT_NORMAL, nullptr);

		
    }


    AudioSystem::~AudioSystem()
    {

    }

	void AudioSystem::Initialize()
	{
		LoadSoundsFromJson("audio.json");
	}

	bool AudioSystem::LoadSoundsFromJson(ptl::string filename)
	{
		auto fullPath = tbsg::Config::Get().MakeAudioPath(filename);

		std::ifstream ifStream{ fullPath.c_str() };
		ptl::string audioJson{ std::istreambuf_iterator<char>(ifStream),std::istreambuf_iterator<char>() };

		RapidJsonDocument jsonDocument;
		rapidjson::ParseResult jsonParseResult = jsonDocument.Parse< rapidjson::kParseCommentsFlag | rapidjson::kParseTrailingCommasFlag >(audioJson.c_str());

		if (jsonParseResult.IsError()) {
			std::cerr << "ERROR Json parsing error: '" << rapidjson::GetParseError_En(jsonParseResult.Code()) << "'.\n    And error code: " << jsonParseResult.Code() << " with offset: " << jsonParseResult.Offset() << std::endl;
			return false;
		}

		if (!jsonDocument.IsObject()) {
			std::cerr << "ERROR, root in json object is not an object!" << std::endl;
			return false;
		}

		RapidJsonValue& value = jsonDocument;
		bool valid = value.HasMember("audioclips");
		if(valid)
		{
			auto arr = value.FindMember("audioclips")->value.GetArray();

			for(size_t i = 0; i < arr.Size(); ++i)
			{
				CreateSound(arr[i].GetString());
			}

			return true;
		}
		else
		{
			return false;
		}

	}

	void AudioSystem::CreateSound(ptl::string name)
    {
        const ptl::string fullPath = tbsg::Config::Get().MakeAudioPath(name);

        auto it = loadedSounds.find(lastID);

        if (it == loadedSounds.end()) 
		{
            FMOD::Sound* sound;
            if (system->createSound(fullPath.c_str(), FMOD_DEFAULT, 0, &sound) == FMOD_OK) 
			{
				soundNames.push_back(std::make_pair(lastID, name));
                loadedSounds.emplace(lastID, sound);
				lastID++;
            }
        }

    }

	void AudioSystem::PlaySoundEffect(unsigned int clipID, bool loop)
    {
        auto sound = loadedSounds.find(clipID);

        if (sound != loadedSounds.end()) {
            if (!loop)
            {
	            sound->second->setMode(FMOD_LOOP_OFF);
            }
            else
            {
                sound->second->setMode(FMOD_LOOP_NORMAL);
                sound->second->setLoopCount(-1);
            }
            FMOD::Channel* channel = nullptr;

            if (system->playSound(sound->second, 0, false, &channel) == FMOD_OK) 
			{
                playingChannels.push_back(channel);
            }
        }

    }

	void AudioSystem::StopAllSoundEffects()
	{
		auto it = playingChannels.begin();

		while(it != playingChannels.end())
		{
			it = playingChannels.erase(it);
		}
	}

	void AudioSystem::StartMusic(unsigned int clipID, bool continuous)
	{
		auto track = loadedSounds.find(clipID);

		if (track != loadedSounds.end()) 
		{
			if (!continuous)
			{
				track->second->setMode(FMOD_LOOP_OFF);
			}		
			else
			{
				track->second->setMode(FMOD_LOOP_NORMAL);
				track->second->setLoopCount(-1);
			}

			bool isPlaying = false;
			musicChannel->isPlaying(&isPlaying);

			if(isPlaying)
			{
				StopMusic();
			}

			system->playSound(track->second, 0, false, &musicChannel);
		}
	}

	void AudioSystem::StopMusic()
	{
		if(musicChannel != nullptr)
		{
			musicChannel->stop();
		}
	}

	

	bool AudioSystem::IsPlaying()
	{
		if (musicChannel != nullptr)
		{
			bool isPlaying = false;
			musicChannel->isPlaying(&isPlaying);
			return isPlaying;
		}
	}

	bool AudioSystem::IsPaused()
	{
		bool isPlaying = false;
		musicChannel->getPaused(&isPlaying);
		return isPlaying;
	}

	void AudioSystem::ResumeMusic()
	{
		musicChannel->setPaused(false);
	}

	void AudioSystem::PauseMusic()
	{
		musicChannel->setPaused(true);
	}

	void AudioSystem::Update()
	{ 

        system->update();

        auto it = playingChannels.begin();

        while (it != playingChannels.end()) 
		{
            bool playing{};
            (*it)->isPlaying(&playing);

            if (!playing) 
			{
                it = playingChannels.erase(it);
            }
            else 
			{
                ++it;
            }
        }
    }

	ptl::string AudioSystem::GetSoundNameFromID(unsigned clipID)
	{
		for (auto sound : soundNames)
		{
			if (sound.first == clipID)
			{
				return sound.second;
			}
		}

		return "";
	}

	unsigned AudioSystem::GetSoundIDFromName(ptl::string name)
	{
		for (auto sound : soundNames)
		{
			if (sound.second == name)
			{
				return sound.first;
			}
		}

		return -1;
	}
}
