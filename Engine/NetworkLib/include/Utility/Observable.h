#pragma once

#include <vector>
#include <functional>

template <class T>
class Observable
{
public:
	Observable() {};
	virtual ~Observable() { m_Observers.clear();  };

	void AddObserver(T* observer);
	void RemoveObserver(T* observer);
	void NotifyObservers(const std::function<void(T*)>& onObserver);

private:
	std::vector<T*> m_Observers;

};

template<class T>
inline void Observable<T>::AddObserver(T * observer)
{
	auto res = std::find(m_Observers.begin(), m_Observers.end(), observer);
	if (res == std::end(m_Observers))
		m_Observers.push_back(observer);
}

template<class T>
inline void Observable<T>::RemoveObserver(T * observer)
{
	auto it = m_Observers.begin();

	while (it != m_Observers.end())
	{
		if ((*it) == observer)
		{
			it = m_Observers.erase(it);
		}
		else
		{
			++it;
		}
	}
}

template<class T>
inline void Observable<T>::NotifyObservers(const std::function<void(T*)>& onObserver)
{
	for (T* observer : m_Observers) {
		onObserver(observer);
	}
}
