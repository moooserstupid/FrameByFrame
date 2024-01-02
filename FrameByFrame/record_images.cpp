#include "record_images.h"

fbf::ImageWriter::ImageWriter(const int source, const std::string format, const NamingConvention naming_convention,
	const bool show_capture, const bool is_capture_strict) {
	m_cap = cv::VideoCapture(source);
	m_fileformat = format;
	m_naming_convention = naming_convention;
	m_show_capture = show_capture;
	m_is_capture_strict = is_capture_strict;
	//cv::namedWindow();
}
bool fbf::ImageWriter::begin_write(PolyM::Queue& shared_queue, std::string filepath, int max_intervals, 
	std::chrono::milliseconds max_interval_duration, std::string prefix) {
	int frame_count = 0;
	int iterator_name_count = 0;
	int file_index = 0;
	m_filepath = filepath + std::to_string(file_index);
	std::filesystem::create_directories(m_filepath);
	m_filepath = m_filepath + '/';
	std::string file_name;
	auto interval_start_time = std::chrono::system_clock::now();
	auto current_time = interval_start_time;
	if (!m_cap.isOpened()) {
		std::cerr << "Error Opening Capture Device Or The Capture Source Has Not Been Initialized" << std::endl;
		return false;
	}
	while (file_index < max_intervals) {
		
		cv::Mat frame;
		m_cap.set(cv::CAP_PROP_POS_FRAMES, 1.0);
		m_cap >> frame;
		++frame_count;
		if (frame.empty()) {
			if (m_is_capture_strict) {
				std::cerr << "Empty Frame Received. Exiting Capture..." << std::endl;
				return false;
			}
			else {
				std::cerr << "Empty Frame Received." << std::endl;
			}
		}
		else {
			if (m_show_capture) {
				cv::imshow("Webcam Capture", frame);
			}
			current_time = std::chrono::system_clock::now();
			switch (m_naming_convention) {
			case DATETIME: {
				const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(current_time.time_since_epoch());
				char buffer[30];
				struct tm timeinfo;
				const time_t raw_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				localtime_s(&timeinfo, &raw_time);
				strftime(buffer, sizeof(buffer), "%F %H-%M-%S", &timeinfo);
				const std::size_t fractional_seconds = ms.count() % 1000;
				file_name = m_filepath + buffer + '.' + std::to_string(fractional_seconds) + m_fileformat;
				break;
			}
			case ITERATOR: {
				iterator_name_count += 1;
				file_name = m_filepath + std::to_string(iterator_name_count) + m_fileformat;
				break;
			}
			case NAME_PLUS_ITERATOR: {
				iterator_name_count += 1;
				file_name = m_filepath + prefix + '_' + std::to_string(iterator_name_count) + m_fileformat;
				break;
			}
			}
			if (!cv::imwrite(file_name, frame)) {
				if (m_is_capture_strict) {
					std::cerr << " | Unable To Save Successfully. Exiting Capture..." << std::endl;
					return false;
				}
				else {
					std::cerr << " | Unable To Save Successfully." << std::endl;
				}
			}
			auto interval_timer = std::chrono::duration_cast<std::chrono::milliseconds> (current_time - interval_start_time);
			if (interval_timer > max_interval_duration) {
				auto interval_timer = std::chrono::duration_cast<std::chrono::seconds> (current_time - interval_start_time);
				const double fps = (double) frame_count / interval_timer.count();
				shared_queue.put(PolyM::DataMsg<double>(fps));	// The fps is sent as the MsgId so as not to waste space
				++file_index;
				if (file_index < max_intervals) {
					frame_count = 0;
					interval_start_time = current_time;
					m_filepath = filepath + std::to_string(file_index);
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
	return true;
}

bool fbf::ImageWriter::end_write() {
	if (m_cap.isOpened()) {
		m_cap.release();
		return true;
	}
	return false;
}