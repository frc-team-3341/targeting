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
  int hueLowerThresh;
  int hueUpperThresh;
  int valueLowerThresh;
  int valueUpperThresh;
  VideoCapture cap;
  bool isFile=false;

  // Get Threshold Values
  cout <<"Hue Lower Threshold: ";
  cin >>hueLowerThresh;
  cout <<"Hue Upper Threshold: ";
  cin >>hueUpperThresh;
  cout <<"Value Lower Threshold: ";
  cin >>valueLowerThresh;
  cout <<"Value Upper Threshold: ";
  cin >>valueUpperThresh;

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
  namedWindow("Hue Thresh", 0);
  namedWindow("Lower Hue", 0);
  namedWindow("Upper Hue", 0);
  namedWindow("Saturation Thresh", 0);
  namedWindow("Value Thresh", 0);
  namedWindow("Hue+Value Thresh", 0);

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
    vector< vector<Point> > img_contours;
    Mat img_saturationUpper;
    Mat img_saturationLower;
    Mat img_hueUpper;
    Mat img_hueLower;
    Mat img_valueUpper;
    Mat img_valueLower;
    Mat img_thresh_hueVal;
    Mat img_valContours;

    // Thresholding
    threshold(img_hsv.hue, img_hueLower, hueLowerThresh, 255, CV_THRESH_BINARY);
    threshold(img_hsv.hue, img_hueUpper, hueUpperThresh, 255, CV_THRESH_BINARY_INV);
    img_hsv_thresh.hue=img_hueLower & img_hueUpper;

    threshold(img_hsv.saturation, img_saturationLower, hueLowerThresh, 255, CV_THRESH_BINARY);
    threshold(img_hsv.saturation, img_saturationUpper, hueUpperThresh, 255, CV_THRESH_BINARY_INV);
    img_hsv_thresh.saturation=img_saturationLower & img_saturationUpper;

    threshold(img_hsv.value, img_valueLower, valueLowerThresh, 255, CV_THRESH_BINARY);
    threshold(img_hsv.value, img_valueUpper, valueUpperThresh, 255, CV_THRESH_BINARY_INV);
    img_hsv_thresh.value=img_valueLower & img_valueUpper;

    img_thresh_hueVal=img_hsv_thresh.hue & img_hsv_thresh.value;

    // Contours
    Scalar contourColor(255.0, 0.0, 255.0, 0.0);
    findContours(img_hsv_thresh.value, img_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    cout <<"found" <<endl;
    drawContours(img_hsv_thresh.value, img_contours, -1, contourColor, CV_FILLED);
    cout <<"drew" <<endl;
    for (unsigned i=0; i<img_contours.size(); ++i) {
      for (unsigned j=0; j<img_contours.at(i).size(); ++j)
	cout <<img_contours.at(i).at(j) <<endl;
    }

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
    imshow("Hue Thresh", img_hsv_thresh.hue);
    imshow("Lower Hue", img_hueLower);
    imshow("Upper Hue", img_hueUpper);
    imshow("Saturation Thresh", img_hsv_thresh.saturation);
    imshow("Value Thresh", img_hsv_thresh.value);
    imshow("Hue+Value Thresh", img_thresh_hueVal);
 
    if ((waitKey(10) & 255) == 27)
      break;
    if (isFile) {
      waitKey();
      break;
    }
  }
  
  return EXIT_SUCCESS;
}
