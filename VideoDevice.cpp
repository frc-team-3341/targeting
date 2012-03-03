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
  while (! isReady);
  isReady = 0;
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
    camera >>image;
    isReady = 1;
  }
}

VideoDevice::~VideoDevice() {
  isFinished = 1;
  captureThread->join();
  delete captureThread;
}
