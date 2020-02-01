






















#ifndef SOL_WRAPPER_HPP
#define SOL_WRAPPER_HPP

#include "types.hpp"

namespace sol {

	namespace detail {
		template <typename T>
		using array_return_type = std::conditional_t<std::is_array<T>::value, std::add_lvalue_reference_t<T>, T>;
	}

	template <typename F, typename = void>
	struct wrapper {
		typedef lua_bind_traits<meta::unqualified_t<F>> traits_type;
		typedef typename traits_type::args_list args_list;
		typedef typename traits_type::args_list free_args_list;
		typedef typename traits_type::returns_list returns_list;

		template <typename... Args>
		static decltype(auto) call(F& f, Args&&... args) {
			return f(std::forward<Args>(args)...);
		}

		struct caller {
			template <typename... Args>
			decltype(auto) operator()(F& fx, Args&&... args) const {
				return call(fx, std::forward<Args>(args)...);
			}
		};
	};

	template <typename F>
	struct wrapper<F, std::enable_if_t<std::is_function<std::remove_pointer_t<meta::unqualified_t<F>>>::value>> {
		typedef lua_bind_traits<std::remove_pointer_t<meta::unqualified_t<F>>> traits_type;
		typedef typename traits_type::args_list args_list;
		typedef typename traits_type::args_list free_args_list;
		typedef typename traits_type::returns_list returns_list;

		template <F fx, typename... Args>
		static decltype(auto) invoke(Args&&... args) {
			return fx(std::forward<Args>(args)...);
		}

		template <typename... Args>
		static decltype(auto) call(F& fx, Args&&... args) {
			return fx(std::forward<Args>(args)...);
		}

		struct caller {
			template <typename... Args>
			decltype(auto) operator()(F& fx, Args&&... args) const {
				return call(fx, std::forward<Args>(args)...);
			}
		};

		template <F fx>
		struct invoker {
			template <typename... Args>
			decltype(auto) operator()(Args&&... args) const {
				return invoke<fx>(std::forward<Args>(args)...);
			}
		};
	};

	template <typename F>
	struct wrapper<F, std::enable_if_t<std::is_member_object_pointer<meta::unqualified_t<F>>::value>> {
		typedef lua_bind_traits<meta::unqualified_t<F>> traits_type;
		typedef typename traits_type::object_type object_type;
		typedef typename traits_type::return_type return_type;
		typedef typename traits_type::args_list args_list;
		typedef types<object_type&, return_type> free_args_list;
		typedef typename traits_type::returns_list returns_list;

		template <F fx>
		static auto call(object_type& mem) -> detail::array_return_type<decltype(mem.*fx)> {
			return mem.*fx;
		}

		template <F fx, typename Arg, typename... Args>
		static decltype(auto) invoke(object_type& mem, Arg&& arg, Args&&...) {
			return mem.*fx = std::forward<Arg>(arg);
		}

		template <typename Fx>
		static auto call(Fx&& fx, object_type& mem) -> detail::array_return_type<decltype(mem.*fx)> {
			return mem.*fx;
		}

		template <typename Fx, typename Arg, typename... Args>
		static void call(Fx&& fx, object_type& mem, Arg&& arg, Args&&...) {
			(mem.*fx) = std::forward<Arg>(arg);
		}

		struct caller {
			template <typename Fx, typename... Args>
			decltype(auto) operator()(Fx&& fx, object_type& mem, Args&&... args) const {
				return call(std::forward<Fx>(fx), mem, std::forward<Args>(args)...);
			}
		};

		template <F fx>
		struct invoker {
			template <typename... Args>
			decltype(auto) operator()(Args&&... args) const {
				return invoke<fx>(std::forward<Args>(args)...);
			}
		};
	};

	template <typename F, typename R, typename O, typename... FArgs>
	struct member_function_wrapper {
		typedef O object_type;
		typedef lua_bind_traits<F> traits_type;
		typedef typename traits_type::args_list args_list;
		typedef types<object_type&, FArgs...> free_args_list;
		typedef meta::tuple_types<R> returns_list;

