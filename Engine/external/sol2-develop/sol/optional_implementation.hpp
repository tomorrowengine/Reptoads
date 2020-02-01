






























#ifndef SOL_OPTIONAL_IMPLEMENTATION_HPP
#define SOL_OPTIONAL_IMPLEMENTATION_HPP

#include "in_place.hpp"

#include <utility>
#include <type_traits>
#include <initializer_list>
#include <cassert>
#include <functional>
#include <string>
#include <stdexcept>
#if defined(SOL_NO_EXCEPTIONS) && SOL_NO_EXCEPTIONS
#include <cstdlib>
#endif 

#define TR2_OPTIONAL_REQUIRES(...) typename ::std::enable_if<__VA_ARGS__::value, bool>::type = false

#if defined __GNUC__ 
#if (__GNUC__ >= 5)
#define TR2_OPTIONAL_GCC_5_0_AND_HIGHER___
#define TR2_OPTIONAL_GCC_4_8_AND_HIGHER___
#elif (__GNUC__ == 4) && (__GNUC_MINOR__ >= 8)
#define TR2_OPTIONAL_GCC_4_8_AND_HIGHER___
#elif (__GNUC__ > 4)
#define TR2_OPTIONAL_GCC_4_8_AND_HIGHER___
#endif
#
#if (__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)
#define TR2_OPTIONAL_GCC_4_7_AND_HIGHER___
#elif (__GNUC__ > 4)
#define TR2_OPTIONAL_GCC_4_7_AND_HIGHER___
#endif
#
#if (__GNUC__ == 4) && (__GNUC_MINOR__ == 8) && (__GNUC_PATCHLEVEL__ >= 1)
#define TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#elif (__GNUC__ == 4) && (__GNUC_MINOR__ >= 9)
#define TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#elif (__GNUC__ > 4)
#define TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#endif
#endif
#
#if defined __clang_major__
#if (__clang_major__ == 3 && __clang_minor__ >= 5)
#define TR2_OPTIONAL_CLANG_3_5_AND_HIGHTER_
#elif (__clang_major__ > 3)
#define TR2_OPTIONAL_CLANG_3_5_AND_HIGHTER_
#endif
#if defined TR2_OPTIONAL_CLANG_3_5_AND_HIGHTER_
#define TR2_OPTIONAL_CLANG_3_4_2_AND_HIGHER_
#elif (__clang_major__ == 3 && __clang_minor__ == 4 && __clang_patchlevel__ >= 2)
#define TR2_OPTIONAL_CLANG_3_4_2_AND_HIGHER_
#endif
#endif
#
#if defined _MSC_VER
#if (_MSC_VER >= 1900)
#define TR2_OPTIONAL_MSVC_2015_AND_HIGHER___
#endif
#endif

#if defined __clang__
#if (__clang_major__ > 2) || (__clang_major__ == 2) && (__clang_minor__ >= 9)
#define OPTIONAL_HAS_THIS_RVALUE_REFS 1
#else
#define OPTIONAL_HAS_THIS_RVALUE_REFS 0
#endif
#elif defined TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#define OPTIONAL_HAS_THIS_RVALUE_REFS 1
#elif defined TR2_OPTIONAL_MSVC_2015_AND_HIGHER___
#define OPTIONAL_HAS_THIS_RVALUE_REFS 1
#else
#define OPTIONAL_HAS_THIS_RVALUE_REFS 0
#endif

#if defined TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#define OPTIONAL_HAS_CONSTEXPR_INIT_LIST 1
#define OPTIONAL_CONSTEXPR_INIT_LIST constexpr
#else
#define OPTIONAL_HAS_CONSTEXPR_INIT_LIST 0
#define OPTIONAL_CONSTEXPR_INIT_LIST
#endif

#if defined(TR2_OPTIONAL_MSVC_2015_AND_HIGHER___) || (defined TR2_OPTIONAL_CLANG_3_5_AND_HIGHTER_ && (defined __cplusplus) && (__cplusplus != 201103L))
#define OPTIONAL_HAS_MOVE_ACCESSORS 1
#else
#define OPTIONAL_HAS_MOVE_ACCESSORS 0
#endif

