//https://stackoverflow.com/questions/15278343/c11-thread-safe-queue
#ifndef SAFE_QUEUE
#define SAFE_QUEUE

#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

extern int on;

// A threadsafe-queue.
template <class T>
class SafeQueue
{
public:
  SafeQueue(void)
    : q()
    , m()
    , c()
  {}

  ~SafeQueue(void)
  {}

  // Add an element to the queue.
  void enqueue(T t)
  {
    std::lock_guard<std::mutex> lock(m);
    q.push(t);
    c.notify_one();
  }

  // Get the "front"-element.
  // If the queue is empty, wait till a element is avaiable.
  T dequeue(void)
  {
    std::unique_lock<std::mutex> lock(m);
	while (q.empty())
	{
		// release lock as long as the wait and reaquire it afterwards.
		if (!on) {
			T v;
			//printf("interrupt\n");
			return v;
		}
		c.wait(lock);
		//Sleep(500);
	}
	
    T val = q.front();
    q.pop();
    return val;
  }

  void notify()
  {
	  std::lock_guard<std::mutex> lock(m);
	  c.notify_one();
  }

private:
  std::queue<T> q;
  mutable std::mutex m;
  std::condition_variable c;
};
#endif