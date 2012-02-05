#ifndef hsvimage_hpp
#define hsvimage_hpp

class HSVImage {
public:
  // Functions
  HSVImage(cv::Mat &); // Constructor
  void LoadRGBImage(cv::Mat &); // Loads RGB Image

  // Variables
  cv::Mat rgb;
  cv::Mat hsv;
  cv::Mat hue;
  cv::Mat saturation;
  cv::Mat value;
private:
};

#endif /* hsvimage_hpp */