#
#if defined(TR2_OPTIONAL_MSVC_2015_AND_HIGHER___) || ((defined __cplusplus) && (__cplusplus == 201103L))
#define OPTIONAL_MUTABLE_CONSTEXPR
#else
#define OPTIONAL_MUTABLE_CONSTEXPR constexpr
#endif

#if defined TR2_OPTIONAL_MSVC_2015_AND_HIGHER___
#pragma warning(push)
#pragma warning(disable : 4814)
#endif

namespace sol {

	
#if defined TR2_OPTIONAL_GCC_4_8_AND_HIGHER___
	
#elif defined TR2_OPTIONAL_CLANG_3_4_2_AND_HIGHER_
	
#elif defined TR2_OPTIONAL_MSVC_2015_AND_HIGHER___
	
#elif defined TR2_OPTIONAL_DISABLE_EMULATION_OF_TYPE_TRAITS
	
#else
	template <typename T>
	using is_trivially_destructible = ::std::has_trivial_destructor<T>;
#endif
	

#if (defined TR2_OPTIONAL_GCC_4_7_AND_HIGHER___)
	
#elif defined TR2_OPTIONAL_CLANG_3_4_2_AND_HIGHER_
	
#elif defined TR2_OPTIONAL_MSVC_2015_AND_HIGHER___
	
#elif defined TR2_OPTIONAL_DISABLE_EMULATION_OF_TYPE_TRAITS
	
#else

	
	template <class T>
	struct is_nothrow_move_constructible {
		static constexprbool value = ::std::is_nothrow_constructible<T, T&&>::value;
	};

	template <class T, class U>
	struct is_assignable {
		template <class X, class Y>
		static constexprbool has_assign(...) {
			return false;
		}

		template <class X, class Y, size_t S = sizeof((::std::declval<X>() = ::std::declval<Y>(), true))>
		
		static constexprbool has_assign(bool) {
			return true;
		}

		static constexprbool value = has_assign<T, U>(true);
	};

	template <class T>
	struct is_nothrow_move_assignable {
		template <class X, bool has_any_move_assign>
		struct has_nothrow_move_assign {
			static constexprbool value = false;
		};

		template <class X>
		struct has_nothrow_move_assign<X, true> {
			static constexprbool value = noexcept(::std::declval<X&>() = ::std::declval<X&&>());
		};

		static constexprbool value = has_nothrow_move_assign<T, is_assignable<T&, T&&>::value>::value;
	};
	

#endif

	
	template <class T>
	class optional;

	
	template <class T>
	class optional<T&>;

	
	template <class T>
	inline constexpr T&& constexpr_forward(typename ::std::remove_reference<T>::type& t) noexcept {
		return static_cast<T&&>(t);
	}

	template <class T>
	inline constexpr T&& constexpr_forward(typename ::std::remove_reference<T>::type&& t) noexcept {
		static_assert(!::std::is_lvalue_reference<T>::value, "!!");
		return static_cast<T&&>(t);
	}

	template <class T>
	inline constexpr typename ::std::remove_reference<T>::type&& constexpr_move(T&& t) noexcept {
		return static_cast<typename ::std::remove_reference<T>::type&&>(t);
	}

#if defined NDEBUG
#define TR2_OPTIONAL_ASSERTED_EXPRESSION(CHECK, EXPR) (EXPR)
#else
#define TR2_OPTIONAL_ASSERTED_EXPRESSION(CHECK, EXPR) ((CHECK) ? (EXPR) : ([] { assert(!#CHECK); }(), (EXPR)))
#endif

	namespace detail_ {

		
		template <typename T>
		struct has_overloaded_addressof {
			template <class X>
			static constexpr bool has_overload(...) {
				return false;
			}

			template <class X, size_t S = sizeof(::std::declval<X&>().operator&())>
			static constexpr bool has_overload(bool) {
				return true;
			}

			static constexpr bool value = has_overload<T>(true);
		};

		template <typename T, TR2_OPTIONAL_REQUIRES(!has_overloaded_addressof<T>)>
		constexpr T* static_addressof(T& ref) {
			return &ref;
		}

		template <typename T, TR2_OPTIONAL_REQUIRES(has_overloaded_addressof<T>)>
		T* static_addressof(T& ref) {
			return ::std::addressof(ref);
		}

		
		template <class U>
		constexpr U convert(U v) {
			return v;
		}

	} 

