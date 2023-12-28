#pragma once
#include <chrono>

namespace utils {
	class Profiler {
	public:
		Profiler();
		void begin_timer();
		std::chrono::milliseconds end_timer();
	private:
		std::chrono::steady_clock::time_point start_time;
	};
}