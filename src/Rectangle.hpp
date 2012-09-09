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

#ifndef Rectangle_hpp
#define Rectangle_hpp

class Rectangle
{
public:
    // Methods
    Rectangle(); // Constructor
    Rectangle(int*); // Constructor
    Rectangle(std::vector<cv::Point>); // Populate Rectangle
    void populate(std::vector<cv::Point>); // Populate Rectangle
    bool containsPoint(cv::Point); // Checks if Point is Inside Rectangle

    // Data
    cv::Point topLeft; // Top Left Corner
    cv::Point topRight; // Top Right Corner
    cv::Point bottomRight; // Bottom Right Corner
    cv::Point bottomLeft; // Bottom Left Corner
    cv::Point center; // Center Point
    int lengthSquaredLeft; // Square of Length of Left Side
    int lengthSquaredRight; // Square of Length of Right Side
    int lengthSquaredTop; // Square of Length of Top Side
    int lengthSquaredBottom; // Square of Length of Bottom Side
    std::vector<int> containedRectangles; // Indices of Contained Rectangles

private:
    // Methods

    // Data
};

#endif /* Rectangle_hpp */
