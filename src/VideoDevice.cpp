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
VideoDevice::VideoDevice(Constants inputConstList) {
  // Variable Declarations
  isFinished = 0;
  isReady = 0;
  constList = inputConstList; // Save Constant List to Class
}

void VideoDevice::startCapture(int deviceID, int isHD) {
  initCamera(deviceID, isHD);
  captureThread = new thread(&VideoDevice::captureFromCamera, this);
}

Mat VideoDevice::getImage() {
  cout <<"Preparing Image" <<endl;
  while (! isReady)
    cout <<"Waiting For Camera..." <<endl;
  //isReady = 0;
  cout <<"Returning Image" <<endl;
  return image;
}

// Private Methods
void VideoDevice::initCamera(int deviceID, int isHD) {
  // Get Video Capture Device
  camera.open(deviceID);
  if (!camera.isOpened()) {
    cerr <<"Unable to open capture device " <<deviceID <<"." <<endl;
    exit(1);
  }
  if (isHD) {
    // Set Capture Resolution
    camera.set(CV_CAP_PROP_FRAME_HEIGHT, 1920);
    camera.set(CV_CAP_PROP_FRAME_WIDTH, 1080);
	  
    // Change Camera Data
    constList.cameraFocalLength = constList.cameraHDFocalLength;
    constList.cameraViewingAngle = constList.cameraHDViewingAngle;
  }
}

void VideoDevice::captureFromCamera() {
  while (! isFinished) {
    //isReady = 0;
    cout <<"Capturing Image" <<endl;
    camera >>image;
    isReady = 1;
    cout <<"Storing Image" <<endl;
  }
  cout <<"Exitting" <<endl;
}

VideoDevice::~VideoDevice() {
  isFinished = 1;
  captureThread->join();
  delete captureThread;
}
