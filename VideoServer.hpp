#ifndef VideoServer_hpp
#define VideoServer_hpp

class VideoServer {
public:
  // Methods
  VideoServer(int /* Device ID */, bool /* Is HD */, Constants /* Constant List */);

  // Data
private:
  // Methods
  void createSharedMemory();
  void destroySharedMemory();
  void initCamera(int /* Device ID */, bool /* Is HD */);
  void getCameraImage();

  // Data
  VideoData *videoData;
  cv::VideoCapture camera;
  Constants constList;
};

#endif /* VideoServer_hpp */
