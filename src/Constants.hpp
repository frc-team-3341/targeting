/*
 *    This file is part of FRC Team 3341 Aimer.
 *
 *    FRC Team 3341 Aimer is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    FRC Team 3341 Aimer is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with FRC Team 3341 Aimer.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef Constants_hpp
#define Constants_hpp

class Constants {
public:
  Constants();

  // Math
  float mathPi;

  // Camera
  int cameraFocalLength;
  float cameraViewingAngle;
  int cameraHDFocalLength;
  float cameraHDViewingAngle;
  int cameraHeight;

  // Image
  int imgCols;
  int imgRows;

  // GUI
  std::string guiWindowName;

  // Rectangle
  float rectBase;
  float rectHeight;
  std::vector<int> rectPossibleHeights;

  // Shooter
  float launchAngleDegrees;
  float launchAngleRadians;

  // Preprocessing
  int preprocessingHueLowerThreshold;
  int preprocessingHueUpperThreshold;
  int preprocessingValueLowerThreshold;
  int preprocessingValueUpperThreshold;
};
#endif /* Constants_hpp */
