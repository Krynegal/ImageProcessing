#include <opencv2/opencv.hpp>


uchar brightness_func(const uchar& x) {
	return 25 * log(1 + x);
}

cv::Mat create_LUT(const cv::Mat& image) {
	cv::Mat look_up_table(cv::Mat::zeros(cv::Size{ 1, 256 }, CV_8UC1));
	for (int i = 0; i < 256; i++) {
		look_up_table.at<uchar>(i) = brightness_func(i);
	}
	return look_up_table;
}

cv::Mat func_graph(const cv::Mat& look_up_table) {
	cv::Size size(512, 512);
	cv::Mat graph(cv::Mat::ones(size, CV_8UC1) * 255);
	double kw(size.width / 256.0);
	int maxVal = 1;

	for (int i = 0; i < look_up_table.rows; i++) {
		uchar val(look_up_table.at<uchar>(i));
		if (val > maxVal) {
			maxVal = val;
		}
	}
	for (int i = 0; i < 256; i++) {
		circle(graph, cv::Point{ static_cast<int>(i * kw), static_cast<int>(size.height * (1.0 - look_up_table.at<uchar>(i) * 1.0 / maxVal)) }, 1, cv::Scalar(0), cv::FILLED);
	}
	cv::Mat h_line(10, 512, CV_8UC1);
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 10; j++) {
			int color = 0;
			if (i % 10 == 0 && i > 9) {
				color = 255;
			}
			h_line.at<uchar>(j, i) = color;
		}
	}
	graph.push_back(h_line);
	std::cout << "\n" << graph.size() << "\n";
	cv::Mat v_line(522, 10, CV_8UC1);
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 522; j++) {
			int color = 0;
			if (j % 10 == 0 && j < 501) {
				color = 255;
			}
			v_line.at<uchar>(j, i) = color;
		}
	}
	cv::hconcat(v_line, graph, graph);
	return graph;
}

int main() {
	cv::Mat img = cv::imread("..\\..\\..\\data\\cross_0256x0256.png");

	cv::imshow("coloured image", img);
	cv::imwrite("lab03_rgb.png", img);
	
	cv::Mat look_up_table(create_LUT(img));
	cv::Mat img_res(cv::Mat::zeros(img.size(), img.type()));
	cv::LUT(img, look_up_table, img_res);
	cv::imshow("lab03_rgb_res", img_res);
	cv::imwrite("lab03_rgb_res.png", img_res);
	
	cv::Mat gray_img;
	cv::cvtColor(img, gray_img, cv::COLOR_BGR2GRAY);
	cv::imshow("lab03_gre", gray_img);
	cv::imwrite("lab03_gre.png", gray_img);
	
	cv::Mat gray_img_res(cv::Mat::zeros(gray_img.size(), gray_img.type()));
	cv::LUT(gray_img, look_up_table, gray_img_res);
	cv::imshow("lab03_gre_res", gray_img_res);
	cv::imwrite("lab03_gre_res.png", gray_img_res);
	
	cv::Mat graph(func_graph(look_up_table));
	cv::imshow("lab03_viz_func", graph);
	cv::imwrite("lab03_viz_func.png", graph);

	cv::waitKey(0);
	return 0;
}