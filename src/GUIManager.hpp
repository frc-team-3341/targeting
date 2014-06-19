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

#ifndef GUIManager_hpp
#define GUIManager_hpp

#include <string>

class GUIManager
{
public:
        GUIManager(Constants* constList); // Constructor
        void init(); // Initialize GUI
	void setImage(cv::Mat image);
	void setImageText(std::string imageText);
        void show(const std::vector<std::vector<cv::Point> > &allRectangles, const std::vector<std::vector<cv::Point> >& finalRectangles); // Show Image
	void show(); // Show Image

private:
        Constants *constList;
	cv::Mat image;
};

#endif /* GUIManager_hpp */