	constexpr struct trivial_init_t {
	} trivial_init{};

	
	struct nullopt_t {
		struct init {};
		constexpr explicit nullopt_t(init) {
		}
	};
	constexpr nullopt_t nullopt{nullopt_t::init()};

	
	class bad_optional_access : public ::std::logic_error {
	public:
		explicit bad_optional_access(const ::std::string& what_arg)
		: ::std::logic_error{what_arg} {
		}
		explicit bad_optional_access(const char* what_arg)
		: ::std::logic_error{what_arg} {
		}
	};

	template <class T>
	struct alignas(T) optional_base {
		char storage_[sizeof(T)];
		bool init_;

		constexpr optional_base() noexcept
		: storage_(), init_(false){};

		explicit optional_base(const T& v)
		: storage_(), init_(true) {
			new (&storage()) T(v);
		}

		explicit optional_base(T&& v)
		: storage_(), init_(true) {
			new (&storage()) T(constexpr_move(v));
		}

		template <class... Args>
		explicit optional_base(in_place_t, Args&&... args)
		: init_(true), storage_() {
			new (&storage()) T(constexpr_forward<Args>(args)...);
		}

		template <class U, class... Args, TR2_OPTIONAL_REQUIRES(::std::is_constructible<T, ::std::initializer_list<U>>)>
		explicit optional_base(in_place_t, ::std::initializer_list<U> il, Args&&... args)
		: init_(true), storage_() {
			new (&storage()) T(il, constexpr_forward<Args>(args)...);
		}
#if defined __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif
		T& storage() {
			return *reinterpret_cast<T*>(&storage_[0]);
		}

		constexpr const T& storage() const {
			return *reinterpret_cast<T const*>(&storage_[0]);
		}
#if defined __GNUC__
#pragma GCC diagnostic pop
#endif

		~optional_base() {
			if (init_) {
				storage().T::~T();
			}
		}
	};

#if defined __GNUC__ && !defined TR2_OPTIONAL_GCC_5_0_AND_HIGHER___
	
	template <typename T>
	using constexpr_optional_base = optional_base<T>;
#else
	template <class T>
	struct alignas(T) constexpr_optional_base {
		char storage_[sizeof(T)];
		bool init_;
		constexpr constexpr_optional_base() noexcept
		: storage_(), init_(false) {
		}

		explicit constexpr constexpr_optional_base(const T& v)
		: storage_(), init_(true) {
			new (&storage()) T(v);
		}

		explicit constexpr constexpr_optional_base(T&& v)
		: storage_(), init_(true) {
			new (&storage()) T(constexpr_move(v));
		}

		template <class... Args>
		explicit constexpr constexpr_optional_base(in_place_t, Args&&... args)
		: init_(true), storage_() {
			new (&storage()) T(constexpr_forward<Args>(args)...);
		}

		template <class U, class... Args, TR2_OPTIONAL_REQUIRES(::std::is_constructible<T, ::std::initializer_list<U>>)>
		OPTIONAL_CONSTEXPR_INIT_LIST explicit constexpr_optional_base(in_place_t, ::std::initializer_list<U> il, Args&&... args)
		: init_(true), storage_() {
			new (&storage()) T(il, constexpr_forward<Args>(args)...);
		}

#if defined __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif
		T& storage() {
			return (*reinterpret_cast<T*>(&storage_[0]));
		}

		constexpr const T& storage() const {
			return (*reinterpret_cast<T const*>(&storage_[0]));
		}
#if defined __GNUC__
#pragma GCC diagnostic pop
#endif

		~constexpr_optional_base() = default;
	};
#endif

	template <class T>
	using OptionalBase = typename ::std::conditional<
		::std::is_trivially_destructible<T>::value,
		constexpr_optional_base<typename ::std::remove_const<T>::type>,
		optional_base<typename ::std::remove_const<T>::type>>::type;

	template <class T>
	class optional : private OptionalBase<T> {
		static_assert(!::std::is_same<typename ::std::decay<T>::type, nullopt_t>::value, "bad T");
		static_assert(!::std::is_same<typename ::std::decay<T>::type, in_place_t>::value, "bad T");

