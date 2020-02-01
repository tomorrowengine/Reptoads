












#ifndef CONDITIONALREMOVER_H_882115092280
#define CONDITIONALREMOVER_H_882115092280

#include "../eventpolicies.h"

#include <memory>

namespace eventpp {

template <typename DispatcherType, typename Enabled = void>
class ConditionalRemover;

template <typename DispatcherType>
class ConditionalRemover <
		DispatcherType,
		typename std::enable_if<std::is_base_of<TagEventDispatcher, DispatcherType>::value>::type
	>
{
private:
	template <typename Condition>
	struct ItemByCondition
	{
		struct Data
		{
			Condition shouldRemove;
			DispatcherType & dispatcher;
			typename DispatcherType::Event event;
			typename DispatcherType::Callback listener;
			typename DispatcherType::Handle handle;
		};

		template <typename ...Args>
		void operator() (Args && ...args) const {
			if(data->shouldRemove()) {
				data->dispatcher.removeListener(data->event, data->handle);
			}
			data->listener(std::forward(args)...);
		}
		
		std::shared_ptr<Data> data;
	};

public:
	explicit ConditionalRemover(DispatcherType & dispatcher)
		: dispatcher(dispatcher)
	{
	}
	
	template <typename Condition>
	typename DispatcherType::Handle appendListener(
			const typename DispatcherType::Event & event,
			const typename DispatcherType::Callback & listener,
			const Condition & condition
		)
	{
		using Wrapper = ItemByCondition<Condition>;
		auto data = std::make_shared<typename Wrapper::Data>(typename Wrapper::Data {
			condition, dispatcher, event, listener, typename DispatcherType::Handle()
		});
		data->handle = dispatcher.appendListener(event, Wrapper{data});
		return data->handle;
	}

	template <typename Condition>
	typename DispatcherType::Handle prependListener(
			const typename DispatcherType::Event & event,
			const typename DispatcherType::Callback & listener,
			const Condition & condition
		)
	{
		using Wrapper = ItemByCondition<Condition>;
		auto data = std::make_shared<typename Wrapper::Data>(typename Wrapper::Data {
			condition, dispatcher, event, listener, typename DispatcherType::Handle()
		});
		data->handle = dispatcher.prependListener(event, Wrapper{data});
		return data->handle;
	}

	template <typename Condition>
	typename DispatcherType::Handle insertListener(
			const typename DispatcherType::Event & event,
			const typename DispatcherType::Callback & listener,
			const typename DispatcherType::Handle & before,
			const Condition & condition
		)
	{
		using Wrapper = ItemByCondition<Condition>;
		auto data = std::make_shared<typename Wrapper::Data>(typename Wrapper::Data {
			condition, dispatcher, event, listener, typename DispatcherType::Handle()
		});
		data->handle = dispatcher.insertListener(event, Wrapper{data}, before);
		return data->handle;
	}

private:
	DispatcherType & dispatcher;
};

template <typename CallbackListType>
class ConditionalRemover <
		CallbackListType,
		typename std::enable_if<std::is_base_of<TagCallbackList, CallbackListType>::value>::type
	>
{
private:
	template <typename Condition>
	struct ItemByCondition
	{
		struct Data
		{
			Condition shouldRemove;
			CallbackListType & callbackList;
			typename CallbackListType::Callback listener;
			typename CallbackListType::Handle handle;
		};

		template <typename ...Args>
		void operator() (Args && ...args) const {
			if(data->shouldRemove()) {
				data->callbackList.remove(data->handle);
			}
			data->listener(std::forward(args)...);
		}
		
		std::shared_ptr<Data> data;
	};

public:
	explicit ConditionalRemover(CallbackListType & callbackList)
		: callbackList(callbackList)
	{
	}
	
	template <typename Condition>
	typename CallbackListType::Handle append(
			const typename CallbackListType::Callback & listener,
			const Condition & condition
		)
	{
		using Wrapper = ItemByCondition<Condition>;
		auto data = std::make_shared<typename Wrapper::Data>(typename Wrapper::Data {
			condition, callbackList, listener, typename CallbackListType::Handle()
		});
		data->handle = callbackList.append(Wrapper{data});
		return data->handle;
	}

	template <typename Condition>
	typename CallbackListType::Handle prepend(
			const typename CallbackListType::Callback & listener,
			const Condition & condition
		)
	{
		using Wrapper = ItemByCondition<Condition>;
		auto data = std::make_shared<typename Wrapper::Data>(typename Wrapper::Data {
			condition, callbackList, listener, typename CallbackListType::Handle()
		});
		data->handle = callbackList.prepend(Wrapper{data});
		return data->handle;
	}

	template <typename Condition>
	typename CallbackListType::Handle insert(
			const typename CallbackListType::Callback & listener,
			const typename CallbackListType::Handle & before,
			const Condition & condition
		)
	{
		using Wrapper = ItemByCondition<Condition>;
		auto data = std::make_shared<typename Wrapper::Data>(typename Wrapper::Data {
			condition, callbackList, listener, typename CallbackListType::Handle()
		});
		data->handle = callbackList.insert(Wrapper{data}, before);
		return data->handle;
	}

private:
	CallbackListType & callbackList;
};

template <typename DispatcherType>
ConditionalRemover<DispatcherType> conditionalRemover(DispatcherType & dispatcher)
{
	return ConditionalRemover<DispatcherType>(dispatcher);
}


} 

#endif

