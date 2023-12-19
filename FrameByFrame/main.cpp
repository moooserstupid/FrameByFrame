#include "record_images.h"

int main() {
	fbf::ImageWriter img_writer(0);
	img_writer.begin_write("Output/", ".bmp", fbf::DATETIME, true, false);
	return 0;
}

