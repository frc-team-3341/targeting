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
#include "Rectangle.hpp"
#include "RectangleProcessor.hpp"

// Public Functions
RectangleProcessor::RectangleProcessor(Constants *inputConstList)
{
    // Variable Initializations
    constList = inputConstList;
}

void RectangleProcessor::processRectangle(Rectangle input, float tHeight, float tBase)
{
    // Variable Initializations
    inputRect = input;
    targetHeight = tHeight;
    targetBase = tBase;

    // Process Rectangle
    computeElevation();
    computeProportionalDistance();
    computeConstantsDistance();
    computeHeight();
    computeHorizontalDistance();
    computeAzimuth();
    //fixHeight();
    computeTilt();
    computeAspectRatio();
    computeLogAspectRatios();
}

float RectangleProcessor::getAzimuth()
{
    return azimuth;
}

int RectangleProcessor::getProportionalDistance()
{
    return proportionalDistance;
}

int RectangleProcessor::getConstantsDistance()
{
    return constantsDistance;
}

float RectangleProcessor::getLogAspectRatios()
{
    return logAspectRatios;
}

int RectangleProcessor::getHorizontalDistance()
{
    return horizontalDistance;
}

int RectangleProcessor::getHeight()
{
    return height;
}

float RectangleProcessor::getTilt()
{
    return tilt;
}

float RectangleProcessor::getElevation()
{
    return elevation;
}

float RectangleProcessor::getAspectRatio()
{
    return aspectRatio;
}

int RectangleProcessor::getTarget()
{
    return target;
}

// Private Functions
void RectangleProcessor::computeProportionalDistance()
{
    // Compute distance from each side of rectangle
    int distanceTop = (targetBase * constList->cameraFocalLength) / sqrt(inputRect.lengthSquaredTop);
    int distanceBottom = (targetBase * constList->cameraFocalLength) / sqrt(inputRect.lengthSquaredBottom);
    int distanceLeft = (targetHeight * constList->cameraFocalLength * cos(elevation)) / sqrt(inputRect.lengthSquaredLeft);
    int distanceRight = (targetHeight * constList->cameraFocalLength * cos(elevation)) / sqrt(inputRect.lengthSquaredRight);
    int distanceBase = (distanceTop + distanceBottom) / 2;
    int distanceHeight = (distanceLeft + distanceRight) / 2;

    // Account for camera angle
    //int oldDistanceHeight = -1e6;
    //std::cout << "distanceHeight = " << distanceHeight << std::endl;
    //while ((float)abs(distanceHeight - oldDistanceHeight) / (float)distanceHeight > 0.01) {
    //oldDistanceHeight = distanceHeight;
    //	distanceHeight *= sqrt(1 - pow((constList->targetHeight.at(target) - constList->cameraHeight) / distanceHeight, 2));
    //	std::cout << "oldDistanceHeight = " << oldDistanceHeight << std::endl;
    //	std::cout << "distanceHeight = " << distanceHeight << std::endl;
    //}

    // Take smaller distance
    if (distanceBase > distanceHeight) {
        proportionalDistance = distanceHeight;
        //std::cout << "distanceHeight (" << distanceHeight << ") picked over distanceBase (" << distanceBase << ")" << std::endl;
    } else {
        proportionalDistance = distanceBase;
        //std::cout << "distanceBase (" << distanceBase << ") picked over distanceHeight (" << distanceHeight << ")" << std::endl;
    }
}

void RectangleProcessor::computeConstantsDistance()
{
    /*std::cout << "targetHeight = " << constList->targetHeight.at(target) << std::endl;
    std::cout << "cameraHeight = " << constList->cameraHeight << std::endl;
    std::cout << "sin(elevation) = " << sin(elevation) << std::endl;*/
    constantsDistance = fabs((targetHeight - (float)constList->cameraHeight) / sin(elevation));

}

void RectangleProcessor::computeLogAspectRatios()
{
    logAspectRatios = fabs(log((float)aspectRatio * targetHeight/targetBase ));
}

void RectangleProcessor::computeAzimuth()
{
    azimuth = ((float)inputRect.center.x - ((float)constList->imgCols / 2.0)) / (float)constList->cameraFocalLength;
}

void RectangleProcessor::computeElevation()
{
    float tanElevation = (((float)constList->imgRows / 2.0) - (float)inputRect.center.y) / (float)constList->cameraFocalLength;
    elevation = atan(tanElevation) + constList->cameraAngleRadians;
}

void RectangleProcessor::computeHeight()
{
    height = (float)proportionalDistance * sin(elevation);
    /*int heightError = 1000000;
      int heightIndex = 0;
      for (unsigned i = 0; i < constList->rectPossibleHeights.size(); ++i) {
      if (abs(constList->rectPossibleHeights.at(i) - height) < heightError) {
      heightIndex = i;
      heightError = abs(constList->rectPossibleHeights.at(i) - height);
      }
      }

      height = constList->rectPossibleHeights.at(heightIndex);*/
}

void RectangleProcessor::computeHorizontalDistance()
{
    //horizontalDistance = sqrt(pow(proportionalDistance, 2) - pow(height, 2));
    horizontalDistance = sqrt(pow(proportionalDistance, 2) - pow(targetHeight- constList->cameraHeight, 2));
}

void RectangleProcessor::computeTilt()
{
    float cosTilt = ((sqrt(inputRect.lengthSquaredTop) + sqrt(inputRect.lengthSquaredBottom)) / (sqrt(inputRect.lengthSquaredLeft) + sqrt(inputRect.lengthSquaredRight))) * (targetHeight /targetBase);
    if (cosTilt > 0.98)
        tilt = sqrt(1 - pow(cosTilt, 2));
    else
        tilt = acos(cosTilt);

    if (inputRect.lengthSquaredLeft < inputRect.lengthSquaredRight)
        tilt *= -1;
}

void RectangleProcessor::computeAspectRatio()
{
    /*std::cout << "sqrt(inputRect.lengthSquaredTop) = " << sqrt(inputRect.lengthSquaredTop) << std::endl;
    std::cout << "sqrt(inputRect.lengthSquaredBottom) = " << sqrt(inputRect.lengthSquaredBottom) << std::endl;
    std::cout << "sqrt(inputRect.lengthSquaredLeft) = " << sqrt(inputRect.lengthSquaredLeft) << std::endl;
    std::cout << "sqrt(inputRect.lengthSquaredRight) = " << sqrt(inputRect.lengthSquaredRight) << std::endl;*/
    aspectRatio = (sqrt(inputRect.lengthSquaredTop) + sqrt(inputRect.lengthSquaredBottom)) / (sqrt(inputRect.lengthSquaredLeft) + sqrt(inputRect.lengthSquaredRight));
}

void RectangleProcessor::to_string()
{
    std::cout << "Proportional distance: " << proportionalDistance << " mm" << std::endl;
    std::cout << "Constants distance: " << constantsDistance << " mm" << std::endl;
    std::cout << "Log of aspect ratios: " << logAspectRatios << std::endl;
    std::cout << "Horizontal distance: " << horizontalDistance << " mm" << std::endl;
    std::cout << "Height: " << height << " mm" << std::endl;
    std::cout << "Azimuth: " << getAzimuth() * 180.0 / constList->mathPi << " degrees" << std::endl;
    std::cout << "Tilt: " << getTilt() * 180.0 / constList->mathPi << " degrees" << std::endl;
    std::cout << "Elevation: " << getElevation() * 180.0 / constList->mathPi << " degrees" << std::endl;
    std::cout << "Aspect Ratio: " << aspectRatio << std::endl << std::endl;
}

