#include "Constants.hpp"

Constants::Constants() {
  // Math
  mathPi = 3.1415927;

  // Camera
  cameraFocalLength = 655;
  cameraViewingAngle = 1.1414453308043;
  cameraHDFocalLength = 655;
  cameraHDViewingAngle = 1.1414453308043;

  // Rectangle
  rectBase = 609.6;
  rectHeight = 457.2;

  // Preprocessing
  preprocessingHueLowerThreshold = 0;
  preprocessingHueUpperThreshold = 40;
  preprocessingValueLowerThreshold = 200;
  preprocessingValueUpperThreshold = 255;
}
