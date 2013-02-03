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
        computeVelocity();
        computeAzimuth();
        computeTilt();
	computeRPM();
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

float RectangleProcessor::getVelocity()
{
        return velocity;
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

float RectangleProcessor::getRPM()
{
	return rpm;
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

void RectangleProcessor::computeVelocity()
{
        float launchDistance = (float)horizontalDistance / 1000;
        float launchHeight = (float)height / 1000;
        float mass = 0.29;
        float cd = 0.5;
        float gravity = 9.80665;
        float air_density = 1.2;
        float surface_area = (8 * 2.54 / 200) * (8 * 2.54 / 200) * 3.14159265358979;
        float air_resistance_constant = 0.5 * cd * air_density * surface_area;
        float a = sqrt(air_resistance_constant / (mass * gravity));
        float b = air_resistance_constant / mass;
        constList->launchAngleRadians = constList->launchAngleDegrees * 3.14159265358979 / 180;
        float velocity1;
        float velocity2;
        float velocity3;
        float f1 = 1;
        float f3 = 3;
        float psi = exp(b * launchHeight);
        velocity1 = 1;
        velocity2 = 25;
        int i = 0;
        while (fabs(f1) > 0.0001) {
                ++i;
                velocity3 = (velocity1 + velocity2) / 2;
                float z1velocity1 = a * velocity1 * sin(constList->launchAngleRadians);
                float z2velocity1 = a * velocity1 * cos(constList->launchAngleRadians);
                float phi_velocity1 = (exp(b * launchDistance) -1) / z2velocity1;
                f1 = psi - z1velocity1 * sin(phi_velocity1) - cos(phi_velocity1);
                float z1velocity3 = a * velocity3 * sin(constList->launchAngleRadians);
                float z2velocity3 = a * velocity3 * cos(constList->launchAngleRadians);
                float phi_velocity3 = (exp(b * launchDistance) -1)/ z2velocity3;
                f3 = psi - z1velocity3 * sin(phi_velocity3) - cos(phi_velocity3);
                if(f1 * f3 < 0)
                        velocity2 = velocity3;
                else
                        velocity1 = velocity3;
                if (i > 30) {
                        velocity1 = 0;
                        f1 = 0;
                }
        }
        velocity = velocity1;
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

void RectangleProcessor::computeRPM()
{	
	rpm = (0.2269 * (float)distance) + 943.64;
}