		template <F fx, typename... Args>
		static R invoke(O& mem, Args&&... args) {
			return (mem.*fx)(std::forward<Args>(args)...);
		}

		template <typename Fx, typename... Args>
		static R call(Fx&& fx, O& mem, Args&&... args) {
			return (mem.*fx)(std::forward<Args>(args)...);
		}

		struct caller {
			template <typename Fx, typename... Args>
			decltype(auto) operator()(Fx&& fx, O& mem, Args&&... args) const {
				return call(std::forward<Fx>(fx), mem, std::forward<Args>(args)...);
			}
		};

		template <F fx>
		struct invoker {
			template <typename... Args>
			decltype(auto) operator()(O& mem, Args&&... args) const {
				return invoke<fx>(mem, std::forward<Args>(args)...);
			}
		};
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args...)> : public member_function_wrapper<R (O::*)(Args...), R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args...) const> : public member_function_wrapper<R (O::*)(Args...) const, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args...) const volatile> : public member_function_wrapper<R (O::*)(Args...) const volatile, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args...)&> : public member_function_wrapper<R (O::*)(Args...)&, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args...) const&> : public member_function_wrapper<R (O::*)(Args...) const&, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args...) const volatile&> : public member_function_wrapper<R (O::*)(Args...) const volatile&, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args..., ...)&> : public member_function_wrapper<R (O::*)(Args..., ...)&, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args..., ...) const&> : public member_function_wrapper<R (O::*)(Args..., ...) const&, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args..., ...) const volatile&> : public member_function_wrapper<R (O::*)(Args..., ...) const volatile&, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args...) &&> : public member_function_wrapper<R (O::*)(Args...)&, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args...) const&&> : public member_function_wrapper<R (O::*)(Args...) const&, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args...) const volatile&&> : public member_function_wrapper<R (O::*)(Args...) const volatile&, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args..., ...) &&> : public member_function_wrapper<R (O::*)(Args..., ...)&, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args..., ...) const&&> : public member_function_wrapper<R (O::*)(Args..., ...) const&, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args..., ...) const volatile&&> : public member_function_wrapper<R (O::*)(Args..., ...) const volatile&, R, O, Args...> {
	};

#if defined(SOL_NOEXCEPT_FUNCTION_TYPE) && SOL_NOEXCEPT_FUNCTION_TYPE
	

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args...) noexcept> : public member_function_wrapper<R (O::*)(Args...) noexcept, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args...) const noexcept> : public member_function_wrapper<R (O::*)(Args...) const noexcept, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args...) const volatile noexcept> : public member_function_wrapper<R (O::*)(Args...) const volatile noexcept, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args...) & noexcept> : public member_function_wrapper<R (O::*)(Args...) & noexcept, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args...) const& noexcept> : public member_function_wrapper<R (O::*)(Args...) const& noexcept, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args...) const volatile& noexcept> : public member_function_wrapper<R (O::*)(Args...) const volatile& noexcept, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args..., ...) & noexcept> : public member_function_wrapper<R (O::*)(Args..., ...) & noexcept, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args..., ...) const& noexcept> : public member_function_wrapper<R (O::*)(Args..., ...) const& noexcept, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args..., ...) const volatile& noexcept> : public member_function_wrapper<R (O::*)(Args..., ...) const volatile& noexcept, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args...) && noexcept> : public member_function_wrapper<R (O::*)(Args...) & noexcept, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args...) const&& noexcept> : public member_function_wrapper<R (O::*)(Args...) const& noexcept, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args...) const volatile&& noexcept> : public member_function_wrapper<R (O::*)(Args...) const volatile& noexcept, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args..., ...) && noexcept> : public member_function_wrapper<R (O::*)(Args..., ...) & noexcept, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args..., ...) const&& noexcept> : public member_function_wrapper<R (O::*)(Args..., ...) const& noexcept, R, O, Args...> {
	};

	template <typename R, typename O, typename... Args>
	struct wrapper<R (O::*)(Args..., ...) const volatile&& noexcept> : public member_function_wrapper<R (O::*)(Args..., ...) const volatile& noexcept, R, O, Args...> {
	};

#endif 

} 

#endif 
