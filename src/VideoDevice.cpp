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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

#include "Constants.hpp"
#include "VideoDevice.hpp"

// Public Methods
VideoDevice::VideoDevice(Constants *inputConstList)
{
        // Variable Declarations
        isFinished = 0;
        isReady = 0;
        constList = *inputConstList;
}

void VideoDevice::startCapture(int deviceID)
{
        initCamera(deviceID);
        captureThread = new std::thread(&VideoDevice::captureFromCamera, this);
}

cv::Mat VideoDevice::getImage()
{
        while (! isReady)
		std::cout << "Waiting for camera..." << std::endl;
        return image;
}

// Private Methods
void VideoDevice::initCamera(int deviceID)
{
        // Get Video Capture Device
        try {
                camera.open(deviceID);
                if (!camera.isOpened()) {
			std::stringstream errorMsg;
                        errorMsg << "Unable to open capture device /dev/video" << deviceID;
                        throw errorMsg.str();
                }
        } catch (std::string errMsg) {
		std::cout << errMsg << std::endl;
                exit(1);
        }
}

void VideoDevice::captureFromCamera()
{
        while (! isFinished) {
                camera >>image;
                isReady = 1;
                constList.imgCols = image.cols;
                constList.imgRows = image.rows;
        }
}

VideoDevice::~VideoDevice()
{
        isFinished = 1;
        captureThread->join();
        delete captureThread;
}
