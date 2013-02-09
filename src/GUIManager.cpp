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

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <cstring>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>
#include <set>

#include "Constants.hpp"
#include "GUIManager.hpp"

// Public Functions
GUIManager::GUIManager(Constants *inputConstList)
{
        // Variable Initializations
        constList = inputConstList;
}

void GUIManager::init()
{
	cv::namedWindow(constList->guiWindowName.c_str(), 0);
}

void GUIManager::setImage(cv::Mat inputImage)
{
	image = inputImage;
}

void GUIManager::setImageText(std::string imageText)
{
	cv::putText(image, imageText, cv::Point(0, constList->imgRows - 5), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(0.0, 255.0, 0.0, 0.0), 1);
}

void GUIManager::show(const std::vector<std::vector<cv::Point> > &allRectangles, const std::vector<std::vector<cv::Point> >& finalRectangles)
{
	for (size_t i = 0; i < allRectangles.size(); i++) {
                const cv::Point* p = &allRectangles.at(i).at(0);
                int n = (int)allRectangles.at(i).size();
		cv::polylines(image, &p, &n, 1, true, cv::Scalar(0, 255, 0), 1, CV_AA);
        }
	
        for (size_t i = 0; i < finalRectangles.size(); ++i) {
                const cv::Point* p = &finalRectangles.at(i).at(0);
                int n = (int)finalRectangles.at(i).size();
		cv::polylines(image, &p, &n, 1, true, cv::Scalar(255, 0, 0), 1, CV_AA);
        }
	
	cv::imshow(constList->guiWindowName.c_str(), image);
}
