#include <filesystem>
#include <queue>
#include <thread>
#include <future>
#include "record_images.h"
#include "encode_video.h"
#include "Queue.hpp"
#include "custom_utils.hpp"
#include <opencv2/core/utils/logger.hpp>
namespace fs = std::filesystem;
int main() {
	using namespace std::chrono_literals;
	cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);
	const std::string img_extension = ".bmp";
	const std::string vid_extension = ".mkv";
	fbf::ImageWriter img_writer(0, img_extension, fbf::DATETIME, true, false);
	fbf::VideoEncoder video_encoder = fbf::VideoEncoder(img_extension, vid_extension, cv::VideoWriter::fourcc('H', '2', '6', '4'));
	const std::string main_dir = "Output/";
	auto interval_duration = 2s;
	const int num_intervals = 5;
	std::thread image_write_thread;
	std::thread encoder_thread;
	{
		PolyM::Queue q;
		utils::Profiler main_thread_profiler("Main Thread");
		image_write_thread = std::thread([&]() { img_writer.begin_write(std::ref(q), main_dir, num_intervals, interval_duration); });
		encoder_thread = std::thread([&]() { video_encoder.begin_encode(std::ref(q), main_dir, num_intervals); });
		image_write_thread.join();
		encoder_thread.join();
	}
	
	return 0;
}

