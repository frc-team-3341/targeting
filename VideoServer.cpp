#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>

#include "Constants.hpp"
#include "VideoData.hpp"
#include "VideoServer.hpp"

using namespace cv;
using namespace std;

VideoServer::VideoServer(int deviceID, bool isHD, Constants inputConstList) {
  signal(SIGCHLD, SIG_IGN); // Ignore Process Death

  if (!fork()) {
    
    exit(0);
  }
}

void VideoServer::createSharedMemory() {
  // Variable Declarations
  key_t key;
  int shmid;

  if ((key = ftok("rectangledetector", 'R')) == -1) // Create Key
    exit(1);

  if ((shmid = shmget(key, 1048576, 0644 | IPC_CREAT)) == -1) // Create Memory Segment
    exit(1);

  videoData = (VideoData *)shmat(shmid, (void *)0, 0);
  if (videoData == (VideoData *)(-1))
    exit(1);
}

void VideoServer::destroySharedMemory() {
  if (shmdt(videoData) == -1) {
    exit(1);
  }
}

void VideoServer::initCamera(int deviceID, bool isHD) {
  // Get Video Capture Device
  camera.open(deviceID);
  if (!camera.isOpened()) {
	  cerr <<"Unable to open capture device " <<deviceID <<"." <<endl;
	  exit(-1);
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

void VideoServer::getCameraImage() {
  // Variable Declarations
  Mat image;
  camera >>image;

  videoData->modifyImage(image);
}
