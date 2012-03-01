#ifndef VideoData_hpp
#define VideoData_hpp

class VideoData {
public:
  // Methods
  VideoData();
  cv::Mat getImage();
  void modifyImage(cv::Mat /* Modified Image */);
  bool getIsFinished();
  void toggleIsFinished();

  // Data
private:
  // Methods

  // Data
  cv::Mat image;
  bool isFinished;
};

#endif /* VideoData_hpp */
