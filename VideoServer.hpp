#ifndef VideoServer_hpp
#define VideoServer_hpp

class VideoServer {
public:
  // Methods
  VideoServer(Constants /* Constant List */);
  void initServer(int /* Device ID */, bool /* Is HD */);

  // Data
private:
  // Methods
  void createSharedMemory();
  void destroySharedMemory();
  void initCamera(int /* Device ID */, bool /* Is HD */);
  void runCamera();
  void readCameraImage();

  // Data
  VideoData *videoData;
  int shmid;
  cv::VideoCapture camera;
  Constants constList;
};

#endif /* VideoServer_hpp */