		constexpr bool initialized() const noexcept {
			return OptionalBase<T>::init_;
		}
		typename ::std::remove_const<T>::type* dataptr() {
			return ::std::addressof(OptionalBase<T>::storage());
		}
		constexpr const T* dataptr() const {
			return detail_::static_addressof(OptionalBase<T>::storage());
		}

#if OPTIONAL_HAS_THIS_RVALUE_REFS == 1
		constexpr const T& contained_val() const& {
			return OptionalBase<T>::storage();
		}
#if OPTIONAL_HAS_MOVE_ACCESSORS == 1
		OPTIONAL_MUTABLE_CONSTEXPR T&& contained_val() && {
			return ::std::move(OptionalBase<T>::storage());
		}
		OPTIONAL_MUTABLE_CONSTEXPR T& contained_val() & {
			return OptionalBase<T>::storage();
		}
#else
		T& contained_val() & {
			return OptionalBase<T>::storage();
		}
		T&& contained_val() && {
			return ::std::move(OptionalBase<T>::storage());
		}
#endif
#else
		constexpr const T& contained_val() const {
			return OptionalBase<T>::storage();
		}
		T& contained_val() {
			return OptionalBase<T>::storage();
		}
#endif

		void clear() noexcept {
			if (initialized())
				dataptr()->T::~T();
			OptionalBase<T>::init_ = false;
		}

		template <class... Args>
		void initialize(Args&&... args) noexcept(noexcept(T(::std::forward<Args>(args)...))) {
			assert(!OptionalBase<T>::init_);
			::new (static_cast<void*>(dataptr())) T(::std::forward<Args>(args)...);
			OptionalBase<T>::init_ = true;
		}

		template <class U, class... Args>
		void initialize(::std::initializer_list<U> il, Args&&... args) noexcept(noexcept(T(il, ::std::forward<Args>(args)...))) {
			assert(!OptionalBase<T>::init_);
			::new (static_cast<void*>(dataptr())) T(il, ::std::forward<Args>(args)...);
			OptionalBase<T>::init_ = true;
		}

	public:
		typedef T value_type;

		
		constexpr optional() noexcept
		: OptionalBase<T>(){};
		constexpr optional(nullopt_t) noexcept
		: OptionalBase<T>(){};

		optional(const optional& rhs)
		: OptionalBase<T>() {
			if (rhs.initialized()) {
				::new (static_cast<void*>(dataptr())) T(*rhs);
				OptionalBase<T>::init_ = true;
			}
		}

		optional(const optional<T&>& rhs)
		: optional() {
			if (rhs) {
				::new (static_cast<void*>(dataptr())) T(*rhs);
				OptionalBase<T>::init_ = true;
			}
		}

		optional(optional&& rhs) noexcept(::std::is_nothrow_move_constructible<T>::value)
		: OptionalBase<T>() {
			if (rhs.initialized()) {
				::new (static_cast<void*>(dataptr())) T(::std::move(*rhs));
				OptionalBase<T>::init_ = true;
			}
		}

		constexpr optional(const T& v)
		: OptionalBase<T>(v) {
		}

		constexpr optional(T&& v)
		: OptionalBase<T>(constexpr_move(v)) {
		}

		template <class... Args>
		explicit constexpr optional(in_place_t, Args&&... args)
		: OptionalBase<T>(in_place, constexpr_forward<Args>(args)...) {
		}

		template <class U, class... Args, TR2_OPTIONAL_REQUIRES(::std::is_constructible<T, ::std::initializer_list<U>>)>
		OPTIONAL_CONSTEXPR_INIT_LIST explicit optional(in_place_t, ::std::initializer_list<U> il, Args&&... args)
		: OptionalBase<T>(in_place, il, constexpr_forward<Args>(args)...) {
		}

		
		~optional() = default;

		
		optional& operator=(nullopt_t) noexcept {
			clear();
			return *this;
		}

		optional& operator=(const optional& rhs) {
			if (initialized() == true && rhs.initialized() == false)
				clear();
			else if (initialized() == false && rhs.initialized() == true)
				initialize(*rhs);
			else if (initialized() == true && rhs.initialized() == true)
				contained_val() = *rhs;
			return *this;
		}

