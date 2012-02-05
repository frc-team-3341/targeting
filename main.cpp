#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "HSVImage.hpp"

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <ctime>

using namespace cv;
using namespace std;

void printUsage() {
  cout <<"Usage: ./rectangledetector.bin [input device number/filename]" <<endl;
}

int main(int argc, char* argv[]) {
  // Check Arguments
  if (argc!=2) {
    printUsage();
    return -1;
  }

  // Variable Declarations
  int lowerthresh=50;
  int upperthresh=200;
  VideoCapture cap;
  bool isFile=false;

  // Check Argument Type
  if (isalpha(argv[1][0]))
    isFile=true;
      
  if (!isFile) {
    // Get Video Capture Device
    cap.open(atoi(argv[1]));
    if (!cap.isOpened()) {
      cerr <<"Unable to open capture device " <<argv[1] <<"." <<endl;
      return -1;
    }
  }
  
  // Create GUI
  namedWindow("Original", 0);
  namedWindow("HSV Image", 0);
  namedWindow("Hue", 0);
  namedWindow("Saturation", 0);
  namedWindow("Lower Saturation", 0);
  namedWindow("Upper Saturation", 0);
  namedWindow("Saturation Single Function", 0);
  namedWindow("Value", 0);

  double secondsPerFrame=0.03;
  double IIRFilterConstant=0.02;

  for (int frameCount=1;; ++frameCount) {
    time_t startTime=time(NULL);
    Mat img;
    if (isFile)
      img=imread(argv[1]); // Load Image from File
    else
      cap >>img; // Load Image from Video Capture Device

    // Image Containers
    HSVImage img_hsv(img);
    HSVImage img_hsv_thresh(img_hsv);
    HSVImage img_hsv_canny(img_hsv);
    HSVImage img_hsv_corners(img_hsv);
    vector< vector<Point2f> > img_planes_cornerPoints;
    Mat img_saturationUpper;
    Mat img_saturationLower;
    Mat img_saturationSingleFunction;

    threshold(img_hsv.saturation, img_saturationLower, lowerthresh, 255, CV_THRESH_BINARY);
    threshold(img_hsv.saturation, img_saturationUpper, upperthresh, 255, CV_THRESH_BINARY_INV);
    img_hsv_thresh.saturation=img_saturationLower & img_saturationUpper;
    threshold(img_hsv.saturation, img_saturationSingleFunction, lowerthresh, upperthresh, CV_THRESH_BINARY);

    /* OLD CODE
    // Allocate Memory to Vectors
    img_planes_cornerPoints.resize(img_planes.size());

    // Run Thresholding
    int thresholdType=CV_THRESH_BINARY;
    for (unsigned i=0; i < img_planes.size(); ++i)
      threshold(img_planes.at(i), img_planes_thresh.at(i), lowerthresh, upperthresh, thresholdType);
    
    // Run Canny
    int apertureSize=7;
    for (unsigned i=0; i < img_planes_thresh.size(); ++i)
      Canny(img_planes_thresh.at(i), img_planes_canny.at(i), lowerthresh, upperthresh, apertureSize);

    // Run Corner Detection
    // Reference: http://www.moosechips.com/2008/08/opencv-corner-detection-using-cvgoodfeaturestotrack/
    int maxCorners=30;
    double qualityLevel=0.1;
    int minDistance=100;
    for (unsigned i=0; i < img_planes_thresh.size(); ++i)
      goodFeaturesToTrack(img_planes_thresh.at(i), img_planes_cornerPoints.at(i), maxCorners, qualityLevel, minDistance);

    // Create Corner Detection Output Images
    for (unsigned i=0; i < img_planes_thresh.size(); ++i)
      img_planes_corners.push_back(img_planes_thresh.at(i).clone());

    // Write Corners to Output Image
    for (unsigned i=0; i < img_planes_cornerPoints.size(); ++i) {
      for (unsigned j=0; j < img_planes_cornerPoints.at(i).size(); ++j) {
	int radius=img.rows/25;
	int pointX=img_planes_cornerPoints.at(i).at(j).x;
	int pointY=img_planes_cornerPoints.at(i).at(j).y;
	Point cornerCoordinates(pointX, pointY);
	Scalar circleColor(255.0, 255.0, 255.0, 0.0);
	int circleThickness=-1; // Filled Circle
	circle(img_planes_corners.at(i), cornerCoordinates, radius, circleColor, circleThickness);
      }
    }
    */

    // Write FPS to Original Image
    int fpsPointX=0;
    int fpsPointY=img.rows-5;
    Point fpsCoordinates(fpsPointX, fpsPointY);
    int fontFace=FONT_HERSHEY_COMPLEX;
    double fontScale=img.rows/320.0;
    Scalar fpsColor(255.0, 255.0, 255.0, 0.0);
    secondsPerFrame=IIRFilterConstant*(time(NULL)-startTime)+(1-IIRFilterConstant)*secondsPerFrame;
    stringstream fps;
    fps <<setprecision(2) <<"FPS: " <<fixed <<1/secondsPerFrame;
    putText(img_hsv.rgb, fps.str(), fpsCoordinates, fontFace, fontScale, fpsColor);

    // Display Images
    imshow("Original", img_hsv.rgb);
    imshow("HSV Image", img_hsv.hsv);
    imshow("Hue", img_hsv.hue);
    imshow("Lower Saturation", img_saturationLower);
    imshow("Upper Saturation", img_saturationUpper);
    imshow("Saturation", img_hsv.saturation);
    imshow("Saturation Single Function", img_saturationSingleFunction);
    imshow("Value", img_hsv.value);
 
    if ((waitKey(10) & 255) == 27)
      break;
    if (isFile) {
      waitKey();
      break;
    }
  }
  
  return EXIT_SUCCESS;
}
