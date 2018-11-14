
#ifndef SAFE_SET
#define SAFE_SET

#include <set>
#include <mutex>
#include <condition_variable>
#include <iostream>

extern int on;

// A threadsafe-queue.
template <class T>
class SafeSet
{
public:
	SafeSet(void)
		: s()
		, target()
		, m()
		, c()
	{}

	~SafeSet(void)
	{}

	void insert_item(T t)
	{
		std::lock_guard<std::mutex> lock(m);
		s.insert(t);
	}

	T delete_item(T t)
	{
		std::lock_guard<std::mutex> lock(m);
		T temp;
		target=s.find(t);
		temp = *target;
		s.erase(target);
		return temp;
	}

	bool empty()
	{
		std::lock_guard<std::mutex> lock(m);
		return s.empty()?true:false;
	}

	T pop()
	{
		std::lock_guard<std::mutex> lock(m);
		T temp;
		target = s.begin();
		temp = *target;
		s.erase(target);
		return temp;
		
	}



	void notify()
	{
		std::lock_guard<std::mutex> lock(m);
		c.notify_one();
	}

private:
	std::set<T> s;
	typename std::set<T>::iterator target;
	mutable std::mutex m;
	std::condition_variable c;
};
#endif