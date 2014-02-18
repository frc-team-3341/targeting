/*
 *    This file is part of FRC Team 3341 Targeting.
 *
 *    FRC Team 3341 Targeting is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    FRC Team 3341 Targeting is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with FRC Team 3341 Targeting.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <cstdlib>
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

// Public Functions
RectangleDetector::RectangleDetector(Constants *inputConstList)
{
    // Variable Initializations
    foundRectangle = true;
    constList = inputConstList;
}

std::vector<Rectangle> RectangleDetector::processImage(cv::Mat input)
{
    // Variable Initializations
    input.copyTo(image);

    preprocessImage();
    findRectangles();
    if (! foundRectangle) return std::vector<Rectangle>();
    populateRectangles();
    if (! foundRectangle) return std::vector<Rectangle>();
    filterUniqueRectangles();
    findVerticalRectangles();
    findHorizontalRectangles();
    findVHPair();
    /* findContainerRectangles();
       if (! foundRectangle) return std::vector<Rectangle>();
       findCorrectRectangles();
       if (! foundRectangle) return std::vector<Rectangle>();
       */


    return outputRectangles;
}

bool RectangleDetector::rectangleWasFound()
{   
    return foundRectangle;
}

std::vector<std::vector<cv::Point> > RectangleDetector::getAllRectangles()
{
    return allRectangles;
}

std::vector<std::vector<cv::Point> > RectangleDetector::getFinalRectangles()
{
    return finalRectangles;
}

// Private Functions
double RectangleDetector::angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
}

void RectangleDetector::preprocessImage()
{
    // Variable Declarations
    HSVImage imageHSV(image);
    cv::Mat hsv_threshHueLower;
    cv::Mat hsv_threshHueUpper;
    cv::Mat hsv_threshValueLower;
    cv::Mat hsv_threshValueUpper;

    // Hue Threshold
    threshold(imageHSV.hue, hsv_threshHueLower, constList->preprocessingHueLowerThreshold, 255, CV_THRESH_BINARY);
    threshold(imageHSV.hue, hsv_threshHueUpper, constList->preprocessingHueUpperThreshold, 255, CV_THRESH_BINARY_INV);
    imageHSV.hue = hsv_threshHueLower & hsv_threshHueUpper;

    // Value Threshold
    threshold(imageHSV.value, hsv_threshValueLower, constList->preprocessingValueLowerThreshold, 255, CV_THRESH_BINARY);
    threshold(imageHSV.value, hsv_threshValueUpper, constList->preprocessingValueUpperThreshold, 255, CV_THRESH_BINARY_INV);
    imageHSV.value = hsv_threshValueLower & hsv_threshValueUpper;

    image = imageHSV.hue & imageHSV.value;
}

void RectangleDetector::findRectangles()
{
    // Clear Rectangle Vectors
    allRectangles.clear();
    finalRectangles.clear();

    // Variable Declarations
    cv::Mat gray0(image.size(), CV_8U);
    cv::Mat gray;

    // Variable Declarations
    std::vector<std::vector<cv::Point> > contours;
    int ch[] = {0, 0};

    cv::mixChannels(&image, 1, &gray0, 1, ch, 1); // Extract Channel

    // Canny helps to catch squares with gradient shading
    // apply Canny. Take the upper threshold from slider
    // and set the lower to 0 (which forces edges merging)
    Canny(gray0, gray, 0, constList->detectionCannyThreshold, 5);

    // dilate canny output to remove potential
    // holes between edge segments
    dilate(gray, gray, cv::Mat(), cv::Point(-1,-1));

    findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE); // Find Contours

    // Variable Declarations
    std::vector<cv::Point> approx;

    // Test Each Contour
    for (size_t i = 0; i < contours.size(); ++i) {
        // approximate contour with accuracy proportional
        // to the contour perimeter
        cv::approxPolyDP(cv::Mat(contours.at(i)), approx, cv::arcLength(cv::Mat(contours.at(i)), true) * 0.02, true);

        // rectangular contours should have 4 vertices after approximation
        // relatively large area (to filter out noisy contours)
        // and be convex.
        // Note: absolute value of an area is used because
        // area may be positive or negative - in accordance with the
        // contour orientation
        if (approx.size() == 4 &&
                fabs(cv::contourArea(cv::Mat(approx))) > 1000 &&
                cv::isContourConvex(cv::Mat(approx))) {
            double maxCosine = 0;

            for(int j = 2; j < 5; ++j) {
                // find the maximum cosine of the angle between joint edges
                double cosine = fabs(angle(approx.at(j%4), approx.at(j-2), approx.at(j-1)));
                maxCosine = MAX(maxCosine, cosine);
            }

            // if cosines of all angles are small
            // (all angles are ~90 degrees) then write quandrange
            // vertices to resultant sequence
            if(maxCosine < constList->detectionMaxCosine)
                allRectangles.push_back(approx);
        }
    }

    if (allRectangles.size() == 0)
        foundRectangle = false;
}

