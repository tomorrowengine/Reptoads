












#ifndef TYPEUTIL_H_879959971810
#define TYPEUTIL_H_879959971810

namespace eventpp {

template <typename F, template <typename> class T>
struct TransformArguments;

template <template <typename> class T, typename RT, typename ...Args>
struct TransformArguments <RT (Args...), T>
{
	using Type = RT (typename T<Args>::type...);
};

template <typename F, typename Replacement>
struct ReplaceReturnType;

template <typename Replacement, typename RT, typename ...Args>
struct ReplaceReturnType <RT (Args...), Replacement>
{
	using Type = Replacement (Args...);
};


} 

#endif

