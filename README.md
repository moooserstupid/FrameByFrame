#FrameByFrame

A simple, lightweight program that gets images from a video source at its frame rate and saves it to a folder. At a specified time interval, it begins encoding the recorded images into a video file, 
then creates a new folder and starts saving the images there. The image getting and saving thread, and the video encoding thread are separate, and the encoding of the images into video does not 
affect the saving of images and vice-versa. Makes use of the c++ opencv library for both recording and encoding. 
