#include "record_images.h"
#include "encode_video.h"
#include "Queue.hpp"
#include <filesystem>
#include <queue>
#include <thread>
#include <future>

namespace fs = std::filesystem;

using namespace std::chrono_literals;
int main() {
	fbf::ImageWriter img_writer(0);
	fbf::VideoEncoder video_encoder = fbf::VideoEncoder();
	const std::string main_dir = "Output/";
	std::thread image_write_thread;
	std::thread encoder_thread;
	auto start_time = std::chrono::steady_clock::now();

	PolyM::Queue q;
	image_write_thread = std::thread([&]() { img_writer.begin_write(std::ref(q), main_dir, ".bmp", fbf::DATETIME, true, false, 5, 30); });
	encoder_thread = std::thread([&]() { video_encoder.begin_encode(std::ref(q), main_dir, 5); });
	image_write_thread.join();
	encoder_thread.join();
	
	auto end_time = std::chrono::steady_clock::now();
	auto run_time = std::chrono::duration_cast<std::chrono::milliseconds> (end_time - start_time);
	std::cout << run_time.count() << '\n';
	return 0;
}