		optional& operator=(optional&& rhs) noexcept(::std::is_nothrow_move_assignable<T>::value&& ::std::is_nothrow_move_constructible<T>::value) {
			if (initialized() == true && rhs.initialized() == false)
				clear();
			else if (initialized() == false && rhs.initialized() == true)
				initialize(::std::move(*rhs));
			else if (initialized() == true && rhs.initialized() == true)
				contained_val() = ::std::move(*rhs);
			return *this;
		}

		template <class U>
		auto operator=(U&& v)
			-> typename ::std::enable_if<
				::std::is_same<typename ::std::decay<U>::type, T>::value,
				optional&>::type {
			if (initialized()) {
				contained_val() = ::std::forward<U>(v);
			}
			else {
				initialize(::std::forward<U>(v));
			}
			return *this;
		}

		template <class... Args>
		void emplace(Args&&... args) {
			clear();
			initialize(::std::forward<Args>(args)...);
		}

		template <class U, class... Args>
		void emplace(::std::initializer_list<U> il, Args&&... args) {
			clear();
			initialize<U, Args...>(il, ::std::forward<Args>(args)...);
		}

		
		void swap(optional<T>& rhs) noexcept(::std::is_nothrow_move_constructible<T>::value&& noexcept(swap(::std::declval<T&>(), ::std::declval<T&>()))) {
			if (initialized() == true && rhs.initialized() == false) {
				rhs.initialize(::std::move(**this));
				clear();
			}
			else if (initialized() == false && rhs.initialized() == true) {
				initialize(::std::move(*rhs));
				rhs.clear();
			}
			else if (initialized() == true && rhs.initialized() == true) {
				using ::std::swap;
				swap(**this, *rhs);
			}
		}

		

		explicit constexpr operator bool() const noexcept {
			return initialized();
		}

		constexpr T const* operator->() const {
			return TR2_OPTIONAL_ASSERTED_EXPRESSION(initialized(), dataptr());
		}

#if OPTIONAL_HAS_MOVE_ACCESSORS == 1

		OPTIONAL_MUTABLE_CONSTEXPR T* operator->() {
			assert(initialized());
			return dataptr();
		}

		constexpr T const& operator*() const& {
			return TR2_OPTIONAL_ASSERTED_EXPRESSION(initialized(), contained_val());
		}

		OPTIONAL_MUTABLE_CONSTEXPR T& operator*() & {
			assert(initialized());
			return contained_val();
		}

		OPTIONAL_MUTABLE_CONSTEXPR T&& operator*() && {
			assert(initialized());
			return constexpr_move(contained_val());
		}

		constexpr T const& value() const& {
			return initialized() ? contained_val()
#ifdef SOL_NO_EXCEPTIONS
							 
							 
							 : *static_cast<T*>(nullptr);
#else
							 : (throw bad_optional_access("bad optional access"), contained_val());
#endif
		}

		OPTIONAL_MUTABLE_CONSTEXPR T& value() & {
			return initialized() ? contained_val()
#ifdef SOL_NO_EXCEPTIONS
							 : *static_cast<T*>(nullptr);
#else
							 : (throw bad_optional_access("bad optional access"), contained_val());
#endif
		}

		OPTIONAL_MUTABLE_CONSTEXPR T&& value() && {
			return initialized() ? contained_val()
#ifdef SOL_NO_EXCEPTIONS
							 
							 
							 : std::move(*static_cast<T*>(nullptr));
#else
							 : (throw bad_optional_access("bad optional access"), contained_val());
#endif
		}

#else

		T* operator->() {
			assert(initialized());
			return dataptr();
		}

		constexpr T const& operator*() const {
			return TR2_OPTIONAL_ASSERTED_EXPRESSION(initialized(), contained_val());
		}

		T& operator*() {
			assert(initialized());
			return contained_val();
		}

		constexpr T const& value() const {
			return initialized() ? contained_val()
#ifdef SOL_NO_EXCEPTIONS
							 
							 
							 : *static_cast<T*>(nullptr);
#else
							 : (throw bad_optional_access("bad optional access"), contained_val());
#endif
		}

