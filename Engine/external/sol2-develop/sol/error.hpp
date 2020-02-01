






















#ifndef SOL_ERROR_HPP
#define SOL_ERROR_HPP

#include <stdexcept>
#include <string>

namespace sol {
	namespace detail {
		struct direct_error_tag {};
		const auto direct_error = direct_error_tag{};
	} 

	class error : public std::runtime_error {
	private:
		
		std::string w;

	public:
		error(const std::string& str)
		: error(detail::direct_error, "lua: error: " + str) {
		}
		error(std::string&& str)
		: error(detail::direct_error, "lua: error: " + std::move(str)) {
		}
		error(detail::direct_error_tag, const std::string& str)
		: std::runtime_error(""), w(str) {
		}
		error(detail::direct_error_tag, std::string&& str)
		: std::runtime_error(""), w(std::move(str)) {
		}

		error(const error& e) = default;
		error(error&& e) = default;
		error& operator=(const error& e) = default;
		error& operator=(error&& e) = default;

		virtual const char* what() const noexcept override {
			return w.c_str();
		}
	};

} 

#endif 
