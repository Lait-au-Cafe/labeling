#include <iostream>
#include <vector>
#include <random>
#include "opencv2/opencv.hpp"

using uchar = unsigned char;
using uint = unsigned int;

int main(){
	cv::VideoCapture capture(0);
	
	if(!capture.isOpened()){
		std::cerr << "Cannot Find Webcam available" << std::endl;
		return EXIT_FAILURE;
	}

	cv::String windowName = "Labeling";
	cv::namedWindow(windowName, CV_WINDOW_AUTOSIZE);

	cv::Mat frame, result;

//	frame = cv::imread("/home/shun/amano/projects/labeling/banana.png", 1);
//	if(frame.empty()){
//		std::cerr << "Cannot load input file" << std::endl;
//		return EXIT_FAILURE;
//	}
//	cv::imshow(windowName, frame);
//	while(cv::waitKey(1) != 113){}


	capture >> frame;
	cv::Size size = frame.size();

	result = frame.clone();

	// Main Process
	const uchar thresh = 128; // threshold
	cv::Vec3b col; // color buffer
		
	std::vector<std::vector<int>> id(size.height, std::vector<int>(size.width)); // pseudo id
	std::vector<cv::Vec3b> label_source; // label
	std::vector<cv::Vec3b> label; // label
	//std::vector<cv::Vec3b> label; // label
	int last_lab;

	int up_left, up, up_right, left;
	int ni, nj;

	std::random_device rnd_dev;
	std::mt19937 rnd_mt(rnd_dev());
	std::uniform_int_distribution<> unif(0, 255);

	while(cv::waitKey(1) != 113){
		capture >> frame;

		label = label_source; // label
		last_lab = -1;
		for(int j = 0; j < size.height; j++){
			for(int i = 0; i < size.width; i++){
				col = frame.at<cv::Vec3b>(j, i);

				// black
				if((col[0] + col[1] + col[2]) / 3 < thresh){
					up_left = up = up_right = left = -1;
					
					// [+] [ ] [ ]
					// [ ]
					ni = i - 1;
					nj = j - 1;
					if(ni >= 0 && nj >= 0){ up_left = id[nj][ni]; }
					if(up_left >= 0){
						id[j][i] = up_left;

						ni += 2;
						if(ni < size.width){
							up_right = id[nj][ni];
							if(up_right >= 0){
								label[up_right] = label[up_left];
							}
						}
					}
					else{
						// [ ] [+] [ ]
						// [ ]
						ni++;
						if(nj >= 0){ up = id[nj][ni]; }
						if(up >= 0){
							id[j][i] = up;

							ni--;
							nj++;
							if(ni >= 0){
								left = id[nj][ni];
								if(left >= 0){
									label[left] = label[up];
								}
							}
						}
						else{
							// [ ] [ ] [+]
							// [ ]
							ni++;
							if(ni < size.width && nj >= 0){ up_right = id[nj][ni]; }
							if(up_right >= 0){
								id[j][i] = up_right;

								ni -= 2;
								nj++;
								if(ni >= 0){
									left = id[nj][ni];
									if(left >= 0){
										label[left] = label[up_right];
									}
								}
							}
							else{
								// [ ] [ ] [ ]
								// [+]
								ni -= 2;
								nj++;
								if(ni >= 0){ left = id[nj][ni]; }
								if(left >= 0){
									id[j][i] = left;
								}
								else{
									// create new label
									last_lab++;
									while(last_lab >= (int)label.size()){
										col = cv::Vec3b(
												unif(rnd_mt), 
												unif(rnd_mt), 
												unif(rnd_mt));
										label_source.push_back(col);
										label.push_back(col);
									}
									id[j][i] = last_lab;
								}
							}
						}
					}
				} 
				else {
					id[j][i] = -1;
				}
				// result.at<cv::Vec3b>(j, i) = col;
			}
		}

		for(int j = 0; j < size.height; j++){
			for(int i = 0; i < size.width; i++){
				int n = id[j][i];
				if(n >= 0){
					result.at<cv::Vec3b>(j, i) = label[n];
				}
				else{
					result.at<cv::Vec3b>(j, i) = cv::Vec3b(255, 255, 255);
				}
			}
		}

		cv::imshow(windowName, result);
//		while(cv::waitKey(1) != 113){}
	}

	std::cout << "Labels" << std::endl;
	for(int k = 0; k <= last_lab; k++){
		std::cout << k << " : " << label[k] << std::endl;
	}

	for(int j = 0; j < size.height; j++){
		for(int i = 0; i < size.width; i++){
			std::cout << id[j][i] << " " << std::ends;
		}
		std::cout << std::endl;
	}

	//cv::destroyAllWindows();

	return 0;
}