		T& value() {
			return initialized() ? contained_val()
#ifdef SOL_NO_EXCEPTIONS
							 
							 
							 : (std::abort(), *static_cast<T*>(nullptr));
#else
							 : (throw bad_optional_access("bad optional access"), contained_val());
#endif
		}

#endif

#if OPTIONAL_HAS_THIS_RVALUE_REFS == 1

		template <class V>
		constexpr T value_or(V&& v) const& {
			return *this ? **this : detail_::convert<T>(constexpr_forward<V>(v));
		}

#if OPTIONAL_HAS_MOVE_ACCESSORS == 1

		template <class V>
		OPTIONAL_MUTABLE_CONSTEXPR T value_or(V&& v) && {
			return *this ? constexpr_move(const_cast<optional<T>&>(*this).contained_val()) : detail_::convert<T>(constexpr_forward<V>(v));
		}

#else

		template <class V>
		T value_or(V&& v) && {
			return *this ? constexpr_move(const_cast<optional<T>&>(*this).contained_val()) : detail_::convert<T>(constexpr_forward<V>(v));
		}

#endif

#else

		template <class V>
		constexpr T value_or(V&& v) const {
			return *this ? **this : detail_::convert<T>(constexpr_forward<V>(v));
		}

#endif
	};

	template <class T>
	class optional<T&> {
		static_assert(!::std::is_same<T, nullopt_t>::value, "bad T");
		static_assert(!::std::is_same<T, in_place_t>::value, "bad T");
		T* ref;

	public:
		
		constexpr optional() noexcept
		: ref(nullptr) {
		}

		constexpr optional(nullopt_t) noexcept
		: ref(nullptr) {
		}

		constexpr optional(T& v) noexcept
		: ref(detail_::static_addressof(v)) {
		}

		optional(T&&) = delete;

		constexpr optional(const optional& rhs) noexcept
		: ref(rhs.ref) {
		}

		explicit constexpr optional(in_place_t, T& v) noexcept
		: ref(detail_::static_addressof(v)) {
		}

		explicit optional(in_place_t, T&&) = delete;

		~optional() = default;

		
		optional& operator=(nullopt_t) noexcept {
			ref = nullptr;
			return *this;
		}

		
		
		
		

		
		
		
		

		template <typename U>
		auto operator=(U&& rhs) noexcept
			-> typename ::std::enable_if<
				::std::is_same<typename ::std::decay<U>::type, optional<T&>>::value,
				optional&>::type {
			ref = rhs.ref;
			return *this;
		}

		template <typename U>
		auto operator=(U&& rhs) noexcept
			-> typename ::std::enable_if<
				!::std::is_same<typename ::std::decay<U>::type, optional<T&>>::value,
				optional&>::type = delete;

		void emplace(T& v) noexcept {
			ref = detail_::static_addressof(v);
		}

		void emplace(T&&) = delete;

		void swap(optional<T&>& rhs) noexcept {
			::std::swap(ref, rhs.ref);
		}

		
		constexpr T* operator->() const {
			return TR2_OPTIONAL_ASSERTED_EXPRESSION(ref, ref);
		}

		constexpr T& operator*() const {
			return TR2_OPTIONAL_ASSERTED_EXPRESSION(ref, *ref);
		}

		constexpr T& value() const {
#ifdef SOL_NO_EXCEPTIONS
			return *ref;
#else
			return ref ? *ref
					 : (throw bad_optional_access("bad optional access"), *ref);
#endif 
		}

		explicit constexpr operator bool() const noexcept {
			return ref != nullptr;
		}

		template <typename V>
		constexpr T& value_or(V&& v) const {
			return *this ? **this : detail_::convert<T&>(constexpr_forward<V>(v));
		}
	};

	template <class T>
	class optional<T&&> {
		static_assert(sizeof(T) == 0, "optional rvalue references disallowed");
	};

	
	template <class T>
	constexpr bool operator==(const optional<T>& x, const optional<T>& y) {
		return bool(x) != bool(y) ? false : bool(x) == false ? true : *x == *y;
	}

	template <class T>
	constexpr bool operator!=(const optional<T>& x, const optional<T>& y) {
		return !(x == y);
	}

	template <class T>
	constexpr bool operator<(const optional<T>& x, const optional<T>& y) {
		return (!y) ? false : (!x) ? true : *x < *y;
	}

