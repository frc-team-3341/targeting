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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "HSVImage.hpp"

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <ctime>

HSVImage::HSVImage(cv::Mat &InputImage)
{
        LoadRGBImage(InputImage);
}

void HSVImage::LoadRGBImage(cv::Mat &InputImage)
{
        InputImage.copyTo(rgb);
        cvtColor(InputImage, InputImage, CV_RGB2HSV); // Convert Image to HSV

        // Get HSV Slices
	std::vector<cv::Mat> slices;
        split(InputImage, slices);
        InputImage.copyTo(hsv);
        slices.at(0).copyTo(hue);
        slices.at(1).copyTo(saturation);
        slices.at(2).copyTo(value);
}