void RectangleDetector::populateRectangles()
{
    for (unsigned i = 0; i < allRectangles.size(); ++i) {
        Rectangle temprect(allRectangles.at(i));
        rectList.push_back(temprect);
    }

    if (rectList.size() == 0)
        foundRectangle = false;
}

void RectangleDetector::filterUniqueRectangles()
{
    // Variable Declarations
    std::vector<Rectangle> rectListUnique;

    for (unsigned i = 0; i < rectList.size(); ++i) {
        bool isUnique = true;
        for (unsigned j = 0; j < rectListUnique.size(); ++j) {
            if (rectList.at(i).topLeft == rectListUnique.at(j).topLeft &&
                    rectList.at(i).topRight == rectListUnique.at(j).topRight &&
                    rectList.at(i).bottomRight == rectListUnique.at(j).bottomRight &&
                    rectList.at(i).bottomLeft == rectListUnique.at(j).bottomLeft)
                isUnique = false;
        }

        if (isUnique)
            rectListUnique.push_back(rectList.at(i));
    }
    rectList = rectListUnique;
    /*
       for (int i = 0; i < (int)rectListUnique.size(); ++i) {
       if (rectListUnique.at(i).markedForRemoval) {
       allRectangles.push_back(rectListUnique.at(i).rectPoints);
       } else {
       finalRectangles.push_back(rectListUnique.at(i).rectPoints);
       outputRectangles.push_back(rectListUnique.at(i));
       std::cout << "adding things to outputRects" << std::endl;
       }
       }
       */

}

bool RectangleDetector::rectangleIsContained(Rectangle rectContainer, Rectangle rectContained)
{
    return (rectContainer.containsPoint(rectContained.center) &&
            rectContained.lengthSquaredLeft < rectContainer.lengthSquaredLeft &&
            rectContained.lengthSquaredRight < rectContainer.lengthSquaredRight &&
            rectContained.lengthSquaredTop < rectContainer.lengthSquaredTop &&
            rectContained.lengthSquaredBottom < rectContainer.lengthSquaredBottom);
}

void RectangleDetector::findVerticalRectangles(){
    for (unsigned i = 0; i < rectList.size(); ++i) {
        if (rectList.at(i).lengthSquaredLeft > rectList.at(i).lengthSquaredTop)
            verticalRectangleList.push_back(rectList.at(i));
    }
}

void RectangleDetector::findHorizontalRectangles(){
    for (unsigned i = 0; i < rectList.size(); ++i) {
        if (rectList.at(i).lengthSquaredTop > rectList.at(i).lengthSquaredLeft)
            horizontalRectangleList.push_back(rectList.at(i));
    }
}

void RectangleDetector::findVHPair(){

    std::cout << verticalRectangleList.size() << std::endl;
    for (unsigned i = 0; i < verticalRectangleList.size(); ++i) {
        for (unsigned j = 0; j < horizontalRectangleList.size(); ++j) {
            if (rectanglePairMatches(verticalRectangleList.at(i), horizontalRectangleList.at(j))){
                verticalRectangleList.at(i).pairedHorizontalRectangles.push_back(j);
                outputRectangles.push_back(verticalRectangleList.at(i));
                outputRectangles.push_back(horizontalRectangleList.at(j));
            }
        }
    }

    for (int i = 0; i < (int)verticalRectangleList.size(); ++i) {
        std::cout << "Vertical Rectangle " << i << std::endl;

        verticalRectangleList.at(i).to_string();

        if ( verticalRectangleList.at(i).pairedHorizontalRectangles.size() > 0){
            std::cout << "Matching Horizontal Rectangle " << "fix index list later" << std::endl;
            horizontalRectangleList.at(verticalRectangleList.at(i).pairedHorizontalRectangles.at(0)).to_string();
        }
        else{
            std::cout << "No Matching Horizontal Rectangle found" << std::endl;
        }

    }

}

bool RectangleDetector::rectanglePairMatches(Rectangle vertical, Rectangle horizontal)
{
    double toleranceDistance = 900;  
    if (horizontal.topLeft.x > vertical.topLeft.x - toleranceDistance &&
            horizontal.topLeft.x < vertical.topLeft.x){
        return true;
    }

    if (horizontal.topRight.x < vertical.topRight.x + toleranceDistance &&
            horizontal.topRight.x > vertical.topRight.x){ 
        return true;
    }
    std::cout << "Returned True" << std::endl;
    return false;

}


