#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

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
  int lowerthresh=190;
  int upperthresh=255;
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
  //namedWindow("Grayscale", 0);
  //namedWindow("Blue Channel", 0);
  //namedWindow("Green Channel", 0);
  //namedWindow("Red Channel", 0);
  //namedWindow("Canny Grayscale", 0);
  //namedWindow("Corners Grayscale", 0);
  //namedWindow("Canny Blue Channel", 0);
  namedWindow("Thresholded Green Channel", 0);
  namedWindow("Canny Green Channel", 0);
  namedWindow("Corners Green Channel", 0);
  //namedWindow("Canny Red Channel", 0);
  
  double secondsPerFrame=0.03;
  double IIRFilterConstant=0.02;

  for (int frameCount=1;; ++frameCount) {
    time_t startTime=time(NULL);
    Mat img;
    if (isFile)
      img=imread(argv[1]); // Load Image from File
    else
      cap >>img; // Load Image from Video Capture Device
    //cvtColor(img, img, CV_RGB2HSV); // Convert Image to HSV
    vector<Mat> img_planes; // Order: Blue, Green, Red, Gray
    vector<Mat> img_planes_thresh;
    vector<Mat> img_planes_canny;
    vector<Mat> img_planes_corners;
    vector< vector<Point2f> > img_planes_cornerPoints;
    
    split(img, img_planes); // Split Image into RGB
    img_planes.resize(img_planes.size() + 1); // Allocate Memory
    cvtColor(img, img_planes.at(3), CV_RGB2GRAY); // Convert Image to Grayscale

    // Allocate Memory to Vectors
    img_planes_thresh.resize(img_planes.size());
    img_planes_canny.resize(img_planes.size());
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
	int pointX=img_planes_cornerPoints.at(1).at(i).x;
	int pointY=img_planes_cornerPoints.at(1).at(i).y;
	Point cornerCoordinates(pointX, pointY);
	Scalar circleColor(255.0, 255.0, 255.0, 0.0);
	int circleThickness=-1; // Filled Circle
	circle(img_planes_corners.at(i), cornerCoordinates, radius, circleColor, circleThickness);
      }
    }

    // Write FPS to Original Image
    int fpsPointX=0;
    int fpsPointY=img.rows-5;
    Point fpsCoordinates(fpsPointX, fpsPointY);
    int fontFace=FONT_HERSHEY_COMPLEX;
    double fontScale=img.rows/960;
    Scalar fpsColor(255.0, 255.0, 255.0, 0.0);
    secondsPerFrame=IIRFilterConstant*(time(NULL)-startTime)+(1-IIRFilterConstant)*secondsPerFrame;
    stringstream fps;
    fps <<setprecision(2) <<fixed <<"FPS: " <<1/secondsPerFrame;
    putText(img, fps.str(), fpsCoordinates, fontFace, fontScale, fpsColor);

    // Display Images
    imshow("Original", img);
    // Blues
    //imshow("Blue Channel", img_planes.at(0));
    //imshow("Thresholded Blue Channel", img_planes_thresh.at(0));
    //imshow("Canny Blue Channel", img_planes_canny.at(0));
    //imshow("Corners Blue Channel", img_planes_canny.at(0));
    // Greens
    //imshow("Green Channel", img_planes.at(1));
    imshow("Thresholded Green Channel", img_planes_thresh.at(1));
    imshow("Canny Green Channel", img_planes_canny.at(1));
    imshow("Corners Green Channel", img_planes_corners.at(1));
    // Reds
    //imshow("Red Channel", img_planes.at(2));
    //imshow("Thresholded Red Channel", img_planes_thresh.at(2));
    //imshow("Canny Red Channel", img_planes_canny.at(2));
    //imshow("Corners Red Channel", img_planes_canny.at(2));
    // Grayscales
    //imshow("Grayscale", img_planes.at(3));
    //imshow("Thresholded Grayscale", img_planes_thresh.at(3));
    //imshow("Canny Grayscale", img_planes_canny.at(3));
    //imshow("Corners Grayscale", img_planes_corners.at(3));

    if ((waitKey(10) & 255) == 27)
      break;
    if (isFile) {
      waitKey();
      break;
    }
  }
  
  return EXIT_SUCCESS;
}
