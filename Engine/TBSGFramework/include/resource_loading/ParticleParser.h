#pragma once
#include "rendering/ParticleEmitter.h"
#include "rendering/IResourceManager.h"
#include "memory/RapidJsonAllocator.h"
#include <rapidjson/allocators.h>
#include <rapidjson/encodings.h>
#include <rapidjson/document.h>


namespace gfx
{
	using RapidJsonValue = rapidjson::GenericValue<rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<RapidJsonAllocator>>;
	using RapidJsonDocument = rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<RapidJsonAllocator>, RapidJsonAllocator>;

	class ParticleParser
	{
		gfx::IResourceManager* resourceManager;

	public:
		ParticleParser(gfx::IResourceManager& resourceManager) : resourceManager(&resourceManager) {}

		ptl::string SerializeParticle(const ParticleEmitter& emitter);
		std::pair<ParticleEmitter, bool> DeserializeParticle(const ptl::string& particleJson);
		std::pair<ParticleEmitter, bool> DeserializeParticleFromFile(const ptl::string& particleJsonPath);

	private:
		TextureId JsonGetTextureOrDefaultTexture(RapidJsonValue& jsonDocument, const ptl::string& key);
		VectorRange JsonGetVectorRangeOrAny(RapidJsonValue& jsonDocument, const ptl::string& key);
		ValueRange JsonGetValueRangeOrDefault(RapidJsonValue& jsonDocument, const ptl::string& key, const ValueRange& defaultValueRange);
		ValueRange JsonGetValueRangeOrDefault(RapidJsonValue& jsonDocument, const ptl::string& key, float defaultConstantValue);
		DirectionRange JsonGetDirectionRangeOrAny(RapidJsonValue& jsonDocument, const ptl::string& key);
		std::pair<glm::vec3, bool> JsonGetVec3(RapidJsonValue& jsonDocument, const ptl::string& key);
		ptl::string JsonGetStringOrDefault(RapidJsonValue& value, const ptl::string& key, const ptl::string& defaultValue);
		float JsonGetFloatOrDefault(RapidJsonValue& value, const ptl::string& key, float defaultValue);
		int JsonGetIntOrDefault(RapidJsonValue& value, const ptl::string& key, int defaultValue);
		bool JsonGetBoolOrDefault(RapidJsonValue& value, const ptl::string& key, bool defaultValue);
	};
}
