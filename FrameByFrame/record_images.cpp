#include "record_images.h"
fbf::ImageWriter::ImageWriter(int source) {
	m_cap = cv::VideoCapture(source);
}

bool fbf::ImageWriter::begin_write(PolyM::Queue& q, std::string filepath, std::string format, 
	NamingConvention naming_conv, bool show_capture, bool is_capture_strict, int max_intervals, 
	int max_interval_duration, std::string prefix) {
	
	int count = 0;
	int m_file_index = 0;
	m_filepath = filepath + std::to_string(m_file_index);
	std::filesystem::create_directories(m_filepath);
	m_filepath = m_filepath + '/';
	m_fileformat = format;
	std::string file_name;
	auto interval_start_time = std::chrono::system_clock::now();
	auto current_time = interval_start_time;
	auto interval_duration_s = std::chrono::duration_cast<std::chrono::seconds> (current_time - interval_start_time);

	if (!m_cap.isOpened()) {
		std::cerr << "Error Opening Capture Device Or The Capture Source Has Not Been Initialized" << std::endl;
		return false;
	}
	while (m_file_index < max_intervals) {
		time_t raw_time;
		struct tm timeinfo;
		char buffer[30];
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
			current_time = std::chrono::system_clock::now();
			
			
			switch (naming_conv) {
			case DATETIME: {
				auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(current_time.time_since_epoch());
				std::size_t fractional_seconds;

				raw_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				localtime_s(&timeinfo, &raw_time);
				strftime(buffer, sizeof(buffer), "%F %H-%M-%S", &timeinfo);
				fractional_seconds = ms.count() % 1000;

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
			//std::cout << file_name << std::endl;
			if (cv::imwrite(file_name, m_frame)) {
				//ThreadStream(std::cout) << file_name + " | File Saved Successfully" << '\n';
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
			

			interval_duration_s = std::chrono::duration_cast<std::chrono::seconds> (current_time - interval_start_time);
			if (interval_duration_s.count() > max_interval_duration) {
				q.put(PolyM::DataMsg<int>(m_file_index));
				++m_file_index;
				if (m_file_index < max_intervals) {
					interval_start_time = current_time;
					m_filepath = filepath + std::to_string(m_file_index);
					std::filesystem::create_directories(m_filepath);
					m_filepath = m_filepath + '/';
				}
				
			}
			
		}

		const int key_pressed = cv::waitKey(1) & 0xFF;
		if (key_pressed == 27) {
			cv::destroyAllWindows();
			break;
		}
	}

	std::cout << "Finished first thread. " << '\n';
	return true;
}

bool fbf::ImageWriter::end_write() {
	if (m_cap.isOpened()) {
		m_cap.release();
		return true;
	}
	return false;
}