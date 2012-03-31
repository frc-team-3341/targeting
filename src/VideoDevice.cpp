/*
 *    This file is part of FRC Team 3341 Aimer.
 *
 *    FRC Team 3341 Aimer is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    FRC Team 3341 Aimer is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with FRC Team 3341 Aimer.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

#include "Constants.hpp"
#include "VideoDevice.hpp"

using namespace cv;
using namespace std;

// Public Methods
VideoDevice::VideoDevice(Constants *inputConstList) {
  // Variable Declarations
  isFinished = 0;
  isReady = 0;
  constList = *inputConstList;
}

void VideoDevice::startCapture(int deviceID) {
  initCamera(deviceID);
  captureThread = new thread(&VideoDevice::captureFromCamera, this);
}

Mat VideoDevice::getImage() {
  while (! isReady)
      cout <<"Waiting for camera..." <<endl;
  return image;
}

// Private Methods
void VideoDevice::initCamera(int deviceID) {
  // Get Video Capture Device
  camera.open(deviceID);
  if (!camera.isOpened()) {
    cerr <<"Unable to open capture device " <<deviceID <<"." <<endl;
    exit(1);
  }
}

void VideoDevice::captureFromCamera() {
  while (! isFinished) {
    camera >>image;
    isReady = 1;
    constList.imgCols = image.cols;
    constList.imgRows = image.rows;
  }
}

VideoDevice::~VideoDevice() {
  isFinished = 1;
  captureThread->join();
  delete captureThread;
}
