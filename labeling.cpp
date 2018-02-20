#include <iostream>
#include "opencv2/opencv.hpp"

int main(){
	cv::VideoCapture capture(0);
	
	if(!capture.isOpened()){
		std::cerr << "Cannot Find Webcam available" << std::endl;
		return EXIT_FAILURE;
	}

	cv::Mat frame, result;

	cv::String windowName = "Labeling";
	cv::namedWindow(windowName, CV_WINDOW_AUTOSIZE);

	capture >> frame;
	cv::Size size = frame.size();

	result = frame.clone();

	while(cv::waitKey(1) != 113){
		capture >> frame;

		for(int j = 0; j < size.height; j++){

			for(int i = 0; i < size.width; i++){
				std::cout << frame.at<cv::Vec3b>(j, i) << std::endl;

			}
		}

		cv::imshow(windowName, frame);
	}

	cv::destroyAllWindows();

	return 0;
}