void RectangleDetector::findContainerRectangles()
{
    // Populate Contained Rectangles Vectors
    for (unsigned i = 0; i < rectList.size(); ++i) {
        for (unsigned j = 0; j < rectList.size(); ++j) {
            if (i == j) continue;
            if (rectangleIsContained(rectList.at(i), rectList.at(j)))
                rectList.at(i).containedRectangles.push_back(j);
        }
    }

    // Filter Out Rectangles That Are Not Contained or Containers
    std::vector<int> rectIndiciesTmp;
    for (unsigned i = 0; i < rectList.size(); ++i) {
        for (unsigned j = 0; j < rectList.at(i).containedRectangles.size(); ++j)
            rectIndiciesTmp.push_back(rectList.at(i).containedRectangles.at(j));
        if (rectList.at(i).containedRectangles.size() > 0) 
            rectIndiciesTmp.push_back(i);
    }
    std::set<int> rectIndiciesSet(rectIndiciesTmp.begin(), rectIndiciesTmp.end());
    std::vector<int> rectIndicies(rectIndiciesSet.begin(), rectIndiciesSet.end());
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
            if (rectangleIsContained(rectListRevised.at(i), rectListRevised.at(j))) {
                rectListRevised.at(i).containedRectangles.push_back(j);
            }
        }
    }

    for (int i = 0; i < (int)rectListRevised.size(); ++i) {
        std::cout << "Rectangle " << i << std::endl;
        std::cout << "\t Top left: " << rectListRevised.at(i).topLeft << std::endl;
        std::cout << "\t Top right: " << rectListRevised.at(i).topRight << std::endl;
        std::cout << "\t Bottom right: " << rectListRevised.at(i).bottomRight << std::endl;
        std::cout << "\t Bottom left: " << rectListRevised.at(i).bottomLeft << std::endl;
        std::cout << "\t Area: " << rectListRevised.at(i).area << std::endl;
    }

    // Remove Contained Rectangles and Containers Not Meeting Threshold
    for (int i = 0; i < (int)rectListRevised.size(); ++i) {
        if (rectListRevised.at(i).markedForRemoval) continue;
        bool meetsThreshold = false;
        for (int j = 0; j < (int)rectListRevised.at(i).containedRectangles.size(); ++j) {
            rectListRevised.at(rectListRevised.at(i).containedRectangles.at(j)).markedForRemoval = true;
            std::cout << "Rectangle " << rectListRevised.at(i).containedRectangles.at(j) << " marked for removal (contained)" << std::endl;
            if (rectListRevised.at(rectListRevised.at(i).containedRectangles.at(j)).area < (rectListRevised.at(i).area * constList->detectionContainedAreaUpperThreshold) &&
                    rectListRevised.at(rectListRevised.at(i).containedRectangles.at(j)).area >= (rectListRevised.at(i).area * constList->detectionContainedAreaLowerThreshold))
                meetsThreshold = true;
        }
        if (!meetsThreshold) {
            rectListRevised.at(i).markedForRemoval = true;
            std::cout << "Rectangle " << i << " marked for removal (does not meet threshold)" << std::endl;
        }
    }
    /*for (int i = (int)rectListRevised.size() - 1; i >= 0; --i) {
      rectListRevised.at(i).containedRectangles.clear();
      if (rectListRevised.at(i).markedForRemoval)
      rectListRevised.erase(rectListRevised.begin() + i - 1);
      }*/

    std::cout << "Length: " << rectListRevised.size() << std::endl;
    if (rectListRevised.size() == 0)
        foundRectangle = false;
}

void RectangleDetector::findCorrectRectangles()
{
    /*// Find Correct Rectangle
      std::vector<int> rectLengthSquareds;
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
    std::vector<cv::Point> correctRectTmp = finalRectangles.at(rectIndex);
    finalRectangles.clear();
    finalRectangles.push_back(correctRectTmp);*/

    allRectangles.clear();
    finalRectangles.clear();
    for (int i = 0; i < (int)rectListRevised.size(); ++i) {
        if (rectListRevised.at(i).markedForRemoval) {
            allRectangles.push_back(rectListRevised.at(i).rectPoints);
        } else {
            finalRectangles.push_back(rectListRevised.at(i).rectPoints);
            outputRectangles.push_back(rectListRevised.at(i));
            std::cout << "adding things to outputRects" << std::endl;
        }
    }
}
