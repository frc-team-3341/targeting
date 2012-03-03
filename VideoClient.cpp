#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#include "Constants.hpp"
#include "VideoData.hpp"
#include "VideoClient.hpp"

using namespace cv;
using namespace std;

void VideoClient::initClient() {
  connectToSharedMemory();
}

cv::Mat VideoClient::getCameraImage() {  
  return videoData->getImage();
}

void VideoClient::connectToSharedMemory() {
  // Variable Declarations
  key_t key;

  // Create Key
  if ((key = ftok("rectangledetector", 'R')) == -1)
    exit(1);

  // Access Memory Segment
  if ((shmid = shmget(key, 10485760, IPC_CREAT | 0666)) == -1)
    exit(1);

  // Get Pointer to Memory Segment
  videoData = (VideoData *)shmat(shmid, (void*)0, 0);
  if (videoData == (void *)(-1))
    exit(1);
}

void VideoClient::disconnectFromSharedMemory() {
  if (shmdt(videoData) == -1)
    exit(1);
}

VideoClient::~VideoClient() {
  videoData->toggleIsFinished();
  disconnectFromSharedMemory();
}
