#include "resource_loading/ParticleParser.h"
#include <rapidjson/document.h>
#include "memory/RapidJsonAllocator.h"
#include <fstream>
#include <rapidjson/error/en.h>

namespace gfx
{
	static int FileExists(const ptl::string& fname)
	{
		std::ifstream infile(fname.c_str());
		return infile.good();
	}

	ptl::string ParticleParser::SerializeParticle(const ParticleEmitter& emitter)
	{
		UNUSED(emitter);

		ASSERT_MSG(false, "Not implemented yet!");
		return "";
	}

	std::pair<ParticleEmitter, bool> ParticleParser::DeserializeParticle(const ptl::string& particleJson)
	{
		RapidJsonDocument jsonDocument;
		rapidjson::ParseResult jsonParseResult = jsonDocument.Parse< rapidjson::kParseCommentsFlag | rapidjson::kParseTrailingCommasFlag >(particleJson.c_str());
		
		if(jsonParseResult.IsError()) {
			std::cerr << "ERROR Json parsing error: '" << rapidjson::GetParseError_En(jsonParseResult.Code()) <<  "'.\n    And error code: " << jsonParseResult.Code() << " with offset: " << jsonParseResult.Offset() << std::endl;
			return { {}, false };
		}

		if(!jsonDocument.IsObject()) {
			std::cerr << "ERROR, root in json object is not an object!" << std::endl;
			return { {}, false };
		}

		ptl::string name = JsonGetStringOrDefault(jsonDocument, "name", "");
		TextureId diffuseTexture = JsonGetTextureOrDefaultTexture(jsonDocument, "diffuseTexture");
		TextureId emissiveTextue = JsonGetTextureOrDefaultTexture(jsonDocument, "emissiveTextue");

		VectorRange spawnOffset				= JsonGetVectorRangeOrAny(jsonDocument, "spawnOffset");
		int			maxParticleAmount		= JsonGetIntOrDefault(jsonDocument, "maxAmount", 500); 
		ValueRange	lifetime				= JsonGetValueRangeOrDefault(jsonDocument, "lifetime", 0.0f);
		VectorRange initialVelocity			= JsonGetVectorRangeOrAny(jsonDocument, "initialVelocity");
		ValueRange	initialScale			= JsonGetValueRangeOrDefault(jsonDocument, "initialScale", 1.0f);
		ValueRange	initialParticleAmount	= JsonGetValueRangeOrDefault(jsonDocument, "initialParticleAmount", 10.0f);
		ValueRange	velocityChangeOverTime	= JsonGetValueRangeOrDefault(jsonDocument, "velocityChangeOverTime", 0.0f);
		bool		useGravity				= JsonGetBoolOrDefault(jsonDocument, "useGravity", true);
		ValueRange	particleSpawnRate		= JsonGetValueRangeOrDefault(jsonDocument, "particleSpawnRate", 0.0f);
		ValueRange	mass					= JsonGetValueRangeOrDefault(jsonDocument, "mass", 1.0f);

		ParticleEmitter::StartParams startParams{};
		startParams.billboardType = BillboardType::QuadLookTowardsCamera;
		startParams.simulationSpace = SimulationSpace::Local;

		startParams.lifeTime = lifetime;
		startParams.maxParticleAmount = maxParticleAmount;
		startParams.initialParticleAmount = initialParticleAmount;
		startParams.initialUniformScale = initialScale;
		startParams.initialVelocity = initialVelocity;
		startParams.particleSpawnRate = particleSpawnRate;
		startParams.spawnOffset = spawnOffset;
		startParams.useGravity = useGravity;
		startParams.velocityChangeOverTime = velocityChangeOverTime;
		startParams.mass = mass;

		core::Transform centerPos{ glm::vec3{0.0, 0.0f, 0.0f}, glm::quat{}, glm::vec3{1.0f, 1.0f, 1.0f} };
		ParticleEmitter emitter{ startParams, centerPos.GetPos(), resourceManager->GetTexture(diffuseTexture), resourceManager->GetTexture(emissiveTextue) };
		return { emitter, true };
	}

