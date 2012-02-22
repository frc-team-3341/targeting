#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <cmath>
#include <cstring>
#include <string>
#include <sstream>

#include "Rectangle.hpp"

using namespace cv;
using namespace std;

Rectangle::Rectangle(vector<Point> inputRect) {
  // Compute Lengths
  lengthSquaredLeft=pow((topLeft.x - bottomLeft.x), 2) + pow((topLeft.y - bottomLeft.y), 2);
}
