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

VideoServer::VideoServer(Constants inputConstList) {
  constList = inputConstList; // Save Constants List to Class
}

void VideoServer::initServer(int deviceID, bool isHD) {
  signal(SIGCHLD, SIG_IGN); // Ignore Process Death

  if (!fork()) {
    initCamera(deviceID, isHD);
    createSharedMemory();
    runCamera();
    destroySharedMemory();
    exit(0);
  }
}

void VideoServer::createSharedMemory() {
  // Variable Declarations
  key_t key;

  // Create Key
  if ((key = ftok("rectangledetector", 'R')) == -1)
    exit(1);

  // Create Memory Segment
  if ((shmid = shmget(key, 1048576, 0666 | IPC_CREAT)) == -1)
    exit(1);

  // Get Pointer to Memory Segment
  videoData = (VideoData *)shmat(shmid, (void *)0, 0);
  if (videoData == (VideoData *)(-1))
    exit(1);
}

void VideoServer::destroySharedMemory() {
  if (shmdt(videoData) == -1)
    exit(1);

  shmctl(shmid, IPC_RMID, NULL);
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

void VideoServer::runCamera() {
  cout <<"server: running camera" <<endl;
  while (! videoData->getIsFinished()) {
    cout <<"server: getting frame" <<endl;
    readCameraImage();
  }
}
  
void VideoServer::readCameraImage() {
  // Variable Declarations
  Mat image;
  camera >>image;

  cout <<"server: grabbing frame" <<endl;

  imshow("test", image);

  videoData->modifyImage(image);

  cout <<"server: grabbed frame" <<endl;
}
