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

#ifndef VideoDevice_hpp
#define VideoDevice_hpp

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <thread>

#include "Constants.hpp"

class VideoDevice
{
public:
        // Methods
        VideoDevice(Constants *constList);
        void startCapture(int devID);
        cv::Mat getImage();
        ~VideoDevice();

private:
        // Methods
        void initCamera(int devID);
        void captureFromCamera();

        // Data
        Constants constList;
        std::thread *captureThread;
        cv::VideoCapture camera;
        cv::Mat image;
        int isFinished;
        int isReady;
};

#endif /* VideoDevice_hpp */
