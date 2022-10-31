#include "pch.h"
#include "ThreadPool.h"

ThreadPool::ThreadPool(unsigned int size) : m_exit(false)
{
	if (size == 0 || size > std::thread::hardware_concurrency())
	{
		size = std::thread::hardware_concurrency();
	}

	for (unsigned int i = 0; i < size; i++)
	{
		m_threads.push_back(std::thread(&ThreadPool::ThreadExec, this));
	}

	idleThreads = m_threads.size();
}

ThreadPool::~ThreadPool()
{
	m_exit = true;
	m_condition.notify_all();
	for (auto& t : m_threads)
	{
		t.join();
	}
}

void ThreadPool::QueueTask(Task& task)
{
	{
		std::unique_lock<std::mutex> lock(m_task_read_mutex);
		m_tasks.push(task);
	}
	
	m_condition.notify_one();
}

void ThreadPool::Merge()
{
	while (true)
	{
		if (idleThreads == m_threads.size()) break;
	}
}

void ThreadPool::ThreadExec()
{
	
	while (true)
	{
		Task task;
		{
			std::unique_lock<std::mutex> lock(m_task_read_mutex);
			m_condition.wait(lock, [this]() { return !m_tasks.empty() || m_exit; });
			if (m_exit) break;
			task = m_tasks.front();
			m_tasks.pop();
		}
		idleThreads--;
		task();
		idleThreads++;
	}
}

void func()
{

}