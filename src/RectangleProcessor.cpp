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

void RectangleProcessor::processRectangle(Rectangle input)
{
        // Variable Initializations
        inputRect = input;

        // Process Rectangle
        computeDistance();
	computeElevation();
        computeHeight();
        computeHorizontalDistance();
        computeAzimuth();
        computeTilt();
        //fixHeight();
}

float RectangleProcessor::getAzimuth()
{
        return azimuth;
}

int RectangleProcessor::getDistance()
{
        return distance;
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

// Private Functions
void RectangleProcessor::computeDistance()
{
        int distanceTop = (constList->rectBase * constList->cameraFocalLength) / sqrt(inputRect.lengthSquaredTop);
        int distanceBottom = (constList->rectBase * constList->cameraFocalLength) / sqrt(inputRect.lengthSquaredBottom);
        int distanceLeft = (constList->rectHeight * constList->cameraFocalLength) / sqrt(inputRect.lengthSquaredLeft);
        int distanceRight = (constList->rectHeight * constList->cameraFocalLength) / sqrt(inputRect.lengthSquaredRight);
        int distanceBase = (distanceTop + distanceBottom) / 2;
        int distanceHeight = (distanceLeft + distanceRight) / 2;
        if (distanceBase > distanceHeight)
                distance = distanceHeight;
        else
                distance = distanceBase;
}

void RectangleProcessor::computeAzimuth()
{
        azimuth = ((float)inputRect.center.x - ((float)constList->imgCols / 2.0)) / (float)constList->cameraFocalLength;
}

void RectangleProcessor::computeElevation()
{
        float tanElevation = (((float)constList->imgRows / 2.0) - (float)inputRect.center.y) / (float)constList->cameraFocalLength;
	elevation = atan(tanElevation);
}

void RectangleProcessor::computeHeight()
{
        height = (float)distance * sin(elevation);
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
        horizontalDistance = sqrt(pow(distance, 2) - pow(height, 2));
}

void RectangleProcessor::fixHeight()
{
        if (height == constList->rectPossibleHeights.at(0))
                height = constList->rectPossibleHeights.at(2);
}

void RectangleProcessor::computeTilt()
{
        float cosTilt = ((sqrt(inputRect.lengthSquaredTop) + sqrt(inputRect.lengthSquaredBottom)) / (sqrt(inputRect.lengthSquaredLeft) + sqrt(inputRect.lengthSquaredRight))) * (constList->rectHeight / constList->rectBase);
        if (cosTilt > 0.98)
		tilt = 0;
        else
                tilt = acos(cosTilt);

        if (inputRect.lengthSquaredLeft < inputRect.lengthSquaredRight)
                tilt *= -1;
}
