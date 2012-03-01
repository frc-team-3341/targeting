#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>

#include "Constants.hpp"
#include "VideoData.hpp"

using namespace cv;
using namespace std;

VideoData::VideoData() {
  // Variable Initializations
  isFinished = false;
}

Mat VideoData::getImage() {
  return image;
}

void VideoData::modifyImage(Mat input) {
  input.copyTo(image);
}

bool VideoData::getIsFinished() {
  return isFinished;
}

void VideoData::toggleIsFinished() {
  if (isFinished)
    isFinished = false;
  if (isFinished)
    isFinished = true;
}