	std::pair<ParticleEmitter, bool> ParticleParser::DeserializeParticleFromFile(const ptl::string& particleJsonPath)
	{
		if (!FileExists(particleJsonPath)) {
			std::cerr << "ERROR could not find particle json file: '" << particleJsonPath << "'." << std::endl;
			return { {}, false };
		}

		std::ifstream ifStream{ particleJsonPath.c_str() };
		ptl::string particleJson{ std::istreambuf_iterator<char>(ifStream),
			std::istreambuf_iterator<char>() };

		return DeserializeParticle(particleJson);
	}

	TextureId ParticleParser::JsonGetTextureOrDefaultTexture(RapidJsonValue& jsonDocument, const ptl::string& key)
	{
		if(jsonDocument.HasMember(key.c_str())) {
			ptl::string texturePath = jsonDocument.FindMember(key.c_str())->value.GetString();
			if(!resourceManager->ContainsTexture(texturePath)) {
				return resourceManager->CreateTexture(texturePath);
			}
			return resourceManager->GetTextureId(texturePath);
		} else {
			return resourceManager->GetDefaultTextureId();
		}
	}

	VectorRange ParticleParser::JsonGetVectorRangeOrAny(RapidJsonValue& jsonDocument, const ptl::string& key)
	{
		VectorRange vectorRange;
		if(jsonDocument.HasMember(key.c_str())) {
			RapidJsonValue& value = jsonDocument.FindMember(key.c_str())->value;

			if (value.IsString() && value.GetString() == ptl::string{ "any" }) {
				vectorRange.SetAsUnitSphere();
				return vectorRange;
			} else if (value.IsObject()) {

				DirectionRange directionRange = JsonGetDirectionRangeOrAny(value, "direction");
				ValueRange lengthRange = JsonGetValueRangeOrDefault(value, "length", 1);

				vectorRange.direction = directionRange;
				vectorRange.magnitudeRange = lengthRange;
				return vectorRange;
			} else {
				std::cerr << "ERROR: VectorRange with key: '" << key << "' is not an object (which means curly braces {}) or given the string value 'any'!" << std::endl;
				return {};
			}
		} else {
			vectorRange.SetAsUnitSphere();
			return vectorRange;
		}
	}

	ValueRange ParticleParser::JsonGetValueRangeOrDefault(RapidJsonValue& jsonDocument, const ptl::string& key,
		const ValueRange& defaultValueRange)
	{
		ValueRange valueRange;

		if(jsonDocument.HasMember(key.c_str())) {
			RapidJsonValue& value = jsonDocument.FindMember(key.c_str())->value;
			float min, max;
			if(value.IsArray()) {
				auto array = value.GetArray();
				if(array.Size() != 2) {
					std::cerr << "ERROR: the array from: '" << key << "' should only have 2 elements! this one has: " << array.Size() << "!" << std::endl;
					return {};
				}
				if (!array[0].IsNumber()) {
					std::cerr << "ERROR: the array element from: '" << key << "' at element '0' is not a number!" << std::endl;
					return {};
				}
				if (!array[1].IsNumber()) {
					std::cerr << "ERROR: the array element from: '" << key << "' at element '1' is not a number!" << std::endl;
					return {};
				}

				min = array[0].GetFloat();
				max = array[1].GetFloat();
				valueRange.SetAsLineair(min, max);
				return valueRange;
			} else if (value.IsObject()) {
				if(!value.HasMember("min")) {
					std::cerr << "ERROR: no propery 'min' found on ValueRange: '" << key << "'!" << std::endl;
					return {};
				}
				if(!value.FindMember("min")->value.IsNumber()) {
					std::cerr << "ERROR: the propery 'min' found on ValueRange: '" << key << "' is not a number!" << std::endl;
					return {};
				}
				if(!value.HasMember("max")) {
					std::cerr << "ERROR: no propery 'max' found on ValueRange: '" << key << "'!" << std::endl;
					return {};
				}
				if (!value.FindMember("max")->value.IsNumber()) {
					std::cerr << "ERROR: the propery 'max' found on ValueRange: '" << key << "' is not a number!" << std::endl;
					return {};
				}

				min = value.FindMember("min")->value.GetFloat();
				max = value.FindMember("max")->value.GetFloat();

				valueRange.SetAsLineair(min, max);
				return valueRange;
			} else if(value.IsNumber()) {
				float constantNumber = value.GetFloat();
				
				valueRange.SetAsConstant(constantNumber);
				
				return valueRange;
			} else {
				std::cerr << "ERROR: Json element: '" << key << "' is not an array or an object or a constant value!" << std::endl;
				return {};
			}
		} else {
			return defaultValueRange;
		}
	}

