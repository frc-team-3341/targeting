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

#ifndef Constants_hpp
#define Constants_hpp

#include <vector>

class Constants
{
    public:
        Constants();

        // Math
        float mathPi;

        // Camera
        int cameraFocalLength;
        float cameraViewingAngleRadians;
        int cameraHDFocalLength;
        float cameraHDViewingAngleRadians;
        int cameraHeight;
        float cameraAngleRadians;

        // Image
        int imgCols;
        int imgRows;

        // GUI
        std::string guiWindowName;

        int targetHorizontal;
        int targetVertical;

        // Target
        std::vector<float> targetRectBase;
        std::vector<float> targetRectHeight;
        std::vector<float> targetHeight;
        std::vector<int> targetPossibleHeights;

        // Preprocessing
        int preprocessingHueLowerThreshold;
        int preprocessingHueUpperThreshold;
        int preprocessingValueLowerThreshold;
        int preprocessingValueUpperThreshold;

        // Detection
        float detectionMaxCosine;
        int detectionCannyThreshold;
        float detectionContainedAreaLowerThreshold;
        float detectionContainedAreaUpperThreshold;

        // Server
        int portNumber;
};
#endif /* Constants_hpp */
