#include "encode_video.h"
namespace fs = std::filesystem;
fbf::VideoEncoder::VideoEncoder()
{
}

template <typename TP>
time_t to_time_t(TP tp) {
	using namespace std::chrono;
	auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now() + system_clock::now());
	return system_clock::to_time_t(sctp);
}
bool fbf::VideoEncoder::begin_encode(PolyM::Queue& q, std::string filepath, int max_intervals)
{
	namespace fs = std::filesystem;
	int m_file_index = 0;
	const std::string img_extension = ".bmp";
	const std::string output_extenstion = ".mkv";
	const int codec = cv::VideoWriter::fourcc('H', '2', '6', '4');
	q.get();
	while (m_file_index < 10) {
		std::map<fs::file_time_type, fs::directory_entry> sort_by_time;
		const std::string subfolder = filepath + std::to_string(m_file_index) + '/';
		const std::string output_name = subfolder + std::to_string(m_file_index) + output_extenstion;
		std::vector<cv::Mat> images;
		for (const auto& entry : fs::directory_iterator(subfolder)) {
			const auto filenameStr = entry.path().filename().string();
			
			if (entry.is_directory()) {
				ThreadStream(std::cout) << "dir: " << filenameStr << '\n';
			}
			else if (entry.is_regular_file()) {
				const std::string name_plus_path = subfolder + filenameStr;
				if (entry.path().extension() == img_extension) {
					auto file_time = entry.last_write_time();

					sort_by_time.insert(std::make_pair(file_time, entry));
				}
			}
			else {
				ThreadStream(std::cout) << "??   " << filenameStr << '\n';
			}
		}

		cv::VideoWriter output_video;
		bool size_set = false;
		cv::Size image_size;
		
		for (const auto& p : sort_by_time)
		{
			cv::Mat img;
			img = cv::imread(p.second.path().generic_string());
			if (!size_set) {
				image_size = img.size();
				output_video.open(output_name, codec, 30, image_size);
				if (!output_video.isOpened()) {
					std::cout << "Could not open the output video for write: " << '\n';
					return -1;
				}
				size_set = true;
			}
			output_video << img;
			std::cout << p.second << std::endl;
		}
		if (m_file_index != 9) q.get();
		++m_file_index;
	}
	
	return false;
}
