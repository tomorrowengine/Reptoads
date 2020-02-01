












#ifndef MIXINFILTER_H_713231680355
#define MIXINFILTER_H_713231680355

#include "../callbacklist.h"
#include "../typeutil.h"

#include <functional>
#include <type_traits>

namespace eventpp {

template <typename Base>
class MixinFilter : public Base
{
private:
	using super = Base;

	using BoolReferencePrototype = typename ReplaceReturnType<
		typename TransformArguments<
			typename super::Prototype,
			std::add_lvalue_reference
		>::Type,
		bool
	>::Type;

	using Filter = std::function<BoolReferencePrototype>;
	using FilterList = CallbackList<BoolReferencePrototype>;

public:
	using FilterHandle = typename FilterList::Handle;

public:
	FilterHandle appendFilter(const Filter & filter)
	{
		return filterList.append(filter);
	}

	bool removeFilter(const FilterHandle & filterHandle)
	{
		return filterList.remove(filterHandle);
	}

	template <typename ...Args>
	bool mixinBeforeDispatch(Args && ...args) const {
		if(! filterList.empty()) {
			if(
				! filterList.forEachIf([&args...](typename FilterList::Callback & callback) {
				return callback(args...);
			})
				) {
				return false;
			}
		}

		return true;
	}

private:
	FilterList filterList;
};


} 


#endif

