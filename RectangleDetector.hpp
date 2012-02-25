#ifndef RectangleDetector_hpp
#define RectangleDetector_hpp

class RectangleDetector {
public:
  // Methods
  RectangleDetector(cv::Mat /* Input Image */);
  bool rectangleWasFound();
  float getAzimuth();
  int getDistance();
  //int getHeight();
  //float getTilt();
  std::vector< std::vector<cv::Point> > getAllRectangles();
  std::vector< std::vector<cv::Point> > getFinalRectangles();
private:
  // Data
  Constants constList;
  cv::Mat image;
  std::vector< std::vector<cv::Point> > allRectangles;
  std::vector< std::vector<cv::Point> > finalRectangles;
  std::vector<Rectangle> rectList;
  std::vector<Rectangle> rectListRevised;
  bool foundRectangle;
  int rectIndex;
  float azimuth;
  int distance;
  //int height;
  //float tilt;

  // Methods
  double angle(cv::Point, cv::Point, cv::Point); // Find Cosine of Angle Between Vectors
  void preprocessImage(); // Threshold and Grayscale
  void findRectangles(); // Finds Rectangles
  void populateRectangles(); // Populates Vector of Rectangles
  bool rectangleIsContained(Rectangle, Rectangle); // Check if Rectangle is Contained
  void findContainedRectangles(); // Finds Contained Rectangles
  void findCorrectRectangles(); // Figures Out Which Rectangles Are Correct
  void computeDistance(); // Computes Distance to the Rectangle
  //void computeHeight(); // Computes Height of the Rectangle
  void computeAzimuth(); // Computes Azimuth to the Rectangle
  //void computeTilt(); // Computes Tilt of the Rectangle
};

#endif /* RectangleDetector_hpp */
