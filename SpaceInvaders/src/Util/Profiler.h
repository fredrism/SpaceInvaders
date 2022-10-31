#pragma once
#include <chrono>
#include <iostream>

#ifdef _DEBUG
	#define SI_PROFILE(x) { ScopeProfiler p(__FILE__, __LINE__, #x); x; }
	#define SI_SCOPE_PROFILE(x) ScopeProfiler p(__FILE__, __LINE__, x)
#else
	#define SI_PROFILE(x) x
	#define SI_SCOPE_PROFILE(x)
#endif



class Timer
{
public:
	Timer()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	void Start()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	std::chrono::duration<float> Stop()
	{
		auto end = std::chrono::high_resolution_clock::now();
		return end - start;
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> start;
};

class ScopeProfiler
{
public:
	ScopeProfiler(const char* file, int line, const char* function)
	{
		start = std::chrono::high_resolution_clock::now();
		m_file = file;
		m_line = line;
		m_func = function;
	}

	~ScopeProfiler()
	{
		auto end = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		
		std::cout << m_line << " : " << m_func << " : " << elapsed << std::endl;
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> start;
	const char* m_file;
	const char* m_func;
	int m_line;
};

class FrameTimer
{
public:
	FrameTimer(int length = 10)
	{
		m_length = length;
		m_lastFrame = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < length; i++)
		{
			m_frames.push_back(1.0 / 60.0);
		}
	}

	float Poll()
	{
		auto newTime = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(newTime - m_lastFrame);
		m_frames[m_index] = elapsed.count() / 1000000000.0;
		m_index = (m_index + 1) % m_length;

		float sum = 0;
		for (int i = 0; i < m_length; i++)
		{
			sum += m_frames[i];
		}
		
		m_lastFrame = newTime;

		return sum / m_length;
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> m_lastFrame;

	int m_index = 0;
	int m_length;
	std::vector<float> m_frames;
};