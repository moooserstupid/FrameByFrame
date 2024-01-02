#include "custom_utils.hpp"

utils::Profiler::Profiler(const char* name) 
	: m_Name(name), m_Stopped(false)
{
	m_StartTimePoint = std::chrono::steady_clock::now();
}

utils::Profiler::~Profiler()
{
	Stop();
}

void utils::Profiler::Stop()
{
	auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
	auto duration = end - start;
	double ms = duration * 0.001;
	std::cout << m_Name << ": " << duration << "us (" << ms << "ms)" << '\n';
	m_Stopped = true;
}
