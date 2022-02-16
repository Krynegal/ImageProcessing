#include <opencv2/opencv.hpp>

int main() {
	cv::Mat img = cv::imread("..\\..\\..\\data\\cross_0256x0256.png");

	std::vector<cv::Mat> rgbChannels(3);
	cv::split(img, rgbChannels);

	cv::Mat g = cv::Mat::zeros(cv::Size(img.rows, img.cols), CV_8UC1);

	std::vector<cv::Mat> channels;
	channels.push_back(g);
	channels.push_back(g);
	channels.push_back(rgbChannels[2]);
	cv::Mat red_img;
	cv::merge(channels, red_img);
	cv::hconcat(img, red_img, img);

	/*channels[2] * 0;
	channels.push_back(g);
	channels.push_back(rgbChannels[1]);
	channels.push_back(g);
	cv::Mat green_img;
	cv::merge(channels, green_img);
	cv::vconcat(img, green_img, img);*/

	cv::imshow("mozaic.png", img);
	cv::waitKey(0);
}