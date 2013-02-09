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

#include <cstdlib>
#include <string>
#include <vector>

#include "Constants.hpp"

Constants::Constants()
{
        // Math
        mathPi = 3.14159265358979;

        // Camera
        cameraFocalLength = 655;
        cameraViewingAngle = 1.1414453308043;
        cameraHDFocalLength = 655;
        cameraHDViewingAngle = 1.1414453308043;
        cameraHeight = 1162;

        // GUI
        guiWindowName = "FRC Team 3341 Targeting";

        // Rectangle
	rectBase = 62.0 * 25.4;
	rectHeight = 20.0 * 25.4;
        rectPossibleHeights.push_back(-171);
        rectPossibleHeights.push_back(387);
        rectPossibleHeights.push_back(1607);

        // Shooter
        launchAngleDegrees = 55.0;

        // Preprocessing
        preprocessingHueLowerThreshold = 0;
        preprocessingHueUpperThreshold = 80;
        preprocessingValueLowerThreshold = 200;
        preprocessingValueUpperThreshold = 255;

	// Detection
	detectionCannyThreshold = 50;
	detectionMaxCosine = 0.3;
	detectionContainedAreaLowerThreshold = 0.0;
	detectionContainedAreaUpperThreshold = 0.9;

	// Server
	portNumber = 3341;
}
