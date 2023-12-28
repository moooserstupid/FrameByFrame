#include "custom_utils.hpp"

utils::Profiler::Profiler()
{
}

void utils::Profiler::begin_timer()
{
	start_time = std::chrono::steady_clock::now();
}

std::chrono::milliseconds utils::Profiler::end_timer()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time);
}
