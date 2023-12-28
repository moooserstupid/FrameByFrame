#pragma once

#include <iostream>
#include <chrono>
#include <time.h>
#include <filesystem>
#include "opencv2/opencv.hpp"
#include "threadstream.h"
#include "Queue.hpp"

namespace fbf {
	enum NamingConvention {
		DATETIME,
		ITERATOR,
		NAME_PLUS_ITERATOR
	};
	class ImageWriter {
	public:
		ImageWriter(const int source, const std::string format, const NamingConvention naming_convention,
			const bool show_capture, const bool is_capture_strict);
		bool begin_write(PolyM::Queue& shared_queue, std::string filepath, 
			int max_intervals, std::chrono::milliseconds max_interval_duration, std::string prefix = "");
		
		bool end_write();
	private:
		cv::VideoCapture m_cap;
		NamingConvention m_naming_convention;
		std::string m_filepath;
		std::string m_fileformat;
		bool m_show_capture;
		bool m_is_capture_strict;
	};
}


