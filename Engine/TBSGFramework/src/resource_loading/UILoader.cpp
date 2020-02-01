#include "resource_loading\UILoader.h"

#include "ui/UIImage.h"
#include "ui/UIText.h"
#include "gameplay/scripting/LuaContextHelper.h"
#include "core/Config.h"
#include "core/StrHash.h"

namespace ui {

	UIStacktructure* UILoader::Load(const char * a_UIFile)
	{
		rapidjson::GenericDocument< rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator < rapidjson::CrtAllocator > > uiDoc;
		char readBuffer[65536];

		FILE* fp;
		errno_t err = fopen_s(&fp, a_UIFile, "r");
		if (err) {
			return nullptr;
		}

		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		uiDoc.ParseStream(is);
		ASSERT_MSG(!uiDoc.HasParseError(), "Synatx Error");
		ASSERT_MSG(uiDoc.HasMember("name"), "Could not find name in json");
		ASSERT_MSG(uiDoc.HasMember("elements"), "Could not find name in json");

		fclose(fp);


		ptl::string name = uiDoc["name"].GetString();

		ptl::vector<UIElementStructure*> elements;

		if (uiDoc["elements"].IsArray()) {
			elements = ParseElementsFromArray(uiDoc["elements"].GetArray());
		}
		return new UIStacktructure(name, elements);
	}

	UIStacktructure* UILoader::Load(const ptl::string & a_UIFile)
	{
		return Load(a_UIFile.c_str());
	}

	ptl::vector<UIElementStructure*> UILoader::ParseElementsFromArray(JsonArray a_Array)
	{
		ptl::vector<UIElementStructure*> elements;

		for (rapidjson::Value::ValueIterator itr = a_Array.Begin(); itr != a_Array.End(); ++itr) {

			ptl::string name = (*itr)["name"].GetString();
			ptl::string tag{};
			
			if ((*itr).HasMember("tag") && !(*itr)["tag"].IsNull())
			{
				tag = (*itr)["tag"].GetString();
			} 

			JsonObject positionJson = (*itr)["position"].GetObject();
			glm::vec3 position{ positionJson["x"].GetFloat(), positionJson["y"].GetFloat(), 0.0f };

			if (positionJson.HasMember("depth") && !positionJson["depth"].IsNull())
			{
				position.z = positionJson["depth"].GetFloat();
			}

			JsonObject scaleJson = (*itr)["size"].GetObject();
			glm::vec2 scale{ scaleJson["width"].GetFloat(), scaleJson["height"].GetFloat() };

			float rotation = (*itr)["rotation"].GetFloat();

			ptl::string type = (*itr)["type"].GetString();

			if (type == "UIElement") {
				elements.push_back(new UIElementStructure(name, tag, ptl::vector<UIEventStructure>{}, position, scale, rotation, ParseElementsFromArray((*itr)["children"].GetArray())));
			}
			else if (type == "UIImage") {
				JsonObject typableData = (*itr)["typable_data"].GetObject();

				ptl::string imagePath = typableData["image"].GetString();
				bool interactable = typableData["interactable"].GetBool();
				ptl::vector<UIEventStructure> events{};

				if (typableData.HasMember("events"))
				{
					auto eventsArray = typableData["events"].GetArray();
					for (auto e = eventsArray.Begin(); e != eventsArray.End(); e++)
					{
						events.push_back(UIEventStructure{
							(*e)["type"].GetString(),
							(*e)["action"].GetString(),
							(*e)["data"].IsNull() ? "" : (*e)["data"].GetString()
							});
					}
				}


				elements.push_back(new UIImageStructure(name, tag, events, position, scale, rotation, ParseElementsFromArray((*itr)["children"].GetArray()), imagePath, interactable));
			}
			else if (type == "UIText") {
				JsonObject typableData = (*itr)["typable_data"].GetObject();

				ptl::string text = typableData["text"].GetString();
				ptl::string fontPath = typableData["font_path"].GetString();

				int fontSize = typableData["font_size"].GetInt();
				int horizontalAlignment = typableData["horizontal_alignment"].GetInt();
				int verticalAlignment = typableData["vertical_alignment"].GetInt();

				bool inputEnabled = false;
				bool obsecureText = false;
				ptl::string validation = "";
				ptl::string interactableTag = "";

				if (typableData.HasMember("input") && !typableData["input"].IsNull())
				{
					inputEnabled = true;

					JsonObject inputData = typableData["input"].GetObject();
					if (inputData.HasMember("validation") && !inputData["validation"].IsNull())
					{
						validation = inputData["validation"].GetString();
					}

					if (inputData.HasMember("obscure_text"))
					{
						obsecureText = inputData["obscure_text"].GetBool();
					}

					if (inputData.HasMember("interactable_tag") && !inputData["interactable_tag"].IsNull())
					{
						interactableTag = inputData["interactable_tag"].GetString();
					}
				}

				JsonObject colorJson = typableData["color"].GetObject();
				glm::vec4 color{ 
					colorJson["r"].IsNull() ? 0.0f : colorJson["r"].GetFloat(),
					colorJson["g"].IsNull() ? 0.0f : colorJson["g"].GetFloat(),
					colorJson["b"].IsNull() ? 0.0f : colorJson["b"].GetFloat(), 
					colorJson["a"].IsNull() ? 1.0f : colorJson["a"].GetFloat()
				};

				elements.push_back(new UITextStructure(name, tag, ptl::vector<UIEventStructure>{}, position, scale, rotation, ParseElementsFromArray((*itr)["children"].GetArray()), fontSize, text, fontPath, horizontalAlignment, verticalAlignment, color, inputEnabled, validation, obsecureText, interactableTag));
			}
		}

		return elements;
	}
}
