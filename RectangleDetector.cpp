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

#include "Constants.hpp"
#include "HSVImage.hpp"
#include "Rectangle.hpp"
#include "RectangleDetector.hpp"

using namespace cv;
using namespace std;

int thresh = 50;
int N = 11;

// Public Functions
RectangleDetector::RectangleDetector(Mat input) {
  // Variable Initializations
  input.copyTo(image);
  foundRectangle=true;
  rectIndex=-1;

  preprocessImage();
  findRectangles();
  if (! foundRectangle) return;
  populateRectangles();
  if (! foundRectangle) return;
  findContainedRectangles();
  if (! foundRectangle) return;
  findCorrectRectangles();
  if (! foundRectangle) return;
  computeDistance();
  computeAzimuth();
}

bool RectangleDetector::rectangleWasFound() {
  return foundRectangle;
}

float RectangleDetector::getAzimuth() {
  return azimuth;
}

int RectangleDetector::getDistance() {
  return distance;
}

vector< vector<Point> > RectangleDetector::getAllRectangles() {
  return allRectangles;
}

vector< vector<Point> > RectangleDetector::getFinalRectangles() {
  return finalRectangles;
}

// Private Functions
double RectangleDetector::angle(Point pt1, Point pt2, Point pt0) {
  double dx1 = pt1.x - pt0.x;
  double dy1 = pt1.y - pt0.y;
  double dx2 = pt2.x - pt0.x;
  double dy2 = pt2.y - pt0.y;
  return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

void RectangleDetector::preprocessImage() {
  // Variable Declarations
  HSVImage imageHSV(image);
  Mat hsv_threshHueLower;
  Mat hsv_threshHueUpper;
  Mat hsv_threshValueLower;
  Mat hsv_threshValueUpper;

  // Hue Threshold
  threshold(imageHSV.hue, hsv_threshHueLower, constList.preprocessingHueLowerThreshold, 255, CV_THRESH_BINARY);
  threshold(imageHSV.hue, hsv_threshHueUpper, constList.preprocessingHueUpperThreshold, 255, CV_THRESH_BINARY_INV);
  imageHSV.hue = hsv_threshHueLower & hsv_threshHueUpper;

  // Value Threshold
  threshold(imageHSV.value, hsv_threshValueLower, constList.preprocessingValueLowerThreshold, 255, CV_THRESH_BINARY);
  threshold(imageHSV.value, hsv_threshValueUpper, constList.preprocessingValueUpperThreshold, 255, CV_THRESH_BINARY_INV);
  imageHSV.value = hsv_threshValueLower & hsv_threshValueUpper;

  image = imageHSV.hue & imageHSV.value;
}

void RectangleDetector::findRectangles() {
  // Clear Rectangle Vectors
  allRectangles.clear();
  finalRectangles.clear();

  // Variable Declarations
  Mat pyr;
  Mat timg;
  Mat gray0(image.size(), CV_8U);
  Mat gray;
  
  // down-scale and upscale the image to filter out the noise
  pyrDown(image, pyr, Size(image.cols/2, image.rows/2));
  pyrUp(pyr, timg, image.size());

  // Variable Declarations
  vector< vector<Point> > contours;
  int ch[] = {0, 0};

  mixChannels(&timg, 1, &gray0, 1, ch, 1); // Extract Channel

  // Try Several Threshold Levels
  for(int l = 0; l < N; ++l) {
    // hack: use Canny instead of zero threshold level.
    // Canny helps to catch squares with gradient shading
    if (l == 0) {
      // apply Canny. Take the upper threshold from slider
      // and set the lower to 0 (which forces edges merging)
      Canny(gray0, gray, 0, thresh, 5);
      // dilate canny output to remove potential
      // holes between edge segments
      dilate(gray, gray, Mat(), Point(-1,-1));
    }
    else {
      // apply threshold if l!=0:
      //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
      gray = gray0 >= (l+1)*255/N;
    }

    findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE); // Find Contours

    // Variable Declarations
    vector<Point> approx;

    // Test Each Contour
    for (size_t i = 0; i < contours.size(); ++i) {
      // approximate contour with accuracy proportional
      // to the contour perimeter
      approxPolyDP(Mat(contours.at(i)), approx, arcLength(Mat(contours.at(i)), true)*0.02, true);
          
      // rectangular contours should have 4 vertices after approximation
      // relatively large area (to filter out noisy contours)
      // and be convex.
      // Note: absolute value of an area is used because
      // area may be positive or negative - in accordance with the
      // contour orientation
      if (approx.size() == 4 &&
	  fabs(contourArea(Mat(approx))) > 1000 &&
	  isContourConvex(Mat(approx))) {
	double maxCosine = 0;

	for(int j = 2; j < 5; ++j) {
	  // find the maximum cosine of the angle between joint edges
	  double cosine = fabs(angle(approx.at(j%4), approx.at(j-2), approx.at(j-1)));
	  maxCosine = MAX(maxCosine, cosine);
	}

	// if cosines of all angles are small
	// (all angles are ~90 degree) then write quandrange
	// vertices to resultant sequence
	if(maxCosine < 0.3)
	  allRectangles.push_back(approx);
      }
    }
  }

  if (allRectangles.size() == 0)
    foundRectangle = false;
}

