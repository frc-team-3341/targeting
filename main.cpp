#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <cmath>
#include <cstring>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>
#include <set>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "Constants.hpp"
#include "Rectangle.hpp"
#include "RectangleDetector.hpp"
#include "CommLink.hpp"

using namespace cv;
using namespace std;

const char* wndname = "Rectangle Detector";

// the function draws all the rectangles in the image
void drawRectangles(Mat& image, const vector< vector<Point> > &allRectangles, const vector< vector<Point> >& finalRectangles)
{
  for (size_t i = 0; i < allRectangles.size(); i++) {
    const Point* p = &allRectangles.at(i).at(0);
    int n = (int)allRectangles.at(i).size();
    polylines(image, &p, &n, 1, true, Scalar(0, 255, 0), 3, CV_AA);
  }

  for (size_t i = 0; i < finalRectangles.size(); ++i) {
    const Point* p=&finalRectangles.at(i).at(0);
    int n=(int)finalRectangles.at(i).size();
    polylines(image, &p, &n, 1, true, Scalar(255, 0, 0), 3, CV_AA);
  }

  imshow(wndname, image);
}

int main(int argc, char* argv[])
{
  namedWindow(wndname, 0);
  CommLink commLink;
  Constants constList;
  VideoCapture cap;
  bool isFile = false;
  bool isHD = false;
  bool firstRun = true;

  // Check Arguments
  if (string(argv[1]) == "-f")
    isFile = true;
  else if (string(argv[1]) == "-hd")
    isHD = true;
      
  if (!isFile) {
    // Get Video Capture Device
    cap.open(atoi(argv[2]));
    if (!cap.isOpened()) {
      cerr <<"Unable to open capture device " <<argv[2] <<"." <<endl;
      return -1;
    }
    if (isHD) {
      // Set Capture Resolution
      cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1920);
      cap.set(CV_CAP_PROP_FRAME_WIDTH, 1080);

      // Change Camera Data
      constList.cameraFocalLength = constList.cameraHDFocalLength;
      constList.cameraViewingAngle = constList.cameraHDViewingAngle;
    }
  }
    
  while (true)
    {
      vector< vector<Point> > allRectangles;
      vector< vector<Point> > finalRectangles;
      bool aquired = false;
      int distanceMM;
      float azimuthRadians;
      float azimuthDegrees;
      Mat original;
      Mat output;

      commLink.waitForPing();

      if (isFile)
	original = imread(argv[2]); // Load Image from File
      else
	cap >>original; // Load Image from Video Capture Device

      // Set Variables
      if (firstRun)
	cout <<"Camera Resolution: " <<original.cols <<"x" <<original.rows <<endl;

      // Get Distance and Azimuth
      RectangleDetector rectDetector(original);
      if (rectDetector.rectangleWasFound()) {
	aquired = true;
	
	// Retrieve Data
	distanceMM = rectDetector.getDistance();
	azimuthRadians = rectDetector.getAzimuth();
	allRectangles = rectDetector.getAllRectangles();
	finalRectangles = rectDetector.getFinalRectangles();

	// Convert Data
	azimuthDegrees = (azimuthRadians * 180.0) / constList.mathPi;
	
	// Print Data
	cout <<"Distance: " <<distanceMM <<"mm" <<endl;
	cout <<"Azimuth: " <<azimuthDegrees <<" degrees, " <<azimuthRadians <<" radians" <<endl;

	// Send Data
	commLink.sendData(distanceMM, 0, azimuthRadians, 0.0);
      }
      else
	cout <<"No rectangle" <<endl;

      // Write Data to Original Image
      int dataPointX = 0;
      int dataPointY = original.rows-5;
      Point dataCoordinates(dataPointX, dataPointY);
      int fontFace = FONT_HERSHEY_COMPLEX;
      double fontScale = (float)original.cols / 400.0;
      Scalar fontColor(0.0, 255.0, 0.0, 0.0);
      int fontThickness = 2;
      stringstream data;
      if (aquired)
	data <<distanceMM <<"mm @ " <<azimuthDegrees <<" degrees";
      else
	data <<"No rectangle";
      putText(original, data.str(), dataCoordinates, fontFace, fontScale, fontColor, fontThickness);

      drawRectangles(original, allRectangles, finalRectangles); // Draw Rectangles and Display Image

      firstRun = false;

      int keycode = waitKey(10);
      if (keycode == 120)
	{
	  stringstream filename;
	  filename <<time(NULL) <<".jpg";
	  imwrite(filename.str().c_str(), output);
	}
      else if (keycode == 27)
	break;
      if (isFile)
	{
	  waitKey();
	  break;
	}
    }

  return 0;
}
