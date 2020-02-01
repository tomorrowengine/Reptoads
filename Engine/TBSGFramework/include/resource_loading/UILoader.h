#pragma once
#include "memory/allocator.h"
#include "memory/String.h"
#include "ui/UIStack.h"
#include "ui/UIElement.h"

#include <rapidjson/allocators.h>
#include <rapidjson/encodings.h>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/rapidjson.h>
#include "core/Assertion.h"
#include "Payloads.h"


namespace ui
{
	class UILoader
	{

		typedef rapidjson::GenericValue< rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator < rapidjson::CrtAllocator > > JsonValue;
		typedef rapidjson::GenericObject<true, JsonValue> JsonConstObject;
		typedef rapidjson::GenericObject<false, JsonValue> JsonObject;
		typedef rapidjson::GenericArray<false, JsonValue> JsonArray;
	public:

		ui::UIStacktructure* Load(const char* a_UIFile);
		ui::UIStacktructure* Load(const ptl::string& a_UIFile);

	private:

		ptl::vector<UIElementStructure*> ParseElementsFromArray(JsonArray a_Array);

	};
}