void RectangleDetector::populateRectangles() {
  for (unsigned i = 0; i < allRectangles.size(); ++i) {
    Rectangle temprect(allRectangles.at(i));
    rectList.push_back(temprect);
  }

  if (rectList.size() == 0)
    foundRectangle = false;
}

bool RectangleDetector::rectangleIsContained(Rectangle rectContainer, Rectangle rectContained) {
  return (rectContainer.containsPoint(rectContained.center) &&
	  rectContained.lengthSquaredLeft < rectContainer.lengthSquaredLeft &&
	  rectContained.lengthSquaredRight < rectContainer.lengthSquaredRight &&
	  rectContained.lengthSquaredTop < rectContainer.lengthSquaredTop &&
	  rectContained.lengthSquaredBottom < rectContainer.lengthSquaredBottom);
}

void RectangleDetector::findContainedRectangles() {
  // Populate Contained Rectangles Vectors
  for (unsigned i = 0; i < rectList.size(); ++i) {
    for (unsigned j = 0; j < rectList.size(); ++j) {
      if (i == j) continue;
      if (rectangleIsContained(rectList.at(i), rectList.at(j)))
	rectList.at(i).containedRectangles.push_back(j);
    }
  }

  // Filter Out Rectangles That Are Not Contained or Containers
  vector<int> rectIndiciesTmp;
  for (unsigned i = 0; i < rectList.size(); ++i) {
    for (unsigned j = 0; j < rectList.at(i).containedRectangles.size(); ++j)
      rectIndiciesTmp.push_back(rectList.at(i).containedRectangles.at(j));
    if (rectList.at(i).containedRectangles.size() > 0)
      rectIndiciesTmp.push_back(i);
  }
  set<int> rectIndiciesSet(rectIndiciesTmp.begin(), rectIndiciesTmp.end());
  vector<int> rectIndicies(rectIndiciesSet.begin(), rectIndiciesSet.end());
  for (unsigned i = 0; i < rectIndicies.size(); ++i)
    finalRectangles.push_back(allRectangles.at(i));
  for (unsigned i = 0; i < rectIndicies.size(); ++i)
    rectListRevised.push_back(rectList.at(rectIndicies.at(i)));

  // Clear Contained Rectangles Vectors
  for (unsigned i = 0; i < rectListRevised.size(); ++i)
    rectListRevised.at(i).containedRectangles.clear();

  // Repopulate Contained Rectangles Vectors
  for (unsigned i = 0; i < rectListRevised.size(); ++i) {
    for (unsigned j = 0; j < rectListRevised.size(); ++j) {
      if (i == j) continue;
      if (rectangleIsContained(rectListRevised.at(i), rectListRevised.at(j)))
	rectListRevised.at(i).containedRectangles.push_back(j);
    }
  }
}

void RectangleDetector::findCorrectRectangles() {
  // Find Correct Rectangle
  vector<int> rectLengthSquareds;
  for (unsigned i = 0; i < rectListRevised.size(); ++i)
    rectLengthSquareds.push_back(rectListRevised.at(i).lengthSquaredTop);
  sort(rectLengthSquareds.begin(), rectLengthSquareds.end());
  for (unsigned i = 0; i < rectListRevised.size(); ++i) {
    if (rectLengthSquareds.back() == rectListRevised.at(i).lengthSquaredTop)
      rectIndex = i;
  }

  if (rectIndex < 0) {
    foundRectangle = false;
    return;
  }

  // Put Correct Rectangle Into Vector
  vector<Point> correctRectTmp = finalRectangles.at(rectIndex);
  finalRectangles.clear();
  finalRectangles.push_back(correctRectTmp);
}

void RectangleDetector::computeDistance() {
  int distanceTop = (constList.rectBase * constList.cameraFocalLength) / sqrt(rectListRevised.at(rectIndex).lengthSquaredTop);
  int distanceBottom = (constList.rectBase * constList.cameraFocalLength) / sqrt(rectListRevised.at(rectIndex).lengthSquaredBottom);
  int distanceLeft = (constList.rectHeight * constList.cameraFocalLength) / sqrt(rectListRevised.at(rectIndex).lengthSquaredLeft);
  int distanceRight = (constList.rectHeight * constList.cameraFocalLength) / sqrt(rectListRevised.at(rectIndex).lengthSquaredRight);
  int distanceBase = (distanceTop + distanceBottom) / 2;
  int distanceHeight = (distanceLeft + distanceRight) / 2;
  if (distanceBase > distanceHeight)
    distance = distanceHeight;
  else
    distance = distanceBase;
}

void RectangleDetector::computeAzimuth() {
  azimuth = ((float)rectListRevised.at(rectIndex).center.x - ((float)image.cols / 2.0)) / (float)constList.cameraFocalLength;
}
