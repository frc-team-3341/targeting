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

#ifndef MultiRectangleProcessor_hpp
#define MultiRectangleProcessor_hpp

#include <vector>

#include "Rectangle.hpp"
#include "Constants.hpp"

class MultiRectangleProcessor
{
    public:
        // Methods
        MultiRectangleProcessor(Constants* constList);
        ~MultiRectangleProcessor();
        void processRectangles(std::vector<Rectangle> foundRectangles);
        Rectangle* getFinalRectangle();
        void printFinalRectangleInformation();

    private:
        // Data
        std::vector<Rectangle> rectList;
        std::vector<Rectangle> rectangles;
        std::vector<Rectangle> verticalRectangleList;
        std::vector<Rectangle> horizontalRectangleList;

        Rectangle* finalRectangle;
        int finalTarget;
        Constants* constList;
        void findVerticalRectangles(); // Finds vertical Rectangles;
        void findHorizontalRectangles(); // Finds horizontal Rectangles;
        void findVHPairs(); // Finds horizontal Rectangles pairs;
        bool rectanglePairMatches(Rectangle, Rectangle); //Primary logic used by findVHPairs to detect whether rectangle pairs match.
        void classifyVHPairs();
        int classifyVHPair(Rectangle, Rectangle);
        bool foundFinal;
};

#endif /* MultiRectangleProcessor_hpp */
