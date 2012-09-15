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

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <cstring>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>
#include <set>
#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <getopt.h>
#include <unistd.h>

#include "Constants.hpp"
#include "VideoDevice.hpp"
#include "Rectangle.hpp"
#include "RectangleDetector.hpp"
#include "RectangleProcessor.hpp"
#include "GUIManager.hpp"
#include "CRIOLink.hpp"

void drawRectangles(cv::Mat& image, const std::vector<std::vector<cv::Point> > &allRectangles, const std::vector<std::vector<cv::Point> >& finalRectangles)
{
        for (size_t i = 0; i < allRectangles.size(); i++) {
                const cv::Point* p = &allRectangles.at(i).at(0);
                int n = (int)allRectangles.at(i).size();
		cv::polylines(image, &p, &n, 1, true, cv::Scalar(0, 255, 0), 3, CV_AA);
        }

        for (size_t i = 0; i < finalRectangles.size(); ++i) {
                const cv::Point* p = &finalRectangles.at(i).at(0);
                int n = (int)finalRectangles.at(i).size();
		cv::polylines(image, &p, &n, 1, true, cv::Scalar(255, 0, 0), 3, CV_AA);
        }
}

int main(int argc, char* argv[])
{

        Constants constList;
	std::stringstream fileName;
	std::stringstream deviceName;
        int isFile = 0;
        int isDevice = 0;
        int isHeadless = 0;
        int isNetworking = 1;
        int firstRun = 1;

        while (true) {
                static struct option long_options[] = {
                        {"headless", no_argument, &isHeadless, 1},
                        {"no-networking", no_argument, &isNetworking, 0},
                        {"device", required_argument, 0, 'd'},
                        {"file", required_argument, 0, 'f'},
                        {0, 0, 0, 0}
                };

                int option_index = 0;
                int c = getopt_long(argc, argv, "d:f:", long_options, &option_index);

                if (c == -1) // End of Options
                        break;

                switch (c) {
                case 0:
                        break;
                case 'd':
                        deviceName <<optarg;
                        isDevice = 1;
                        break;
                case 'f':
                        fileName <<optarg;
                        isFile = 1;
                        break;
                case '?':
                        exit(127);
                        break;
                default:
                        abort();
                }
        }

        if (! isFile && ! isDevice) {
		std::cout << argv[0] << ": missing required options" << std::endl;
                exit(127);
        }

        // Initialize Video Device
        VideoDevice videoDevice(&constList);
        if (isDevice) {
                videoDevice.startCapture(atoi(deviceName.str().c_str()));
        }

        // Initialize CRIO Communication Link
        CRIOLink cRIOLink;
        if (isNetworking)
                cRIOLink.initServer();

        // Initialize GUI
        GUIManager guiManager(&constList);
        if (! isHeadless)
                guiManager.init();

        while (true) {
		std::vector<std::vector<cv::Point> > allRectangles;
		std::vector<std::vector<cv::Point> > finalRectangles;
                int aquired = 0;
                int distanceMM;
                int horizontalDistanceMM;
                int heightMM;
                float velocity;
                float azimuthRadians;
                float azimuthDegrees;
                float tiltRadians;
                float tiltDegrees;
		cv::Mat original;
		cv::Mat output;

                if (isNetworking)
                        cRIOLink.waitForPing();

                if (isFile)
                        original = cv::imread(fileName.str().c_str()); // Load Image from File
                else if (isDevice)
                        videoDevice.getImage().copyTo(original); // Load Image from Camera
                else
                        exit(1);

                // Print Variables
                if (firstRun && ! isHeadless)
			std::cout << "Camera Resolution: " << original.cols << "x" << original.rows << std::endl;

                // Process Rectangle
                RectangleDetector rectDetector(&constList);
                Rectangle foundRectangle = rectDetector.processImage(original);

                if (rectDetector.rectangleWasFound()) {
                        aquired = 1;

                        // Process Rectangle
                        RectangleProcessor rectProcessor(&constList);
                        rectProcessor.processRectangle(foundRectangle);

                        // Retrieve Data
                        distanceMM = rectProcessor.getDistance();
                        horizontalDistanceMM = rectProcessor.getHorizontalDistance();
                        heightMM = rectProcessor.getHeight();
                        velocity = rectProcessor.getVelocity();
                        azimuthRadians = rectProcessor.getAzimuth();
                        tiltRadians = rectProcessor.getTilt();
                        allRectangles = rectDetector.getAllRectangles();
                        finalRectangles = rectDetector.getFinalRectangles();

                        // Convert Data
                        azimuthDegrees = (azimuthRadians * 180.0) / constList.mathPi;
                        tiltDegrees = (tiltRadians * 180.0) / constList.mathPi;

                        // Print Data
                        if (! isHeadless) {
				std::cout << "Distance: " << distanceMM << " mm" << std::endl;
				std::cout << "Horizontal Distance: " << horizontalDistanceMM << " mm" << std::endl;
				std::cout << "Height: " << heightMM << " mm" << std::endl;
				std::cout << "Velocity: " << velocity << " m/s" << std::endl;
				std::cout << "Azimuth: " << azimuthDegrees << " degrees, " << azimuthRadians << " radians" << std::endl;
				std::cout << "Tilt: " << tiltDegrees << " degrees, " << tiltRadians << " radians" << std::endl;

                        }

                        // Send Data
                        if (isNetworking)
                                cRIOLink.sendData(velocity, heightMM, azimuthRadians, tiltRadians);
                } else {
                        // Print Data
                        if (! isHeadless)
				std::cout << "No rectangle" << std::endl;

                        // Send Data
                        if (isNetworking)
                                cRIOLink.sendData();
                }

                // Write Data to Original Image
                int dataPointX = 0;
                int dataPointY = constList.imgRows - 5;
		cv::Point dataCoordinates(dataPointX, dataPointY);
                int fontFace = cv::FONT_HERSHEY_COMPLEX;
                double fontScale = (float)original.cols / 400.0;
		cv::Scalar fontColor(0.0, 255.0, 0.0, 0.0);
                int fontThickness = 2;
		std::stringstream data;
                if (aquired)
                        data << distanceMM << "mm @ " << azimuthDegrees << " degrees";
                else
                        data << "No rectangle";
		cv::putText(original, data.str(), dataCoordinates, fontFace, fontScale, fontColor, fontThickness);

                if (! isHeadless) {
                        drawRectangles(original, allRectangles, finalRectangles); // Draw Rectangles
                        guiManager.show(original); // Show Image
                }

                firstRun = 0;

                if (! isHeadless) {
                        int keycode = cv::waitKey(10);
                        if (keycode == 57) {
				std::stringstream filename;
                                filename << time(NULL) << ".jpg";
				cv::imwrite(filename.str().c_str(), output);
                        } else if (keycode == 27)
                                exit(EXIT_SUCCESS);
                        if (isFile) {
				cv::waitKey();
                                exit(EXIT_SUCCESS);
                        }
                } else {
                        if (isNetworking && isFile)
                                cRIOLink.waitForPing();
                }
        }

        return EXIT_SUCCESS;
}
