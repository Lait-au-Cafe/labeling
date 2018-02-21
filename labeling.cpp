#include <iostream>
#include "opencv2/opencv.hpp"

typedef unsigned char uchar;

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

	const uchar thresh = 128;
	cv::Vec3b col;
	while(cv::waitKey(1) != 113){
		capture >> frame;

		for(int j = 0; j < size.height; j++){
			for(int i = 0; i < size.width; i++){
				col = frame.at<cv::Vec3b>(j, i);
				col = (col[0] + col[1] + col[2]) / 3 > thresh ?
						cv::Vec3b(255, 255, 255) : cv::Vec3b(0, 0, 0);
				result.at<cv::Vec3b>(j, i) = col;
			}
		}


		cv::imshow(windowName, result);
	}

	cv::destroyAllWindows();

	return 0;
}
