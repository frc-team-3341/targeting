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

#include "Constants.hpp"
#include "Rectangle.hpp"

class RectangleProcessor
{
    public:
        // Methods
        RectangleProcessor(Constants* constList);
        void processRectangle(Rectangle inputRect, int target);
        float getAzimuth();
        int getProportionalDistance();
        int getConstantsDistance();
        float getLogAspectRatios();
        int getHorizontalDistance();
        float getElevation();
        int getHeight();
        float getTilt();
        float getAspectRatio();
        int getTarget();
        void to_string();

    protected:
        // Data
        Rectangle inputRect;
        Constants *constList;
        int target;
        float azimuth;
        int proportionalDistance;
        int constantsDistance;
        float logAspectRatios;
        int horizontalDistance;
        float elevation;
        int height;
        float tilt;
        float aspectRatio;

        // Methods
        void computeProportionalDistance(); // Computes distance with rectangle proportions
        void computeConstantsDistance(); // Computes distance with constants values
        void computeLogAspectRatios(); // Computes log of calculated aspect ratio to true aspect ratio
        void computeHorizontalDistance(); // Computes Horizontal Distance to the Rectangle
        void computeElevation(); // Computes Elevation of the Rectangle
        void computeHeight(); // Computes Height of the Rectangle
        void computeAzimuth(); // Computes Azimuth to the Rectangle
        void fixHeight(); // Changes Lowest Hoop to Highest
        void computeTilt(); // Computes Tilt of the Rectangle
        void computeAspectRatio(); // Computes aspect ratio to determine which goal
};

#endif /* RectangleProcessor_hpp */
