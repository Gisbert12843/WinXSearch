#include "thread_guard.h"

#include <vector>
#include <thread>
#include <iostream>

//singleton implemented thread_guard managing all occuring threads
class thread_guard
{
private:
	thread_guard() = default;
	~thread_guard() = default;
	thread_guard(const thread_guard&) = delete;
	thread_guard& operator=(const thread_guard&) = delete;



public:
	std::vector<std::thread>thread_list = {};
	int current_thread_count = 0;
	const int processor_count = std::thread::hardware_concurrency();


	static thread_guard& getInstance()
	{
		static thread_guard instance;

		return instance;
	}

	void operator()() const
	{

	}

};

