#ifndef VideoClient_hpp
#define VideoClient_hpp

class VideoClient {
public:
  // Methods
  void initClient();
  cv::Mat getCameraImage();
  ~VideoClient();

  // Data
private:
  // Methods
  void connectToSharedMemory();
  void disconnectFromSharedMemory();

  // Data
  VideoData *videoData;
  int shmid;
};

#endif /* VideoClient_hpp */
