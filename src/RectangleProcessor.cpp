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

void RectangleProcessor::processRectangle(Rectangle input, int inputTarget)
{
        // Variable Initializations
        inputRect = input;
	target = inputTarget;

        // Process Rectangle
	computeElevation();
        computeProportionalDistance();
	computeConstantsDistance();
	computeLogDistances();
        computeHeight();
        computeHorizontalDistance();
        computeAzimuth();
	//fixHeight();
        computeTilt();
	computeAspectRatio();
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

float RectangleProcessor::getLogDistances()
{
	return logDistances;
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

// Private Functions
void RectangleProcessor::computeProportionalDistance()
{
	// Compute distance from each side of rectangle
        int distanceTop = (constList->targetRectBase.at(target) * constList->cameraFocalLength) / sqrt(inputRect.lengthSquaredTop);
        int distanceBottom = (constList->targetRectBase.at(target) * constList->cameraFocalLength) / sqrt(inputRect.lengthSquaredBottom);
        int distanceLeft = (constList->targetRectHeight.at(target) * constList->cameraFocalLength * cos(elevation)) / sqrt(inputRect.lengthSquaredLeft);
        int distanceRight = (constList->targetRectHeight.at(target) * constList->cameraFocalLength * cos(elevation)) / sqrt(inputRect.lengthSquaredRight);
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
		std::cout << "distanceHeight (" << distanceHeight << ") picked over distanceBase (" << distanceBase << ")" << std::endl;
	} else {
                proportionalDistance = distanceBase;
		std::cout << "distanceBase (" << distanceBase << ") picked over distanceHeight (" << distanceHeight << ")" << std::endl;
	}
}

void RectangleProcessor::computeConstantsDistance()
{
	std::cout << "targetHeight = " << constList->targetHeight.at(target) << std::endl;
	std::cout << "cameraHeight = " << constList->cameraHeight << std::endl;
	std::cout << "sin(elevation) = " << sin(elevation) << std::endl;
	constantsDistance = fabs(((float)constList->targetHeight.at(target) - (float)constList->cameraHeight) / sin(elevation));

}

void RectangleProcessor::computeLogDistances()
{
	logDistances = fabs(log((float)proportionalDistance / (float)constantsDistance));
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
        horizontalDistance = sqrt(pow(proportionalDistance, 2) - pow(height, 2));
}

void RectangleProcessor::fixHeight()
{
        if (height == constList->targetPossibleHeights.at(0))
                height = constList->targetPossibleHeights.at(2);
}

void RectangleProcessor::computeTilt()
{
        float cosTilt = ((sqrt(inputRect.lengthSquaredTop) + sqrt(inputRect.lengthSquaredBottom)) / (sqrt(inputRect.lengthSquaredLeft) + sqrt(inputRect.lengthSquaredRight))) * (constList->targetRectHeight.at(target) / constList->targetRectBase.at(target));
        if (cosTilt > 0.98)
		tilt = sqrt(1 - pow(cosTilt, 2));
        else
                tilt = acos(cosTilt);

        if (inputRect.lengthSquaredLeft < inputRect.lengthSquaredRight)
                tilt *= -1;
}

void RectangleProcessor::computeAspectRatio()
{
	std::cout << "sqrt(inputRect.lengthSquaredTop) = " << sqrt(inputRect.lengthSquaredTop) << std::endl;
	std::cout << "sqrt(inputRect.lengthSquaredBottom) = " << sqrt(inputRect.lengthSquaredBottom) << std::endl;
	std::cout << "sqrt(inputRect.lengthSquaredLeft) = " << sqrt(inputRect.lengthSquaredLeft) << std::endl;
	std::cout << "sqrt(inputRect.lengthSquaredRight) = " << sqrt(inputRect.lengthSquaredRight) << std::endl;
	aspectRatio = (sqrt(inputRect.lengthSquaredTop) + sqrt(inputRect.lengthSquaredBottom)) / (sqrt(inputRect.lengthSquaredLeft) + sqrt(inputRect.lengthSquaredRight));
}
