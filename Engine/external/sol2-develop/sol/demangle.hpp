






















#ifndef SOL_DEMANGLE_HPP
#define SOL_DEMANGLE_HPP

#include <string>
#include <array>
#include <cctype>
#if defined(__GNUC__) && defined(__MINGW32__) && (__GNUC__ < 6)
extern "C" {
#include <ctype.h>
}
#endif 
#include <locale>

namespace sol {
namespace detail {
#if defined(__GNUC__) || defined(__clang__)
	template <typename T, class seperator_mark = int>
	inline std::string ctti_get_type_name() {
		
		using namespace std;
		static const std::array<std::string, 2> removals = {{"{anonymous}", "(anonymous namespace)"}};
		std::string name = __PRETTY_FUNCTION__;
		std::size_t start = name.find_first_of('[');
		start = name.find_first_of('=', start);
		std::size_t end = name.find_last_of(']');
		if (end == std::string::npos)
			end = name.size();
		if (start == std::string::npos)
			start = 0;
		if (start < name.size() - 1)
			start += 1;
		name = name.substr(start, end - start);
		start = name.rfind("seperator_mark");
		if (start != std::string::npos) {
			name.erase(start - 2, name.length());
		}
		while (!name.empty() && isblank(name.front()))
			name.erase(name.begin());
		while (!name.empty() && isblank(name.back()))
			name.pop_back();

		for (std::size_t r = 0; r < removals.size(); ++r) {
			auto found = name.find(removals[r]);
			while (found != std::string::npos) {
				name.erase(found, removals[r].size());
				found = name.find(removals[r]);
			}
		}

		return name;
	}
#elif defined(_MSC_VER)
	template <typename T>
	inline std::string ctti_get_type_name() {
		static const std::array<std::string, 7> removals = {{"public:", "private:", "protected:", "struct ", "class ", "`anonymous-namespace'", "`anonymous namespace'"}};
		std::string name = __FUNCSIG__;
		std::size_t start = name.find("get_type_name");
		if (start == std::string::npos)
			start = 0;
		else
			start += 13;
		if (start < name.size() - 1)
			start += 1;
		std::size_t end = name.find_last_of('>');
		if (end == std::string::npos)
			end = name.size();
		name = name.substr(start, end - start);
		if (name.find("struct", 0) == 0)
			name.replace(0, 6, "", 0);
		if (name.find("class", 0) == 0)
			name.replace(0, 5, "", 0);
		while (!name.empty() && isblank(name.front()))
			name.erase(name.begin());
		while (!name.empty() && isblank(name.back()))
			name.pop_back();

		for (std::size_t r = 0; r < removals.size(); ++r) {
			auto found = name.find(removals[r]);
			while (found != std::string::npos) {
				name.erase(found, removals[r].size());
				found = name.find(removals[r]);
			}
		}

		return name;
	}
#else
#error Compiler not supported for demangling
#endif 

	template <typename T>
	inline std::string demangle_once() {
		std::string realname = ctti_get_type_name<T>();
		return realname;
	}

	template <typename T>
	inline std::string short_demangle_once() {
		std::string realname = ctti_get_type_name<T>();
		
		static const std::array<std::string, 10> ops = {{"operator<", "operator<<", "operator<<=", "operator<=", "operator>", "operator>>", "operator>>=", "operator>=", "operator->", "operator->*"}};
		int level = 0;
		std::ptrdiff_t idx = 0;
		for (idx = static_cast<std::ptrdiff_t>(realname.empty() ? 0 : realname.size() - 1); idx > 0; --idx) {
			if (level == 0 && realname[idx] == ':') {
				break;
			}
			bool isleft = realname[idx] == '<';
			bool isright = realname[idx] == '>';
			if (!isleft && !isright)
				continue;
			bool earlybreak = false;
			for (const auto& op : ops) {
				std::size_t nisop = realname.rfind(op, idx);
				if (nisop == std::string::npos)
					continue;
				std::size_t nisopidx = idx - op.size() + 1;
				if (nisop == nisopidx) {
					idx = static_cast<std::ptrdiff_t>(nisopidx);
					earlybreak = true;
				}
				break;
			}
			if (earlybreak) {
				continue;
			}
			level += isleft ? -1 : 1;
		}
		if (idx > 0) {
			realname.erase(0, realname.length() < static_cast<std::size_t>(idx) ? realname.length() : idx + 1);
		}
		return realname;
	}

	template <typename T>
	inline const std::string& demangle() {
		static const std::string d = demangle_once<T>();
		return d;
	}

	template <typename T>
	inline const std::string& short_demangle() {
		static const std::string d = short_demangle_once<T>();
		return d;
	}
}
} 

#endif 
