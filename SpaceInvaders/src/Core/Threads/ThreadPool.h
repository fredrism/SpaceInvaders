#pragma once
#include <functional>
#include <thread>
#include <mutex>

#include <vector>
#include <queue>

typedef std::function<void(void)> Task;

class ThreadPool
{
public:
	ThreadPool(unsigned int size = 0);
	~ThreadPool();

	void QueueTask(Task& task);
	void Merge();
	bool Exit() { return m_exit; }

	std::atomic_int idleThreads;
	void ThreadExec();

private:
	std::vector<std::thread> m_threads;
	std::queue<Task> m_tasks;

	std::condition_variable m_condition;
	std::mutex m_task_read_mutex;
	std::mutex m_frame_mutex;
	
	std::atomic_bool m_exit;
};

