#include "record_images.h"

fbf::ImageWriter::ImageWriter(int source) {
	m_cap = cv::VideoCapture(source);
}

bool fbf::ImageWriter::begin_write(std::string filepath, std::string format, NamingConvention naming_conv, bool show_capture, bool is_capture_strict, std::string prefix) {
	
	int count = 0;
	m_filepath = filepath;
	m_fileformat = format;
	std::string file_name;
	if (!m_cap.isOpened()) {
		std::cerr << "Error Opening Capture Device Or The Capture Source Has Not Been Initialized" << std::endl;
		return false;
	}
	while (true) {
		time_t raw_time;
		struct tm timeinfo;
		char buffer[30];
		//std::stringstream file_name;

		m_cap.set(cv::CAP_PROP_POS_FRAMES, 1.0);
		m_cap >> m_frame;
		if (m_frame.empty()) {
			if (is_capture_strict) {
				std::cerr << "Empty Frame Received. Exiting Capture..." << std::endl;
				return false;
			}
			else {
				std::cerr << "Empty Frame Received." << std::endl;
			}
		}
		else {
			if (show_capture) {
				cv::imshow("Webcam Capture", m_frame);
			}
			switch (naming_conv) {
			case DATETIME: {
				auto current_time = std::chrono::high_resolution_clock::now();
				auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(current_time.time_since_epoch());
				std::size_t fractional_seconds;

				raw_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				localtime_s(&timeinfo, &raw_time);
				strftime(buffer, sizeof(buffer), "%F %H-%M-%S", &timeinfo);
				fractional_seconds = ms.count() % 1000;

				//file_name << m_filepath << buffer << '.' << fractional_seconds << m_fileformat;
				file_name = m_filepath + buffer + '.' + std::to_string(fractional_seconds) + m_fileformat;
				break;
			}
			case ITERATOR: {
				count += 1;
				file_name = m_filepath + std::to_string(count) + m_fileformat;
				break;
			}

			case NAME_PLUS_ITERATOR: {
				count += 1;
				file_name = m_filepath + prefix + '_' + std::to_string(count) + m_fileformat;
				break;
			}
			}
			std::cout << file_name << std::endl;
			if (cv::imwrite(file_name, m_frame)) {
				std::cout << " | File Saved Successfully" << std::endl;
			}
			else {
				if (is_capture_strict) {
					std::cerr << " | Unable To Save Successfully. Exiting Capture..." << std::endl;
					return false;
				}
				else {
					std::cerr << " | Unable To Save Successfully." << std::endl;
				}
			}
		}
		cv::waitKey(25);
	}
	cv::waitKey(0);
	return true;
}