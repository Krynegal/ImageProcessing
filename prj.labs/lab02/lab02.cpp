#include <opencv2/opencv.hpp>
#include <opencv2/cvconfig.h>


cv::Mat getMosaic(cv::Mat img) {
	cv::Mat red_img, green_img, blue_img, tmp_img, out_img;

	std::vector<cv::Mat> rgbChannels(3);
	cv::split(img, rgbChannels);

	cv::Mat g = cv::Mat::zeros(cv::Size(img.rows, img.cols), CV_8UC1);

	std::vector<cv::Mat> blue_channel{rgbChannels[0], g, g};
	std::vector<cv::Mat> green_channel{g, rgbChannels[1], g};
	std::vector<cv::Mat> red_channel{g, g, rgbChannels[2]};

	cv::merge(blue_channel, blue_img);
	cv::merge(green_channel, green_img);
	cv::merge(red_channel, red_img);

	cv::hconcat(img, red_img, tmp_img);
	cv::hconcat(green_img, blue_img, out_img);
	cv::vconcat(tmp_img, out_img, out_img);

	return out_img;
}

cv::Mat getHistogram(cv::Mat img) {
	std::vector<cv::Mat> rgb_channels;
	split(img, rgb_channels);

	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange[] = { range };
	
	cv::Mat b_hist, g_hist, r_hist;
	calcHist(&rgb_channels[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, histRange);
	calcHist(&rgb_channels[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, histRange);
	calcHist(&rgb_channels[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, histRange);
	
	int hist_w = 512, hist_h = 256;
	int bin_w = cvRound((double)hist_w / histSize);

	cv::Mat hist_img(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
	normalize(b_hist, b_hist, 0, hist_img.rows, cv::NORM_MINMAX, -1, cv::Mat());
	normalize(g_hist, g_hist, 0, hist_img.rows, cv::NORM_MINMAX, -1, cv::Mat());
	normalize(r_hist, r_hist, 0, hist_img.rows, cv::NORM_MINMAX, -1, cv::Mat());

	for (int i = 1; i < histSize; i++)
	{
		line(hist_img, cv::Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			cv::Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
			cv::Scalar(255, 0, 0), 2, 8, 0);
		line(hist_img, cv::Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			cv::Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
			cv::Scalar(0, 255, 0), 2, 8, 0);
		line(hist_img, cv::Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			cv::Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
			cv::Scalar(0, 0, 255), 2, 8, 0);
	}

	return hist_img;
}

int main() {
	cv::Mat img = cv::imread("..\\..\\..\\data\\cross_0256x0256.png");

	cv::imwrite("cross_0256x0256_025.jpg", img, {cv::IMWRITE_JPEG_QUALITY, 25});
	cv::Mat jpg_img(cv::imread("cross_0256x0256_025.jpg"));

	cv::Mat HQ_mosaic(getMosaic(img));
	cv::imwrite("cross_0256x0256_png_channels.png", HQ_mosaic);

	cv::Mat LQ_mosaic(getMosaic(jpg_img));
	cv::imwrite("cross_0256x0256_jpg_channels.png", LQ_mosaic);

	cv::Mat hist_hq_img(getHistogram(img));
	cv::Mat hist_lq_img(getHistogram(jpg_img));

	cv::Mat space(20, 512, CV_8UC3, cv::Scalar(255, 255, 255));
	hist_hq_img.push_back(space);
	cv::Mat hists;
	cv::vconcat(hist_hq_img, hist_lq_img, hists);
	cv::imwrite("cross_0256x0256_hists.png", hists);
	cv::imshow("hists", hists);

	cv::waitKey(0);
}