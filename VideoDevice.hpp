#ifndef VideoDevice_hpp
#define VideoDevice_hpp

class VideoDevice {
public:
  // Methods
  VideoDevice(Constants /* Constants List */);
  void startCapture(int /* Device ID */, int /* Is HD */);
  cv::Mat getImage();
  ~VideoDevice();

private:
  // Methods
  void initCamera(int /* Device ID */, int /* Is HD */);
  void captureFromCamera();

  // Data
  Constants constList;
  std::thread *captureThread;
  cv::VideoCapture camera;
  cv::Mat image;
  int isFinished;
  int isReady;
};

#endif /* VideoDevice_hpp */