	template <class T>
	constexpr bool operator>(const optional<T>& x, const optional<T>& y) {
		return (y < x);
	}

	template <class T>
	constexpr bool operator<=(const optional<T>& x, const optional<T>& y) {
		return !(y < x);
	}

	template <class T>
	constexpr bool operator>=(const optional<T>& x, const optional<T>& y) {
		return !(x < y);
	}

	
	template <class T>
	constexpr bool operator==(const optional<T>& x, nullopt_t) noexcept {
		return (!x);
	}

	template <class T>
	constexpr bool operator==(nullopt_t, const optional<T>& x) noexcept {
		return (!x);
	}

	template <class T>
	constexpr bool operator!=(const optional<T>& x, nullopt_t) noexcept {
		return bool(x);
	}

	template <class T>
	constexpr bool operator!=(nullopt_t, const optional<T>& x) noexcept {
		return bool(x);
	}

	template <class T>
	constexpr bool operator<(const optional<T>&, nullopt_t) noexcept {
		return false;
	}

	template <class T>
	constexpr bool operator<(nullopt_t, const optional<T>& x) noexcept {
		return bool(x);
	}

	template <class T>
	constexpr bool operator<=(const optional<T>& x, nullopt_t) noexcept {
		return (!x);
	}

	template <class T>
	constexpr bool operator<=(nullopt_t, const optional<T>&) noexcept {
		return true;
	}

	template <class T>
	constexpr bool operator>(const optional<T>& x, nullopt_t) noexcept {
		return bool(x);
	}

	template <class T>
	constexpr bool operator>(nullopt_t, const optional<T>&) noexcept {
		return false;
	}

	template <class T>
	constexpr bool operator>=(const optional<T>&, nullopt_t) noexcept {
		return true;
	}

	template <class T>
	constexpr bool operator>=(nullopt_t, const optional<T>& x) noexcept {
		return (!x);
	}

	
	template <class T>
	constexpr bool operator==(const optional<T>& x, const T& v) {
		return bool(x) ? *x == v : false;
	}

	template <class T>
	constexpr bool operator==(const T& v, const optional<T>& x) {
		return bool(x) ? v == *x : false;
	}

	template <class T>
	constexpr bool operator!=(const optional<T>& x, const T& v) {
		return bool(x) ? *x != v : true;
	}

	template <class T>
	constexpr bool operator!=(const T& v, const optional<T>& x) {
		return bool(x) ? v != *x : true;
	}

	template <class T>
	constexpr bool operator<(const optional<T>& x, const T& v) {
		return bool(x) ? *x < v : true;
	}

	template <class T>
	constexpr bool operator>(const T& v, const optional<T>& x) {
		return bool(x) ? v > *x : true;
	}

	template <class T>
	constexpr bool operator>(const optional<T>& x, const T& v) {
		return bool(x) ? *x > v : false;
	}

	template <class T>
	constexpr bool operator<(const T& v, const optional<T>& x) {
		return bool(x) ? v < *x : false;
	}

	template <class T>
	constexpr bool operator>=(const optional<T>& x, const T& v) {
		return bool(x) ? *x >= v : false;
	}

	template <class T>
	constexpr bool operator<=(const T& v, const optional<T>& x) {
		return bool(x) ? v <= *x : false;
	}

	template <class T>
	constexpr bool operator<=(const optional<T>& x, const T& v) {
		return bool(x) ? *x <= v : true;
	}

	template <class T>
	constexpr bool operator>=(const T& v, const optional<T>& x) {
		return bool(x) ? v >= *x : true;
	}

	
	template <class T>
	constexpr bool operator==(const optional<T&>& x, const T& v) {
		return bool(x) ? *x == v : false;
	}

	template <class T>
	constexpr bool operator==(const T& v, const optional<T&>& x) {
		return bool(x) ? v == *x : false;
	}

	template <class T>
	constexpr bool operator!=(const optional<T&>& x, const T& v) {
		return bool(x) ? *x != v : true;
	}

	template <class T>
	constexpr bool operator!=(const T& v, const optional<T&>& x) {
		return bool(x) ? v != *x : true;
	}

