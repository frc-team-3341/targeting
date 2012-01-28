#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
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
  time_t startTime=time(NULL);
  int lowerthresh=220;
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
      cerr <<"Cannot open capture device." <<endl;
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
  
  for (int frameCount=1;; frameCount++) {
    Mat img;
    if (isFile)
      img=imread(argv[1]); // Load Image from File
    else
      cap >>img; // Load Image from Video Capture Device
    //cvtColor(img, img, CV_RGB2HSV); // Convert Image to HSV
    Mat img_gray;
    cvtColor(img, img_gray, CV_RGB2GRAY); // Convert Image to Grayscale
    Mat img_gray_thresh;
    Mat img_gray_canny;
    Mat img_gray_corners;
    vector<Point2f> img_gray_cornerPoints;
    vector<Mat> img_rgb_planes;
    vector<Mat> img_rgb_planes_thresh;
    vector<Mat> img_rgb_planes_canny;
    vector<Mat> img_rgb_planes_corners;
    vector< vector<Point2f> > img_rgb_planes_cornerPoints;
    
    split(img, img_rgb_planes); // Split Image into RGB
    img_rgb_planes_thresh.resize(img_rgb_planes.size()); // Allocate Memory to Threshold Output Vector
    img_rgb_planes_canny.resize(img_rgb_planes.size()); // Allocate Memory to Canny Output Vector
    img_rgb_planes_cornerPoints.resize(img_rgb_planes.size()); // Allocate Memory to Corner Detection Output Vector

    // Run Thresholding
    int thresholdType=CV_THRESH_BINARY;
    threshold(img_gray, img_gray_thresh, lowerthresh, upperthresh, thresholdType);
    threshold(img_rgb_planes.at(0), img_rgb_planes_thresh.at(0), lowerthresh, upperthresh, thresholdType);
    threshold(img_rgb_planes.at(1), img_rgb_planes_thresh.at(1), lowerthresh, upperthresh, thresholdType);
    threshold(img_rgb_planes.at(2), img_rgb_planes_thresh.at(2), lowerthresh, upperthresh, thresholdType);
    

    // Run Canny
    int apertureSize=7;
    Canny(img_gray_thresh, img_gray_canny, lowerthresh, upperthresh, apertureSize);
    Canny(img_rgb_planes_thresh.at(0), img_rgb_planes_canny.at(0), lowerthresh, upperthresh, apertureSize);
    Canny(img_rgb_planes_thresh.at(1), img_rgb_planes_canny.at(1), lowerthresh, upperthresh, apertureSize);
    Canny(img_rgb_planes_thresh.at(2), img_rgb_planes_canny.at(2), lowerthresh, upperthresh, apertureSize);
    //dilate(img_gray_canny, img_gray_canny, Mat(), Point(-1,-1)); 

    // Run Corner Detection
    // Reference: http://www.moosechips.com/2008/08/opencv-corner-detection-using-cvgoodfeaturestotrack/
    int maxCorners=10;
    double qualityLevel=0.1;
    int minDistance=10;
    goodFeaturesToTrack(img_gray_thresh, img_gray_cornerPoints, maxCorners, qualityLevel, minDistance);
    goodFeaturesToTrack(img_rgb_planes_thresh.at(0), img_rgb_planes_cornerPoints.at(0), maxCorners, qualityLevel, minDistance);
    goodFeaturesToTrack(img_rgb_planes_thresh.at(1), img_rgb_planes_cornerPoints.at(1), maxCorners, qualityLevel, minDistance);
    goodFeaturesToTrack(img_rgb_planes_thresh.at(2), img_rgb_planes_cornerPoints.at(2), maxCorners, qualityLevel, minDistance); 

    // Print Coordinates of Corners
    for (unsigned i=0; i < img_rgb_planes_cornerPoints.at(1).size(); i++) {
      cout <<"0: " <<i <<": " <<img_rgb_planes_cornerPoints.at(1).at(i) <<endl;
    }
    for (unsigned i=0; i < img_rgb_planes_cornerPoints.size(); i++) {
      for (unsigned j=0; j < img_rgb_planes_cornerPoints.at(i).size(); j++)
	cout <<i+1 <<": " <<j <<": " <<img_rgb_planes_cornerPoints.at(i).at(j) <<endl;
    }

    // Create Corner Detection Output Images
    img_gray_corners=img_gray_canny.clone();
    for (unsigned int i=0; i < img_rgb_planes_canny.size(); i++)
      img_rgb_planes_corners.push_back(img_rgb_planes_canny.at(i).clone());

    // Write Corners to Output Image
    for (unsigned i=0; i < img_rgb_planes_cornerPoints.at(1).size(); i++) {
      int radius=10;
      int pointX=img_rgb_planes_cornerPoints.at(1).at(i).x;
      int pointY=img_rgb_planes_cornerPoints.at(1).at(i).y;
      Point cornerCoordinates(pointX, pointY);
      Scalar circleColor(255.0, 255.0, 255.0, 0.0);
      int circleThickness=-1; // Filled Circle
      circle(img_rgb_planes_corners.at(1), cornerCoordinates, radius, circleColor, circleThickness);
    }

    // Display Images
    imshow("Original", img);
    //imshow("Grayscale", img_gray);
    //imshow("Blue Channel", img_rgb_planes.at(0));
    //imshow("Green Channel", img_rgb_planes.at(1));
    //imshow("Red Channel", img_rgb_planes.at(2));
    //imshow("Thresholded Grayscale", img_gray_thresh);
    //imshow("Canny Grayscale", img_gray_canny);
    //imshow("Corners Grayscale", img_gray_corners);
    //imshow("Canny Blue Channel", img_rgb_planes_canny.at(0));
    imshow("Thresholded Green Channel", img_rgb_planes_thresh.at(1));
    imshow("Canny Green Channel", img_rgb_planes_canny.at(1));
    imshow("Corners Green Channel", img_rgb_planes_corners.at(1));
    //imshow("Canny Red Channel", img_rgb_planes_canny.at(2));
 
    cout <<"FPS: " <<frameCount/(time(NULL)-startTime) <<endl;

    if ((waitKey(10) & 255) == 27)
      break;
    if (isFile) {
      waitKey();
      break;
    }
  }
  
  return EXIT_SUCCESS;
}
