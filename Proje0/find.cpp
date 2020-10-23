#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace cv;


void check_program_arguments(int argc) {
	if(argc != 2) {
		cout << "Error! Program usage:" << endl;
		cout << "./circle_detect image_circles_path" << endl;
		exit(-1);
	}	
}

void check_if_image_exist(const Mat &img, const string &path) {
	if(img.empty()) {
		cout << "Error! Unable to load image: " << path << endl;
		exit(-1);
	}	
}


int main(int argc, char **argv) {
	// Usage: ./circle_detect image_circles_path
	check_program_arguments(argc);
	
	// Load input image
	string path_image{argv[1]};
	Mat bgr_image = imread(path_image);

	// Check if the image can be loaded
	check_if_image_exist(bgr_image, path_image);

	Mat orig_image = bgr_image.clone();

	medianBlur(bgr_image, bgr_image, 3);

	// Convert input image to HSV
	Mat hsv_image;
	cvtColor(bgr_image, hsv_image, COLOR_BGR2HSV);

	// Threshold the HSV image, keep only the red pixels
	Mat lower_red_hue_range;
	Mat upper_red_hue_range;
	inRange(hsv_image, Scalar(0, 100, 100), Scalar(10, 255, 255), lower_red_hue_range);
	inRange(hsv_image, Scalar(160, 100, 100), Scalar(179, 255, 255), upper_red_hue_range);

	// Combine the above two images
	Mat red_hue_image;
	addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);

	GaussianBlur(red_hue_image, red_hue_image, Size(9, 9), 2, 2);

	// Use the Hough transform to detect circles in the combined threshold image
	vector<Vec3f> circles;
	HoughCircles(red_hue_image, circles, HOUGH_GRADIENT, 1, red_hue_image.rows/8, 100, 20, 0, 0);

	// Loop over all detected circles and outline them on the original image
	if(circles.size() == 0) exit(-1);
    
	for(size_t current_circle = 0; current_circle < circles.size(); ++current_circle) {
		Point center(round(circles[current_circle][0]), round(circles[current_circle][1]));
		int radius = round(circles[current_circle][2]);

		circle(orig_image, center, radius, Scalar(0, 255, 0), 5);
	}

	// Show images
	namedWindow("Threshold lower image", WINDOW_AUTOSIZE);
	imshow("Threshold lower image", lower_red_hue_range);
	namedWindow("Threshold upper image", WINDOW_AUTOSIZE);
	imshow("Threshold upper image", upper_red_hue_range);
	namedWindow("Combined threshold images", WINDOW_AUTOSIZE);
	imshow("Combined threshold images", red_hue_image);
	namedWindow("Detected red circles on the input image", WINDOW_AUTOSIZE);
	imshow("Detected red circles on the input image", orig_image);

	waitKey(0);

	
	return 0;
}