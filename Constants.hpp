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