	template <class T>
	constexpr bool operator<(const optional<T&>& x, const T& v) {
		return bool(x) ? *x < v : true;
	}

	template <class T>
	constexpr bool operator>(const T& v, const optional<T&>& x) {
		return bool(x) ? v > *x : true;
	}

	template <class T>
	constexpr bool operator>(const optional<T&>& x, const T& v) {
		return bool(x) ? *x > v : false;
	}

	template <class T>
	constexpr bool operator<(const T& v, const optional<T&>& x) {
		return bool(x) ? v < *x : false;
	}

	template <class T>
	constexpr bool operator>=(const optional<T&>& x, const T& v) {
		return bool(x) ? *x >= v : false;
	}

	template <class T>
	constexpr bool operator<=(const T& v, const optional<T&>& x) {
		return bool(x) ? v <= *x : false;
	}

	template <class T>
	constexpr bool operator<=(const optional<T&>& x, const T& v) {
		return bool(x) ? *x <= v : true;
	}

	template <class T>
	constexpr bool operator>=(const T& v, const optional<T&>& x) {
		return bool(x) ? v >= *x : true;
	}

	
	template <class T>
	constexpr bool operator==(const optional<const T&>& x, const T& v) {
		return bool(x) ? *x == v : false;
	}

	template <class T>
	constexpr bool operator==(const T& v, const optional<const T&>& x) {
		return bool(x) ? v == *x : false;
	}

	template <class T>
	constexpr bool operator!=(const optional<const T&>& x, const T& v) {
		return bool(x) ? *x != v : true;
	}

	template <class T>
	constexpr bool operator!=(const T& v, const optional<const T&>& x) {
		return bool(x) ? v != *x : true;
	}

	template <class T>
	constexpr bool operator<(const optional<const T&>& x, const T& v) {
		return bool(x) ? *x < v : true;
	}

	template <class T>
	constexpr bool operator>(const T& v, const optional<const T&>& x) {
		return bool(x) ? v > *x : true;
	}

	template <class T>
	constexpr bool operator>(const optional<const T&>& x, const T& v) {
		return bool(x) ? *x > v : false;
	}

	template <class T>
	constexpr bool operator<(const T& v, const optional<const T&>& x) {
		return bool(x) ? v < *x : false;
	}

	template <class T>
	constexpr bool operator>=(const optional<const T&>& x, const T& v) {
		return bool(x) ? *x >= v : false;
	}

	template <class T>
	constexpr bool operator<=(const T& v, const optional<const T&>& x) {
		return bool(x) ? v <= *x : false;
	}

	template <class T>
	constexpr bool operator<=(const optional<const T&>& x, const T& v) {
		return bool(x) ? *x <= v : true;
	}

	template <class T>
	constexpr bool operator>=(const T& v, const optional<const T&>& x) {
		return bool(x) ? v >= *x : true;
	}

	
	template <class T>
	void swap(optional<T>& x, optional<T>& y) noexcept(noexcept(x.swap(y))) {
		x.swap(y);
	}

	template <class T>
	constexpr optional<typename ::std::decay<T>::type> make_optional(T&& v) {
		return optional<typename ::std::decay<T>::type>(constexpr_forward<T>(v));
	}

	template <class X>
	constexpr optional<X&> make_optional(::std::reference_wrapper<X> v) {
		return optional<X&>(v.get());
	}

} 

namespace std {
	template <typename T>
	struct hash<sol::optional<T>> {
		typedef typename hash<T>::result_type result_type;
		typedef sol::optional<T> argument_type;

		constexpr result_type operator()(argument_type const& arg) const {
			return arg ? ::std::hash<T>{}(*arg) : result_type{};
		}
	};

	template <typename T>
	struct hash<sol::optional<T&>> {
		typedef typename hash<T>::result_type result_type;
		typedef sol::optional<T&> argument_type;

		constexpr result_type operator()(argument_type const& arg) const {
			return arg ? ::std::hash<T>{}(*arg) : result_type{};
		}
	};
} 

#if defined TR2_OPTIONAL_MSVC_2015_AND_HIGHER___
#pragma warning(pop)
#endif

#undef TR2_OPTIONAL_REQUIRES
#undef TR2_OPTIONAL_ASSERTED_EXPRESSION

#endif 