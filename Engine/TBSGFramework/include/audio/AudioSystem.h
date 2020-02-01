#pragma once

#include "fmod.hpp"
#include "fmod_errors.h" 

#include "memory/smart_ptr.h"
#include "memory/Containers.h"
#include "memory/String.h"

#include "core/Config.h"
#include "core/StrHash.h"
#include "memory/RapidJsonAllocator.h"
#include <rapidjson/allocators.h>
#include <rapidjson/encodings.h>
#include <rapidjson/document.h>


namespace audio {

	using RapidJsonValue = rapidjson::GenericValue<rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<RapidJsonAllocator>>;
	using RapidJsonDocument = rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<RapidJsonAllocator>, RapidJsonAllocator>;

	class AudioSystem
	{
	public:
		AudioSystem();
		~AudioSystem();

		void Initialize();

		bool LoadSoundsFromJson(ptl::string filename);
		void CreateSound(ptl::string name);
		void PlaySoundEffect(unsigned int clipID, bool loop = false);
		void StopAllSoundEffects();

		void StartMusic(unsigned int clipID, bool continuous = true);
		void StopMusic();

		bool IsPlaying();
		bool IsPaused();
		void ResumeMusic();
		void PauseMusic();

		void Update();

		ptl::string GetSoundNameFromID(unsigned int clipID);
		unsigned int GetSoundIDFromName(ptl::string name);


	private:
		FMOD::System* system;

		ptl::vector<std::pair<unsigned int, ptl::string>> soundNames{};
		ptl::unordered_map<unsigned int, FMOD::Sound*> loadedSounds{};

		ptl::vector<FMOD::Channel*> playingChannels;
		FMOD::Channel* musicChannel = nullptr;

		unsigned int lastID = 0;
	};
}
