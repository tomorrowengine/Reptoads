#pragma once

#ifndef BREAK_DEBUGGER
	#if defined(PLATFORM_WINDOWS)
		#include <intrin.h>
		#define BREAK_DEBUGGER() __debugbreak()
		#define BREAK_DEBUGGER()
	#endif
#endif
#ifndef BREAK_DEBUGGER_WHEN
	#if defined(PLATFORM_WINDOWS)
		#define BREAK_DEBUGGER_WHEN(condition) if(condition){ BREAK_DEBUGGER(); }
	#else
		#define BREAK_DEBUGGER_WHEN(condition)
	#endif
#endif

#include <iostream>
#include "memory/Containers.h"
#include "memory/String.h"




inline void SmallPart_PrintErrorOnLine(ptl::string message = "", const char* file = nullptr, int line = -1, const char* expressionName = nullptr)
{
	using namespace ptl::string_literals;
	ptl::string s = "Assertion failed!\nAt: "s + file + " : " + ptl::to_string(line) + " with the expression: \n'" + expressionName;
	if (!message.empty())
		s += "'\n message: '"s + message + "'";

	std::cerr << s << '\n';
}


#ifndef BREAK_DEBUGGER
#define BREAK_DEBUGGER()
#endif
#ifndef ASSERT
#define ASSERT(expression) if(!(expression)) { SmallPart_PrintErrorOnLine("", __FILE__, __LINE__, #expression); BREAK_DEBUGGER(); }
#endif
#ifndef ASSERT_MSG
#define ASSERT_MSG(expression, message) if(!(expression)) { SmallPart_PrintErrorOnLine(message, __FILE__, __LINE__, #expression); BREAK_DEBUGGER(); }
#endif
