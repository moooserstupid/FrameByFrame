#include "encode_video.h"
#include "custom_utils.hpp"

namespace fs = std::filesystem;
fbf::VideoEncoder::VideoEncoder(const std::string img_extension, const std::string output_extension,
	const int fourcc_code) {
	m_img_extension = img_extension;
	m_output_extension = output_extension;
	m_codec = fourcc_code;
}

int fbf::VideoEncoder::begin_encode(PolyM::Queue& shared_queue, std::string filepath, int max_intervals){
	namespace fs = std::filesystem;
	cv::VideoWriter output_video;
	cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);
	int interval_count = 0;
	int file_index = 0;
	while (file_index < max_intervals) {
		auto msg = shared_queue.get();
		{
			const auto& dm = dynamic_cast<PolyM::DataMsg<double>&>(*msg);
			const double fps = dm.getMsgId();	// The fps is sent as the MsgId so as not to waste space
			utils::Profiler encoder_profiler("Encoder");
			std::map<unsigned long, fs::directory_entry> sort_by_time;
			const std::string subfolder = filepath + std::to_string(file_index) + '/';
			const std::string output_name = subfolder + std::to_string(file_index) + m_output_extension;
			for (const auto& entry : fs::directory_iterator(subfolder)) {
				const auto filenameStr = entry.path().filename().string();
				if (entry.is_directory()) {
					ThreadStream(std::cout) << "dir: " << filenameStr << '\n';
				}
				else if (entry.is_regular_file()) {
					const std::string name_plus_path = subfolder + filenameStr;
					if (entry.path().extension() == m_img_extension) {
						sort_by_time.insert(std::make_pair(entry.last_write_time().time_since_epoch().count(), entry));
					}
				}
				else {
					ThreadStream(std::cout) << "??   " << filenameStr << '\n';
				}
			}
			bool size_set = false;
			cv::Size image_size;

			for (const auto& p : sort_by_time)
			{
				cv::Mat img;
				{
					utils::Profiler img_read_profiler("Image Read");
					img = cv::imread(p.second.path().generic_string());
				}
				if (!size_set) {
					image_size = img.size();
					output_video.open(output_name, m_codec, fps, image_size);
					if (!output_video.isOpened()) {
						std::cout << "Could not open the output video for write: " << '\n';
						return -1;
					}
					size_set = true;
				}
				{
					utils::Profiler video_write_profiler("Video Write");
					output_video << img;
				}
			}
			

		}
		++file_index;
	}
	return 0;
}