	ValueRange ParticleParser::JsonGetValueRangeOrDefault(RapidJsonValue& jsonDocument, const ptl::string& key,
		float defaultConstantValue)
	{
		ValueRange defaultRange;
		defaultRange.SetAsConstant(defaultConstantValue);

		return JsonGetValueRangeOrDefault(jsonDocument, key, defaultRange);
	}

	DirectionRange ParticleParser::JsonGetDirectionRangeOrAny(RapidJsonValue& jsonDocument, const ptl::string& key)
	{
		DirectionRange directionRange;

		if (jsonDocument.HasMember(key.c_str())) {
			RapidJsonValue& value = jsonDocument.FindMember(key.c_str())->value;
			if (value.IsString()) {
				if (value.GetString() == ptl::string{ "any" }) {
					directionRange.SetAsAny();
					return directionRange;
				} else {
					std::cerr << "ERROR: A string value is specified at: '" << key << "' But only the value 'any' can be accepted. Supplied value: '" << value.GetString() << "'!" << std::endl;
					return { };
				}
			} else if (value.IsObject()) {
				if (!value.HasMember("min")) {
					std::cerr << "ERROR: no propery 'min' found on DirectionRange: '" << key << "'!" << std::endl;
					return {};
				}
				if (!value.HasMember("max")) {
					std::cerr << "ERROR: no propery 'max' found on DirectionRange: '" << key << "'!" << std::endl;
					return {};
				}

				bool successMin;
				glm::vec3 vec3Min;
				std::tie(vec3Min, successMin) = JsonGetVec3(value, "min");

				bool successMax;
				glm::vec3 vec3Max;
				std::tie(vec3Max, successMax) = JsonGetVec3(value, "max");

				if(!successMin) {
					std::cerr << "Could not parse min vector for DirectionRange at: '" << key << "'!" << std::endl;
					return {};
				}
				if (!successMax) {
					std::cerr << "Could not parse max vector for DirectionRange at: '" << key << "'!" << std::endl;
					return {};
				}

				directionRange.SetAsLineair(vec3Min, vec3Max);
				return directionRange;
			} else {
				std::cerr << "ERROR: Json element: '" << key << "' is not an array or an object or a constant value!" << std::endl;
				return {};
			}
		} else {
			directionRange.SetAsAny();
			return directionRange;
		}
	}

