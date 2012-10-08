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

#ifndef RectangleDetector_hpp
#define RectangleDetector_hpp

class RectangleDetector
{
public:
        // Methods
        RectangleDetector(Constants* constList);
        Rectangle processImage(cv::Mat inputImage);
        bool rectangleWasFound();
        std::vector< std::vector<cv::Point> > getAllRectangles();
        std::vector< std::vector<cv::Point> > getFinalRectangles();
private:
        // Data
        Constants *constList;
        cv::Mat image;
        std::vector< std::vector<cv::Point> > allRectangles;
        std::vector< std::vector<cv::Point> > finalRectangles;
        std::vector<Rectangle> rectList;
        std::vector<Rectangle> rectListRevised;
        bool foundRectangle;
        int rectIndex;

        // Methods
        double angle(cv::Point, cv::Point, cv::Point); // Find Cosine of Angle Between Vectors
        void preprocessImage(); // Threshold and Grayscale
        void findRectangles(); // Finds Rectangles
        void populateRectangles(); // Populates Vector of Rectangles
        void filterUniqueRectangles(); // Filter Unique Rectangles
        bool rectangleIsContained(Rectangle, Rectangle); // Check if Rectangle is Contained
        void findContainedRectangles(); // Finds Contained Rectangles
        void findCorrectRectangles(); // Figures Out Which Rectangles Are Correct
        void computeTilt(); // Computes Tilt of the Rectangle
};

#endif /* RectangleDetector_hpp */
