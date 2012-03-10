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

#ifndef hsvimage_hpp
#define hsvimage_hpp

class HSVImage {
public:
  // Functions
  HSVImage(cv::Mat &); // Constructor
  void LoadRGBImage(cv::Mat &); // Loads RGB Image

  // Variables
  cv::Mat rgb;
  cv::Mat hsv;
  cv::Mat hue;
  cv::Mat saturation;
  cv::Mat value;
private:
};

#endif /* hsvimage_hpp */