	std::pair<glm::vec3, bool> ParticleParser::JsonGetVec3(RapidJsonValue& jsonDocument, const ptl::string& key)
	{
		if (jsonDocument.HasMember(key.c_str())) {
			auto& value = jsonDocument.FindMember(key.c_str())->value;
			if (value.IsArray()) {
				auto array = value.GetArray();
				if (array.Size() != 3) {
					std::cerr << "ERROR: the array from: '" << key << "' should only have 3 elements! this one has: " << array.Size() << "!" << std::endl;
					return { {}, false };
				}
				if (!array[0].IsNumber()) {
					std::cerr << "ERROR: the array element from: '" << key << "' at element '0' is not a number!" << std::endl;
					return { {}, false };
				}
				if (!array[1].IsNumber()) {
					std::cerr << "ERROR: the array element from: '" << key << "' at element '1' is not a number!" << std::endl;
					return { {}, false };
				}
				if (!array[2].IsNumber()) {
					std::cerr << "ERROR: the array element from: '" << key << "' at element '2' is not a number!" << std::endl;
					return { {}, false };
				}

				float x = array[0].GetFloat();
				float y = array[1].GetFloat();
				float z = array[2].GetFloat();

				return { {x, y, z}, true };
			} else if(value.IsObject()) {
				if(!value.HasMember("x")) {
					std::cerr << "ERROR: the property 'x' is not found on vector with key: '" << key << "'!" << std::endl;
					return { {}, false };
				}
				if (!value.HasMember("y")) {
					std::cerr << "ERROR: the property 'y' is not found on vector with key: '" << key << "'!" << std::endl;
					return { {}, false };
				}
				if (!value.HasMember("y")) {
					std::cerr << "ERROR: the property 'y' is not found on vector with key: '" << key << "'!" << std::endl;
					return { {}, false };
				}
				if (!value.FindMember("x")->value.IsNumber()) {
					std::cerr << "ERROR: the property 'x' from vector with key: '" << key << "' is not a number!" << std::endl;
					return { {}, false };
				}
				if (!value.FindMember("y")->value.IsNumber()) {
					std::cerr << "ERROR: the property 'y' from vector with key: '" << key << "' is not a number!" << std::endl;
					return { {}, false };
				}
				if (!value.FindMember("z")->value.IsNumber()) {
					std::cerr << "ERROR: the property 'z' from vector with key: '" << key << "' is not a number!" << std::endl;
					return { {}, false };
				}

				float x = value.FindMember("x")->value.GetFloat();
				float y = value.FindMember("y")->value.GetFloat();
				float z = value.FindMember("z")->value.GetFloat();

				return { {x, y, z}, true };
			} else {
				std::cerr << "While parsing a vector at with key: '" << key << "' Only a array or an object are allowed!" << std::endl;
				return { {}, false };
			}
		} else {
			return { {}, false };
		}
	}

	ptl::string ParticleParser::JsonGetStringOrDefault(RapidJsonValue& value, const ptl::string& key, const ptl::string& defaultValue)
	{
		if (value.HasMember(key.c_str())) {
			return value.FindMember(key.c_str())->value.GetString();
		}
		return defaultValue;
	}

	float ParticleParser::JsonGetFloatOrDefault(RapidJsonValue& value, const ptl::string& key, float defaultValue)
	{
		if (value.HasMember(key.c_str())) {
			auto& subValue = value.FindMember(key.c_str())->value;
			if (!subValue.IsNumber()) {
				std::cerr << "ERROR element with key: '" << key << "' should be a number!" << std::endl;
				return 0.0f;
			}
			return subValue.GetFloat();
		} else {
			return defaultValue;
		}
	}

	int ParticleParser::JsonGetIntOrDefault(RapidJsonValue& value, const ptl::string& key, int defaultValue)
	{
		if (value.HasMember(key.c_str())) {
			auto& subValue = value.FindMember(key.c_str())->value;
			if (!subValue.IsNumber()) {
				std::cerr << "ERROR element with key: '" << key << "' should be a number!" << std::endl;
				return 0;
			}
			return subValue.GetInt();
		} else {
			return defaultValue;
		}
	}

	bool ParticleParser::JsonGetBoolOrDefault(RapidJsonValue& value, const ptl::string& key, bool defaultValue)
	{
		if (value.HasMember(key.c_str())) {
			auto& subValue = value.FindMember(key.c_str())->value;
			if (!subValue.IsBool()) {
				std::cerr << "ERROR element with key: '" << key << "' should be a boolean!" << std::endl;
				return 0.0f;
			}
			return subValue.GetBool();
		} else {
			return defaultValue;
		}
	}
}
