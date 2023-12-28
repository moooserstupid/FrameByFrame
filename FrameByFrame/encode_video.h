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
		VideoEncoder(const std::string img_extension, const std::string output_extension, const int fourcc_code);
		bool begin_encode(PolyM::Queue& shared_queue, std::string filepath, int max_intervals);
	private:
		std::string m_img_extension;
		std::string m_output_extension;
		int m_codec;
	};
}