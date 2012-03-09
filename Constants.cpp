#include <vector>

#include "Constants.hpp"

Constants::Constants() {
  // Math
  mathPi = 3.14159265358979;

  // Camera
  cameraFocalLength = 655;
  cameraViewingAngle = 1.1414453308043;
  cameraHDFocalLength = 655;
  cameraHDViewingAngle = 1.1414453308043;
  cameraHeight = 1162;

  // Rectangle
  rectBase = 609.6;
  rectHeight = 457.2;
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
}
