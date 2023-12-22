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
		ImageWriter(int);
		bool begin_write(PolyM::Queue& q, std::string filepath, std::string format, 
			NamingConvention naming_conv, bool show_capture, bool is_capture_strict, 
			int max_intervals, int max_interval_duration,  std::string prefix = "");
		bool end_write();
	private:
		cv::Mat m_frame;
		cv::VideoCapture m_cap;
		std::string m_filepath;
		std::string m_filename;
		std::string m_fileformat;
		int m_file_index = 0;
	};
}


