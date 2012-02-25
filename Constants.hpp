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

  // Rectangle
  float rectBase;
  float rectHeight;

  // Preprocessing
  int preprocessingLowerThreshold;
};
#endif /* Constants_hpp */
