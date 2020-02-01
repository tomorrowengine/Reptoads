












#ifndef COUNTERREMOVER_H_211532679833
#define COUNTERREMOVER_H_211532679833

#include "../eventpolicies.h"

#include <memory>

namespace eventpp {

template <typename DispatcherType, typename Enabled = void>
class CounterRemover;

template <typename DispatcherType>
class CounterRemover <
		DispatcherType,
		typename std::enable_if<std::is_base_of<TagEventDispatcher, DispatcherType>::value>::type
	>
{
private:
	struct Wrapper
	{
		struct Data
		{
			int triggerCount;
			DispatcherType & dispatcher;
			typename DispatcherType::Event event;
			typename DispatcherType::Callback listener;
			typename DispatcherType::Handle handle;
		};

		template <typename ...Args>
		void operator() (Args && ...args) const {
			if(--data->triggerCount <= 0) {
				data->dispatcher.removeListener(data->event, data->handle);
			}
			data->listener(std::forward(args)...);
		}
		
		std::shared_ptr<Data> data;
	};
	
public:
	explicit CounterRemover(DispatcherType & dispatcher)
		: dispatcher(dispatcher)
	{
	}
	
	typename DispatcherType::Handle appendListener(
			const typename DispatcherType::Event & event,
			const typename DispatcherType::Callback & listener,
			const int triggerCount = 1
		)
	{
		auto data = std::make_shared<typename Wrapper::Data>(typename Wrapper::Data {
			triggerCount, dispatcher, event, listener, typename DispatcherType::Handle()
		});
		data->handle = dispatcher.appendListener(event, Wrapper{data});
		return data->handle;
	}

	typename DispatcherType::Handle prependListener(
			const typename DispatcherType::Event & event,
			const typename DispatcherType::Callback & listener,
			const int triggerCount = 1
		)
	{
		auto data = std::make_shared<typename Wrapper::Data>(typename Wrapper::Data {
			triggerCount, dispatcher, event, listener, typename DispatcherType::Handle()
		});
		data->handle = dispatcher.prependListener(event, Wrapper{data});
		return data->handle;
	}

	typename DispatcherType::Handle insertListener(
			const typename DispatcherType::Event & event,
			const typename DispatcherType::Callback & listener,
			const typename DispatcherType::Handle & before,
			const int triggerCount = 1
		)
	{
		auto data = std::make_shared<typename Wrapper::Data>(typename Wrapper::Data {
			triggerCount, dispatcher, event, listener, typename DispatcherType::Handle()
		});
		data->handle = dispatcher.insertListener(event, Wrapper{data}, before);
		return data->handle;
	}

private:
	DispatcherType & dispatcher;
};

template <typename CallbackListType>
class CounterRemover <
		CallbackListType,
		typename std::enable_if<std::is_base_of<TagCallbackList, CallbackListType>::value>::type
	>
{
private:
	struct Wrapper
	{
		struct Data
		{
			int triggerCount;
			CallbackListType & callbackList;
			typename CallbackListType::Callback listener;
			typename CallbackListType::Handle handle;
		};

		template <typename ...Args>
		void operator() (Args && ...args) const {
			if(--data->triggerCount <= 0) {
				data->callbackList.remove(data->handle);
			}
			data->listener(std::forward(args)...);
		}
		
		std::shared_ptr<Data> data;
	};

public:
	explicit CounterRemover(CallbackListType & callbackList)
		: callbackList(callbackList)
	{
	}
	
	typename CallbackListType::Handle append(
			const typename CallbackListType::Callback & listener,
			const int triggerCount = 1
		)
	{
		auto data = std::make_shared<typename Wrapper::Data>(typename Wrapper::Data {
			triggerCount, callbackList, listener, typename CallbackListType::Handle()
		});
		data->handle = callbackList.append(Wrapper{data});
		return data->handle;
	}

	typename CallbackListType::Handle prepend(
			const typename CallbackListType::Callback & listener,
			const int triggerCount = 1
		)
	{
		auto data = std::make_shared<typename Wrapper::Data>(typename Wrapper::Data {
			triggerCount, callbackList, listener, typename CallbackListType::Handle()
		});
		data->handle = callbackList.prepend(Wrapper{data});
		return data->handle;
	}

	typename CallbackListType::Handle insert(
			const typename CallbackListType::Callback & listener,
			const typename CallbackListType::Handle & before,
			const int triggerCount = 1
		)
	{
		auto data = std::make_shared<typename Wrapper::Data>(typename Wrapper::Data {
			triggerCount, callbackList, listener, typename CallbackListType::Handle()
		});
		data->handle = callbackList.insert(Wrapper{data}, before);
		return data->handle;
	}

private:
	CallbackListType & callbackList;
};

template <typename DispatcherType>
CounterRemover<DispatcherType> counterRemover(DispatcherType & dispatcher)
{
	return CounterRemover<DispatcherType>(dispatcher);
}


} 

#endif
