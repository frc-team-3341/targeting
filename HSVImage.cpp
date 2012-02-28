#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "HSVImage.hpp"

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <ctime>

using namespace cv;
using namespace std;

HSVImage::HSVImage(Mat &InputImage) {
  LoadRGBImage(InputImage);
}

void HSVImage::LoadRGBImage(Mat &InputImage) {
  InputImage.copyTo(rgb);
  cvtColor(InputImage, InputImage, CV_RGB2HSV); // Convert Image to HSV
  
  // Get HSV Slices
  vector<Mat> slices;
  split(InputImage, slices);
  InputImage.copyTo(hsv);
  slices.at(0).copyTo(hue);
  slices.at(1).copyTo(saturation);
  slices.at(2).copyTo(value);
}
