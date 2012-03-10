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

#ifndef RectangleDetector_hpp
#define RectangleDetector_hpp

class RectangleDetector {
public:
  // Methods
  RectangleDetector(cv::Mat /* Input Image */);
  bool rectangleWasFound();
  float getAzimuth();
  int getDistance();
  int getHorizontalDistance();
  float getVelocity();
  int getHeight();
  float getTilt();
  std::vector< std::vector<cv::Point> > getAllRectangles();
  std::vector< std::vector<cv::Point> > getFinalRectangles();
private:
  // Data
  Constants constList;
  cv::Mat image;
  std::vector< std::vector<cv::Point> > allRectangles;
  std::vector< std::vector<cv::Point> > finalRectangles;
  std::vector<Rectangle> rectList;
  std::vector<Rectangle> rectListRevised;
  bool foundRectangle;
  int rectIndex;
  float azimuth;
  int distance;
  int horizontalDistance;
  float velocity;
  int height;
  float tilt;

  // Methods
  double angle(cv::Point, cv::Point, cv::Point); // Find Cosine of Angle Between Vectors
  void preprocessImage(); // Threshold and Grayscale
  void findRectangles(); // Finds Rectangles
  void populateRectangles(); // Populates Vector of Rectangles
  bool rectangleIsContained(Rectangle, Rectangle); // Check if Rectangle is Contained
  void findContainedRectangles(); // Finds Contained Rectangles
  void findCorrectRectangles(); // Figures Out Which Rectangles Are Correct
  void computeDistance(); // Computes Distance to the Rectangle
  void computeHorizontalDistance(); // Computes Horizontal Distance to the Rectangle
  void computeHeight(); // Computes Height of the Rectangle
  void computeVelocity(); // Computes Velocity of Ball
  void computeAzimuth(); // Computes Azimuth to the Rectangle
  void fixHeight(); // Changes Lowest Hoop to Highest
  void computeTilt(); // Computes Tilt of the Rectangle
};

#endif /* RectangleDetector_hpp */
