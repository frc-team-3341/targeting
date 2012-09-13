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

#ifndef RectangleProcessor_hpp
#define RectangleProcessor_hpp

class RectangleProcessor
{
public:
    // Methods
    RectangleProcessor(Constants* /* Constant List */);
    void processRectangle(Rectangle);
    float getAzimuth();
    int getDistance();
    int getHorizontalDistance();
    float getVelocity();
    int getHeight();
    float getTilt();
private:
    // Data
    Rectangle inputRect;
    Constants *constList;
    float azimuth;
    int distance;
    int horizontalDistance;
    float velocity;
    int height;
    float tilt;

    // Methods
    void computeDistance(); // Computes Distance to the Rectangle
    void computeHorizontalDistance(); // Computes Horizontal Distance to the Rectangle
    void computeHeight(); // Computes Height of the Rectangle
    void computeVelocity(); // Computes Velocity of Ball
    void computeAzimuth(); // Computes Azimuth to the Rectangle
    void fixHeight(); // Changes Lowest Hoop to Highest
    void computeTilt(); // Computes Tilt of the Rectangle
};

#endif /* RectangleProcessor_hpp */
