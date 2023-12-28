#include <filesystem>
#include <queue>
#include <thread>
#include <future>
#include "record_images.h"
#include "encode_video.h"
#include "Queue.hpp"
#include "custom_utils.hpp"

namespace fs = std::filesystem;
int main() {
	using namespace std::chrono_literals;
	fbf::ImageWriter img_writer(0, ".bmp", fbf::DATETIME, true, false);
	fbf::VideoEncoder video_encoder = fbf::VideoEncoder(".bmp", ".mkv", cv::VideoWriter::fourcc('H', '2', '6', '4'));
	const std::string main_dir = "Output/";
	auto interval_duration = 2s;
	const int num_intervals = 5;
	std::thread image_write_thread;
	std::thread encoder_thread;
	utils::Profiler main_thread_profiler;

	PolyM::Queue q;
	main_thread_profiler.begin_timer();
	image_write_thread = std::thread([&]() { img_writer.begin_write(std::ref(q), main_dir, num_intervals, interval_duration); });
	encoder_thread = std::thread([&]() { video_encoder.begin_encode(std::ref(q), main_dir, num_intervals); });
	image_write_thread.join();
	encoder_thread.join();
	
	std::cout << "Main Thread Run Time: " << main_thread_profiler.end_timer().count() << '\n';
	return 0;
}

