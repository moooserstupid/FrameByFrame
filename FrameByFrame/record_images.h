#pragma once

#include <stdio.h>
#include <chrono>
#include <time.h>
#include "opencv2/opencv.hpp"

namespace fbf {
	enum NamingConvention {
		DATETIME,
		ITERATOR,
		NAME_PLUS_ITERATOR
	};
	class ImageWriter {

	public:
		ImageWriter(int);
		bool begin_write(std::string filepath, std::string format, NamingConvention naming_conv, bool show_capture, bool is_capture_strict, std::string prefix = "");
		//bool end_write();
	private:
		cv::Mat m_frame;
		cv::VideoCapture m_cap;
		std::string m_filepath;
		std::string m_filename;
		std::string m_fileformat;
		
	};
}


