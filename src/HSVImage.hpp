/*
 *    Copyright 2012, 2013, 2014 Tushar Pankaj
 *
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

#ifndef hsvimage_hpp
#define hsvimage_hpp

class HSVImage
{
public:
        // Functions
        HSVImage(cv::Mat &rgbImg); // Constructor
        void LoadRGBImage(cv::Mat &rgbImg); // Loads RGB Image

        // Variables
        cv::Mat rgb;
        cv::Mat hsv;
        cv::Mat hue;
        cv::Mat saturation;
        cv::Mat value;
private:
};

#endif /* hsvimage_hpp */
