#ifndef Rectangle_hpp
#define Rectangle_hpp

class Rectangle {
public:
  // Methods
  Rectangle(std::vector<cv::Point>); // Populate Rectangle
  bool containsPoint(cv::Point); // Checks if Point is Inside Rectangle

  // Data
  cv::Point topLeft; // Top Left Corner
  cv::Point topRight; // Top Right Corner
  cv::Point bottomRight; // Bottom Right Corner
  cv::Point bottomLeft; // Bottom Left Corner
  cv::Point center; // Center Point
  int lengthSquaredLeft; // Square of Length of Left Side
  int lengthSquaredRight; // Square of Length of Right Side
  int lengthSquaredTop; // Square of Length of Top Side
  int lengthSquaredBottom; // Square of Length of Bottom Side
  std::vector<int> containedRectangles; // Indices of Contained Rectangles
  
private:
  // Methods

  // Data
};

#endif /* Rectangle_hpp */
