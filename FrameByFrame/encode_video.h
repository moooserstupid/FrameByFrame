#pragma once
#include <string>
#include <iostream>
#include <filesystem>
#include <chrono>
#include "opencv2/opencv.hpp"
#include "threadstream.h"
#include "Queue.hpp"
namespace fbf {
	class VideoEncoder {
	public:
		VideoEncoder();
		bool begin_encode(PolyM::Queue& q, std::string filepath, int max_intervals);
	};
}