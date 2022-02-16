#include <opencv2/opencv.hpp>
#include <chrono>

int main() {
  cv::Mat img(60, 768, CV_8UC1);
  img = 0;

  auto start_time = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < 256; i++) {
      for (int j = 0; j < img.rows ; j++) {
          for (int k = 0; k < 3; k++) {
            img.at<uchar>(j, 3 * i + k) = i;
          }
      }
  }
  auto end_time = std::chrono::high_resolution_clock::now(); 
  std::chrono::duration<double> duration = end_time - start_time;
  std::cout << "Duration " << duration.count() << "s" << std::endl;


  cv::Mat img2;
  auto start_time2 = std::chrono::high_resolution_clock::now();
  img.convertTo(img2, CV_64F, 1.0 / 255, 0);
  cv::pow(img2, 2.2, img2);
  img2.convertTo(img2, CV_8UC1, 255);
  auto end_time2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration2 = end_time2 - start_time2;
  std::cout << "Duration2 " << duration2.count() << "s" << std::endl;


  cv::Mat img3;
  img.convertTo(img3, CV_64F, 1.0 / 255, 0);
  auto start_time3 = std::chrono::high_resolution_clock::now();
  for (int x = 0; x < img3.rows; x++) {
      for (int y = 0; y < img3.cols / 3; y++) {
          for (int k = 0; k < 3; k++) {
              img3.at<double>(x, 3 * y + k) = cv::pow(img3.at<double>(x, 3 * y + k), 2.2);
          }
      }
  }
  auto end_time3 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration3 = end_time3 - start_time3;
  std::cout << "Duration3 " << duration3.count() << "s" << std::endl;
  img3.convertTo(img3, CV_8UC1, 255);


  img.push_back(img2);
  img.push_back(img3);
  cv::imshow("lab01.png", img);
  cv::imwrite("lab01.png", img);
  cv::waitKey(0);
}
