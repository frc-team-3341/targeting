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

int thresh =50;
int N = 11;

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
    removeSimilarRectangles();
    if (! foundRectangle) return std::vector<Rectangle>();
    removeMarkedRectangles();

    std::cout << "Returning output Rectangles..." << std::endl;

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
    std::cout << "Pre-processing image..." << std::endl;
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
    //cv::imshow("hue thresholded", imageHSV.hue);

    // Value Threshold
    threshold(imageHSV.value, hsv_threshValueLower, constList->preprocessingValueLowerThreshold, 255, CV_THRESH_BINARY);
    threshold(imageHSV.value, hsv_threshValueUpper, constList->preprocessingValueUpperThreshold, 255, CV_THRESH_BINARY_INV);
    imageHSV.value = hsv_threshValueLower & hsv_threshValueUpper;
    //cv::imshow("value thresholded", imageHSV.value);

    image = imageHSV.value;
    cv::imshow("thresholded", image);
}

void RectangleDetector::findRectangles()
{
    std::cout << "Finding rectangles..." << std::endl;
    // Clear Rectangle Vectors
    allRectangles.clear();
    finalRectangles.clear();


    // Variable Declarations
    cv::Mat pyr;
    cv::Mat timg;
    cv::Mat gray0(image.size(), CV_8U);
    cv::Mat gray;

    cv::pyrDown(image, pyr, cv::Size(image.cols / 2, image.rows / 2));
    cv::pyrUp(pyr, timg, image.size());

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
    if (approx.size() >= 4 &&
    //approx.size() <= 6 &&
    fabs(cv::contourArea(cv::Mat(approx))) > 200// &&
    //cv::isContourConvex(cv::Mat(approx))
    ) {
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
    
    
    std::cout << allRectangles.size();
    if (allRectangles.size() == 0)
        foundRectangle = false;


}

void RectangleDetector::populateRectangles()
{
    std::cout << "Populating Rectangles..." << std::endl;
    for (unsigned i = 0; i < allRectangles.size(); ++i) {
        Rectangle temprect(allRectangles.at(i));
        rectList.push_back(temprect);
    }

    if (rectList.size() == 0)
        foundRectangle = false;
}

void RectangleDetector::filterUniqueRectangles()
{
    std::cout << "Filtering Unique Rectangles..." << std::endl;
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
}

void RectangleDetector::removeSimilarRectangles(){
    std::cout << "Removing very similar rectangles..." << std::endl;
    for (unsigned i = 0; i < rectList.size(); ++i) {
        for (unsigned j = 0; j < rectList.size(); ++j) {
            if (i == j) continue;
            if (rectList.at(j).markedForRemoval) continue;
            if (similarRectangles(rectList.at(i), rectList.at(j))){
                rectList.at(i).markedForRemoval = true;
                std::cout << "marked" << std::endl;
            }
        }
        rectListRevised.push_back(rectList.at(i));
    }

}

bool RectangleDetector::similarRectangles(Rectangle one, Rectangle two){
    if(one.topLeft.x + 20 > two.topLeft.x && one.topLeft.x - 20 < two.topLeft.x &&
            one.topLeft.y + 20 > two.topLeft.y && one.topLeft.y - 20 < two.topLeft.y &&

            one.topRight.x + 20 > two.topRight.x && one.topRight.x - 20 < two.topRight.x &&
            one.topRight.y + 20 > two.topRight.y && one.topRight.y - 20 < two.topRight.y &&

            one.bottomLeft.x + 20 > two.bottomLeft.x && one.bottomLeft.x - 20 < two.bottomLeft.x &&
            one.bottomLeft.y + 20 > two.bottomLeft.y && one.bottomLeft.y - 20 < two.bottomLeft.y &&

            one.bottomRight.x + 20 > two.bottomRight.x && one.bottomRight.x - 20 < two.bottomRight.x &&
            one.bottomRight.y + 20 > two.bottomRight.y && one.bottomRight.y - 20 < two.bottomRight.y){
        std::cout << "marked similar rectangle for removal" << std::endl;
        return true;
    }

    return false;
}

void RectangleDetector::removeMarkedRectangles()
{
    std::cout << "Removing incorrect rectangles" << std::endl;

    allRectangles.clear();
    finalRectangles.clear();

    //std::cout << std::endl << "Printing Remaining Rectangles: " << std::endl;

    for (int i = 0; i < (int)rectListRevised.size(); ++i) {
        if (rectListRevised.at(i).markedForRemoval) {
            allRectangles.push_back(rectListRevised.at(i).rectPoints);
        } else {
            finalRectangles.push_back(rectListRevised.at(i).rectPoints);
            outputRectangles.push_back(rectListRevised.at(i));
        }
    }

    //std::cout << std::endl << "Remaining Rectangles Count: " << outputRectangles.size() << std::endl;

}
