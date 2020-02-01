












#ifndef EVENTUTIL_H_802615012766
#define EVENTUTIL_H_802615012766

#include "../eventpolicies.h"

#include <memory>

namespace eventpp {

template <typename DispatcherType>
bool removeListener(
		DispatcherType & dispatcher,
		const typename DispatcherType::Event & event,
		const typename DispatcherType::Callback & listener
	)
{
	bool found = false;
	dispatcher.forEachIf(
		event,
		[&found, &dispatcher, &event, &listener](
				const typename DispatcherType::Handle & handle,
				const typename DispatcherType::Callback & item
			) -> bool {
			if(item == listener) {
				found = true;
				dispatcher.removeListener(event, handle);
				return false;
			}
			else {
				return true;
			}
		}
	);
	
	return found;
}

template <typename CallbackListType>
bool removeListener(
		CallbackListType & callbackList,
		const typename CallbackListType::Callback & callback
	)
{
	bool found = false;
	callbackList.forEachIf(
		[&found, &callbackList, &callback](
				const typename CallbackListType::Handle & handle,
				const typename CallbackListType::Callback & item
			) -> bool {
			if(item == callback) {
				found = true;
				callbackList.remove(handle);
				return false;
			}
			else {
				return true;
			}
		}
	);
	
	return found;
}

template <typename DispatcherType>
bool hasListener(
	DispatcherType & dispatcher,
	const typename DispatcherType::Event & event,
	const typename DispatcherType::Callback & listener
)
{
	bool found = false;
	dispatcher.forEachIf(
		event,
		[&found, &listener](
			const typename DispatcherType::Handle & handle,
			const typename DispatcherType::Callback & item
			) -> bool {
			if(item == listener) {
				found = true;
				return false;
			}
			else {
				return true;
			}
		}
	);

	return found;
}

template <typename DispatcherType>
bool hasAnyListener(
	DispatcherType & dispatcher,
	const typename DispatcherType::Event & event
)
{
	bool found = false;
	dispatcher.forEachIf(
		event,
		[&found](
			const typename DispatcherType::Handle & handle,
			const typename DispatcherType::Callback & item
			) -> bool {
		found = true;
		return false;
	}
	);

	return found;
}

template <typename CallbackListType>
bool hasListener(
	CallbackListType & callbackList,
	const typename CallbackListType::Callback & callback
)
{
	bool found = false;
	callbackList.forEachIf(
		[&found, &callback](
			const typename CallbackListType::Handle & handle,
			const typename CallbackListType::Callback & item
			) -> bool {
			if(item == callback) {
				found = true;
				return false;
			}
			else {
				return true;
			}
		}
	);

	return found;
}

template <typename CallbackListType>
bool hasAnyListener(
	CallbackListType & callbackList
)
{
	bool found = false;
	callbackList.forEachIf(
		[&found](
			const typename CallbackListType::Handle & handle,
			const typename CallbackListType::Callback & item
			) -> bool {
			found = true;
			return false;
		}
	);

	return found;
}


} 

#endif

