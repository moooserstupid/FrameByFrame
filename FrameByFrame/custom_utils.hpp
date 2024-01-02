#pragma once
#include <iostream>
#include <chrono>

namespace utils {
	class Profiler {
	public:
		Profiler(const char* name);
		~Profiler();
		void Stop();
	private:
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimePoint;
		const char* m_Name;
		bool m_Stopped;
	};
}